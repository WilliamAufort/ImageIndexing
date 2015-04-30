#include "../include/distance.h"
#include "../include/granulometry.h"
#include <iostream>

using namespace std;
using namespace DGtal;
using namespace Z2i;

int main (int argc, char* argv[])
{
	if (argc != 3)
	{
		cerr << "Use: ./retrieval image.pgm histo.hist" << endl;
		exit (1);
	}
	trace.beginBlock ("Distance computations");
	cout << distanceImageToHisto(argv[1],argv[2]);
	trace.endBlock();
	return 0;
}
