// Example for C++11 hacking
// https://github.com/headupinclouds/thread-pool-cpp/blob/master/tests/thread_pool.t.cpp

#include <thread_pool.hpp>

#include <thread>
#include <future>
#include <functional>

#include <iostream>

#include <stdexcept>
#include <iostream>
#include <sstream>

#define ASSERT(expr) \
    if (!(expr)) { \
        std::ostringstream ss; \
        ss << __FILE__ << ":" <<__LINE__ << " " << #expr; \
        throw std::runtime_error(ss.str()); \
    }

template <typename TestFunc>
inline void doTest(const char *name, TestFunc &&test) {
    std::cout << " - test ( " << name;
    try {
        test();
    } catch (const std::exception &e) {
        std::cout << " => failed with: " << e.what() << " )" << std::endl;
        throw;
    }
    std::cout << " => succeed )" << std::endl;
}

int run_benchmark();
int run_test();

int main(int argc, char **argv)
{
    run_test();
    run_benchmark();
}

static const size_t CONCURRENCY = 16;
static const size_t REPOST_COUNT = 1000000;

struct Heavy
{
    bool verbose;
    std::vector<char> resource;
    
    Heavy(bool verbose = false)
    : verbose(verbose)
    , resource(100*1024*1024)
    {
        if (verbose) {
            std::cout << "heavy default constructor" << std::endl;
        }
    }
    
    Heavy(const Heavy &o)
    : verbose(o.verbose)
    , resource(o.resource)
    {
        if (verbose) {
            std::cout << "heavy copy constructor" << std::endl;
        }
    }
    
    Heavy(Heavy &&o)
    : verbose(o.verbose)
    , resource(std::move(o.resource))
    {
        if (verbose) {
            std::cout << "heavy move constructor" << std::endl;
        }
    }
    
    Heavy & operator==(const Heavy &o)
    {
        verbose = o.verbose;
        resource = o.resource;
        if (verbose) {
            std::cout << "heavy copy operator" << std::endl;
        }
        return *this;
    }
    
    Heavy & operator==(const Heavy &&o)
    {
        verbose = o.verbose;
        resource = std::move(o.resource);
        if (verbose) {
            std::cout << "heavy move operator" << std::endl;
        }
        return *this;
    }
    
    ~Heavy()
    {
        if (verbose) {
            std::cout << "heavy destructor. " << (resource.size() ? "Owns resource" : "Doesn't own resource") << std::endl;
        }
    }
};

typedef ThreadPool<128> MyThreadPool;

struct RepostJob
{
    //Heavy heavy;
    MyThreadPool *thread_pool;
    volatile size_t counter;
    long long int begin_count;
    std::promise<void> *waiter;
    
    RepostJob(MyThreadPool *thread_pool, std::promise<void> *waiter)
    : thread_pool(thread_pool)
    , counter(0)
    , waiter(waiter)
    {
        begin_count = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    }
    
    void operator()()
    {
        if (counter++ < REPOST_COUNT) {
            if (thread_pool) {
                thread_pool->post(*this);
                return;
            }
        }
        else {
            long long int end_count = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            std::cout << "reposted " << counter
            << " in " << (double)(end_count - begin_count)/(double)1000000 << " ms"
            << std::endl;
            waiter->set_value();
        }
    }
};

int run_benchmark()
{
    std::cout << "Benchmark job reposting" << std::endl;
    
    {
        std::cout << "***thread pool cpp***" << std::endl;
        
        std::promise<void> waiters[CONCURRENCY];
        MyThreadPool thread_pool;
        for (auto &waiter : waiters) {
            thread_pool.post(RepostJob(&thread_pool, &waiter));
        }
        
        for (auto &waiter : waiters) {
            waiter.get_future().wait();
        }
    }
    
    return 0;
}

int run_test()
{
    std::cout << "*** Testing ThreadPool ***" << std::endl;
    
    doTest("post job", []() {
        MyThreadPool pool;
        
        std::packaged_task<int()> t([](){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            return 42;
        });
        
        std::future<int> r = t.get_future();
        
        // Note: fastest variant (compared to 'process()' given prepackaged task)
        pool.post(t);
        
        ASSERT(42 == r.get());
    });
    
    doTest("process job", []() {
        MyThreadPool pool;
        
        // Note: This method of posting job to thread pool is much slower than 'post()' due to std::future and
        std::future<int> r = pool.process([]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            return 42;
        });
        
        ASSERT(42 == r.get());
    });
    
    struct my_exception {};
    
    doTest("process job with exception", []() {
        MyThreadPool pool;
        
        std::future<int> r = pool.process([]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            throw my_exception();
            return 42;
        });
        
        try {
            ASSERT(r.get() == 42 && !"should not be called, exception expected");
        } catch (const my_exception &e) {
        }
    });
    
    return 0;
}
