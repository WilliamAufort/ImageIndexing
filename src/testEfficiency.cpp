#include "../include/granulometry.h"
#include "DGtal/io/readers/GenericReader.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

/**
* A little function to see the computation time of the two algorithms discribed to compute the granulometric function
*/

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cerr << "Use: ./testEfficiency input.pgm" << endl;
		exit (1);
	}
	myLittleImage image = GenericReader<myLittleImage>::import(argv[1]);

	trace.beginBlock ("Naive method");
	testSpeedNaive(image,argv[1]);
	trace.endBlock();

	trace.beginBlock("Medial axis method");
	testSpeedQuick(image,argv[1]);
	trace.endBlock();
	return 0;
}
