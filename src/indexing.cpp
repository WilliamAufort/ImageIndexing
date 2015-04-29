#include "../include/granulometry.h"
#include "DGtal/io/readers/GenericReader.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

string changeExtension(string fileName)
{
	int lastIndex = fileName.find_last_of(".");
	string newFile = fileName.substr(0, lastIndex) + ".hist";
	return newFile;
}

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
	unsigned int nbPoints = granuloWithMedialAxis(image,granuloImage);

	unsigned int maxGranulo = 0;
	for (myLittleImage::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
		if (granuloImage(*it) > maxGranulo)
			maxGranulo = granuloImage(*it);

	unsigned int pas = 20;
	string fileName = changeExtension(argv[1]);
	buildHistogram(granuloImage,maxGranulo,pas,nbPoints,fileName);

	trace.endBlock();
	return 0;
}
