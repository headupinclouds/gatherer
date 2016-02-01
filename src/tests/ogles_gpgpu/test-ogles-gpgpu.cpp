#include <gtest/gtest.h>

extern const char* imageFilename;
extern const char* truthFilename;

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	assert(argc == 3);
	imageFilename = argv[1];
	truthFilename = argv[2];
	return RUN_ALL_TESTS();
}

