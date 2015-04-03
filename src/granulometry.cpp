#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/io/readers/GenericReader.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/colormaps/HueShadeColorMap.h"
#include "DGtal/io/boards/Board2D.h"
#include "DGtal/images/ImageSelector.h"
#include "DGtal/images/SimpleThresholdForegroundPredicate.h"
#include "DGtal/geometry/volumes/distance/DistanceTransformation.h"
#include "DGtal/shapes/implicit/ImplicitBall.h"

using namespace std;
using namespace DGtal;

typedef ImageSelector<Z2i::Domain, unsigned int>::Type Image;

// Colormap used for the SVG output
typedef HueShadeColorMap<long int, 2> HueTwice;

// Point predicate
typedef functors::SimpleThresholdForegroundPredicate<Image> PointPredicate;

// Distance transformation
typedef DistanceTransformation<Z2i::Space, PointPredicate, Z2i::L2Metric> DTL2; 

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cerr << "Use : ./granulometry input.pgm output.svg" << endl;
		exit (1);
	}
	trace.beginBlock ("Granulometric computations");

	Image image = GenericReader<Image>::import(argv[1]);
	Board2D board;

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
	board.clear();	
	Display2DFactory::drawImage<HueTwice>(board, granuloImage, 0.0, maxGranulo + 1);
	board.saveSVG(argv[2]);

	trace.endBlock();
	return 0;
}
