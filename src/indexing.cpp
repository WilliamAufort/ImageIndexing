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

	Image image = GenericReader<Image>::import(argv[1]);

	// Granulometric function

	Image granuloImage (image.domain());
	for (Image::Range::Iterator it = granuloImage.range().begin(); it != granuloImage.range().end(); ++it)
		*it = 0;

	unsigned int nbBalls = buildGranulo(image,granuloImage);

	trace.info() << "Granulometric function computed with " << nbBalls << " balls" << endl;

	unsigned int maxGranulo = 0;
	for (Image::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
		if (granuloImage(*it) > maxGranulo)
			maxGranulo = granuloImage(*it);

	// Save	
	// saveGranulo(granuloImage, maxGranulo, argv[2]);
	
	// Build the histogramm
	unsigned int pas = 20;
	string fileName = changeExtension(argv[1]);
	buildHistogram(granuloImage,maxGranulo,pas,nbBalls,fileName);

	trace.endBlock();
	return 0;
}
