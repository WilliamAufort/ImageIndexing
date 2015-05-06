#include <DGtal/base/Common.h>
#include "../include/toolbox.h"
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 2 && argc != 3)
	{
		cerr << "Usage: ./addNoise image.pgm (noise)" << endl;
		return EXIT_FAILURE;
	}

	double noise;
	if (argc == 2)
		noise = 0.5;
	else
		noise = stod(argv[2]);

	modifyImage(argv[1],"test.pgm");
	return 0;
}
