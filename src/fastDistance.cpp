#include "../include/distance.h"
#include "../include/granulometry.h"
#include "../include/toolbox.h"
#include <iostream>

// scaling
#include <DGtal/images/ConstImageAdapter.h>
#include <DGtal/kernel/BasicPointFunctors.h>
#include "DGtal/io/readers/GenericReader.h"
#include "DGtal/io/writers/GenericWriter.h"
#include "DGtal/images/ImageContainerBySTLVector.h"
#include "DGtal/images/ImageSelector.h"
#include <DGtal/images/IntervalForegroundPredicate.h>
#include <cmath>

using namespace std;
using namespace DGtal;
using namespace Z2i;

typedef ImageSelector < Z2i::Domain, unsigned char>::Type MyImage;

/*
* Compute the number of pixels in an image
*
* @param image The image
*
* @return Its size
*/
double sizeOfImage(MyImage& image)
{
	Point p = image.domain().upperBound() - image.domain().lowerBound();
	return static_cast<double>(p[0]*p[1]);
}


/*
* Do a scaling with a convinient factor (at the end, 300x300 image)
*
* @param input The filename of the image file
*
* @param output The filename where we store the scaled image
*
* Note: ideas from imgScale.cpp of David Coeurjolly
*/
void scaling(string input, string output)
{
	MyImage image = GenericReader<MyImage>::import( input );
	double size = sizeOfImage(image);
	double scale = 1.0;
	if (size >= 300.0*300.0)
		scale = sqrt((300.0 * 300.0) / size);

	vector<double> scales(2,1.0/scale);
   	typedef functors::BasicDomainSubSampler< MyImage::Domain ,Z2i::Integer, double > ReSampler;
   	ReSampler reSampler(image.domain(), scales, Z2i::Point(0,0));
	typedef DGtal::ConstImageAdapter<MyImage , MyImage::Domain, ReSampler, MyImage::Value,DGtal::functors::Identity >  SamplerImageAdapter;
	SamplerImageAdapter sampledImage (image,reSampler.getSubSampledDomain(), reSampler, functors::Identity()); 
	sampledImage >> output;
}

int main (int argc, char* argv[])
{
	if (argc != 3)
	{
		cerr << "Use: ./fastDistance image1.pgm image2.pgm" << endl;
		exit (1);
	}
	scaling(argv[1],"tmp1.pgm");
	scaling(argv[2],"tmp2.pgm");
    vector<double> histo1 = imageToHistogram("tmp1.pgm", true);
    vector<double> histo2 = imageToHistogram("tmp2.pgm");
	cout << EMD(histo1, histo2) << endl;
	return 0;
}
