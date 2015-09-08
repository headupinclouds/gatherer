#include "opencv2/opencv.hpp"

int main(int, char**)
{
    cv::VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
    {
	std::cerr << "Could not open camera" << std::endl;
        return -1;
    }

    for(;;)
    {
	cv::Mat frame;
        cap >> frame; // get a new frame from camera
	
	std::cout << frame.size() << " " << cv::mean(frame)[0] << std::endl;
    }
    return 0;
}
