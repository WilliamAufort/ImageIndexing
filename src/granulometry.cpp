#include <iostream>
#include <vector>
#include "DGtal/base/Common.h"
#include "DGtal/io/readers/GenericReader.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/colormaps/HueShadeColorMap.h"
#include "DGtal/io/colormaps/GradientColorMap.h"
#include "DGtal/images/ImageSelector.h"
#include "DGtal/images/SimpleThresholdForegroundPredicate.h"
#include "DGtal/geometry/volumes/distance/DistanceTransformation.h"
#include "DGtal/shapes/implicit/ImplicitBall.h"
#include "DGtal/io/writers/PPMWriter.h"

using namespace std;
using namespace DGtal;

typedef ImageSelector<Z2i::Domain, unsigned int>::Type Image;

// Colormap used for the SVG output
typedef HueShadeColorMap<long int, 2> HueTwice;
typedef GradientColorMap<int> 		  Gradient;

// Point predicate
typedef functors::SimpleThresholdForegroundPredicate<Image> PointPredicate;

// Distance transformation
typedef DistanceTransformation<Z2i::Space, PointPredicate, Z2i::L2Metric> DTL2; 

//////////////////////////////////////////////////////////////////////

void buildHistogram(Image& granuloImage, unsigned int maxGranulo, unsigned int pas, unsigned int compteur, string fileName)
{
	vector<double> histo(pas+1,0.0);
	double cast_max = static_cast<double>(maxGranulo);
	double cast_pas = static_cast<double>(pas);
	double cast_compteur = static_cast<double>(compteur);
	for (Image::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
	{
		if (granuloImage.domain().isInside(*it)) /// inside the image
		{
			unsigned int normalizedValue = static_cast<unsigned int>(static_cast<double>(granuloImage(*it)) / cast_max * cast_pas);
			histo[normalizedValue]++;
		}
	}
	ofstream file(fileName.c_str());
    if (file.is_open())
	{    
		//for (unsigned int i = 0; i <= pas; ++i)
		// it seems that the points that doesn't belong to the object are considering in 
		// DT are in fact in the histogram... Just not consider them before close the problem
		// Another advantage of this is that we use a kind a filters of "bad" balls which can 
		// appear in the border
		for (unsigned int i= 1; i <= pas; ++i) 
		{
			histo[i] /= cast_compteur;
			file << i << " " << histo[i] << endl;
		}
	}	
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cerr << "Use : ./granulometry input.pgm output.svg" << endl;
		exit (1);
	}
	trace.beginBlock ("Granulometric computations");

	Image image = GenericReader<Image>::import(argv[1]);

	PointPredicate predicate(image,0);

	DTL2 dtL2(image.domain(), predicate, Z2i::l2Metric);

	// Granulometric function

	Image granuloImage (image.domain());
	for (Image::Range::Iterator it = granuloImage.range().begin(); it != granuloImage.range().end(); ++it)
		*it = 0;

	unsigned int compteur = 0; /// number of points in the image
	for (Image::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
	{
		if (dtL2(*it) > 0) /// inside the image
		{
			compteur++;
			// Build a sphere inside the object of radius dtL2(*it)
			unsigned int radius = static_cast<unsigned int>(dtL2(*it));
			Z2i::RealPoint center = *it;
			ImplicitBall<Z2i::Space> ball(center,radius);
			// To iterate on the sphere, we build a tangent bounding box around it and iterate into the box
			Z2i::Point top = center + Z2i::Point::diagonal(radius +1);
			Z2i::Point bottom = center - Z2i::Point::diagonal(radius +1);
			Z2i::Domain sphereDomain(bottom,top);
			for (Z2i::Domain::ConstIterator it = sphereDomain.begin(); it != sphereDomain.end(); ++it)
			{
				if ((granuloImage.domain().isInside(*it)) // Point inside the image
					&& (ball(*it) > 0)					  // Point inside the ball
					&& (granuloImage(*it) < radius)) 	  // Granulometric value has to be updated			
				{
					granuloImage.setValue(*it,radius);
				}
			}
		}	
	}
	trace.info() << "Granulometric function computed with " << compteur << " balls" << endl;

	unsigned int maxGranulo = 0;
	for (Image::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain	().end(); ++it)
		if (granuloImage(*it) > maxGranulo)
			maxGranulo = granuloImage(*it);

	// Save
	PPMWriter<Image,HueTwice>::exportPPM(argv[2],granuloImage,HueTwice(0,maxGranulo+1));	
//	PPMWriter<Image,Gradient>::exportPPM(argv[2],granuloImage,Gradient(1,maxGranulo+1));	

	// Build the histogramm
	unsigned int pas = 20;
	string fileName = "histo.txt";
	buildHistogram(granuloImage,maxGranulo,pas,compteur,fileName);

	trace.endBlock();
	return 0;
}
