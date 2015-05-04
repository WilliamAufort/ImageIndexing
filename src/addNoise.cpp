#include <DGtal/base/Common.h>
#include "../include/toolbox.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./addNoise image.pgm" << std::endl;
		return EXIT_FAILURE;
	}
	modifyImage(argv[1],"test.pgm");
	return 0;
}
