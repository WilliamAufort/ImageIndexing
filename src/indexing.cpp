#include "../include/granulometry.h"
#include "DGtal/io/readers/GenericReader.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cerr << "Use: ./indexing input.pgm" << endl;
		exit (1);
	}
	trace.beginBlock ("Granulometric computations");

	myLittleImage image = GenericReader<myLittleImage>::import(argv[1]);
	myLittleImage granuloImage (image.domain());
	for (myLittleImage::Range::Iterator it = granuloImage.range().begin(); it != granuloImage.range().end(); ++it)
		*it = 0;

	unsigned int nbBalls = granuloWithMedialAxis(image,granuloImage);

	trace.info() << "Granulometric function computed with " << nbBalls << " balls" << endl;

	unsigned int maxGranulo = 0;
	unsigned int nbPoints = 0;
	for (myLittleImage::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
	{	
		if (granuloImage(*it) > 0)
			nbPoints++;
		if (granuloImage(*it) > maxGranulo)
			maxGranulo = granuloImage(*it);
	}

	unsigned int pas = 20;
	string fileName = changeExtension(argv[1]);
	buildHistogram(granuloImage,maxGranulo,pas,nbPoints,fileName);

	trace.endBlock();
	return 0;
}
