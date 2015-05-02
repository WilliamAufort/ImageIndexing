#include <DGtal/base/Common.h>
//#include "../include/toolbox.h"
#include "../include/transforms.h"
#include "DGtal/io/readers/GenericReader.h"
#include "DGtal/io/writers/PGMWriter.h"

using namespace std;
using namespace DGtal;

int main(int argc, char* argv[])
{
	/*trace.info() << "Helloworld from DGtal ";
	trace.emphase() << "(version "<< DGTAL_VERSION << ")"<< endl;
	string filename = "database/tree-1.pgm";
	trace.info() << extractClass(filename) << endl;
	string histoname = "histograms/database/device3-10.hist";
	trace.info() << extractClass(histoname) << endl;*/

	// test modifyImage
//	modifyImage("../database/octopus-1.pgm","octopus-test.pgm");
	myLittleImage image = GenericReader<myLittleImage>::import("octopus-test.pgm");
	myLittleImage filteredImage (image.domain());
	/*granuloWithMedialAxis(image, granuloImage);
	unsigned int maxGranulo = 0;
	for (myLittleImage::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
		if (granuloImage(*it) > maxGranulo)
			maxGranulo = granuloImage(*it);
	saveGranulo(granuloImage,maxGranulo,"granulo-noised.eps");*/
	applyFilter(image,filteredImage);
	PGMWriter<myLittleImage>::exportPGM("without-noise.pgm",filteredImage);
	
	return 0;
	
}
