#include "../include/granulometry.h"
#include "DGtal/io/readers/GenericReader.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cerr << "Use: ./testEfficiency input.pgm" << endl;
		exit (1);
	}
	myLittleImage image = GenericReader<myLittleImage>::import(argv[1]);

	// First method	
	trace.beginBlock ("Naive method");
	testSpeedNaive(image,argv[1]);
	trace.endBlock();

	// Second method
	trace.beginBlock("Medial axis method");
	testSpeedQuick(image,argv[1]);
	trace.endBlock();
	return 0;
}
