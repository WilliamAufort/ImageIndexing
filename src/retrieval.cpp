#include "../include/distance.h"
#include "../include/granulometry.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

int main (int argc, char* argv[])
{
	if (argc != 3)
	{
		cerr << "Use: ./retrieval image1.pgm image2.pgm" << endl;
		exit (1);
	}
	trace.beginBlock ("Distance computations");
	// Some calculations
	trace.endBlock();
	return 0;
}
