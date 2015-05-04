#include <DGtal/base/Common.h>
#include "../include/toolbox.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
		return EXIT_FAILURE;
	// test modifyImage
	modifyImage(argv[1],"test.pgm");
	return 0;
}
