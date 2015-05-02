#include <iostream>
#include <vector>
#include "DGtal/io/boards/Board2D.h"
#include "../include/granulometry.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

/**
* Compute the granulometric function on a given image using the naive algorithm (see the report)
*
* @param image The input image 
*
* @param granuloImage The image where we store the granulometric function values
*/
unsigned int buildNaiveGranulo(myLittleImage& image, myLittleImage& granuloImage)
{
	PointPredicate predicate(image,0);
	DTL2 dtL2(image.domain(), predicate, l2Metric);
	unsigned int nbPoints = 0;
	for (myLittleImage::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
		if (dtL2(*it) > 0) // inside the image
		{
			nbPoints++;
			// Build a sphere inside the object of radius dtL2(*it)
			unsigned int radius = static_cast<unsigned int>(dtL2(*it));
			RealPoint center = *it;
			ImplicitBall<Space> ball(center,radius);
			// To iterate on the sphere, we build a tangent bounding box around it and iterate into the box
			Point top = center + Point::diagonal(radius +1);
			Point bottom = center - Point::diagonal(radius +1);
			Domain sphereDomain(bottom,top);
			for (Domain::ConstIterator ite = sphereDomain.begin(); ite != sphereDomain.end(); ++ite)
			{
				if ((granuloImage.domain().isInside(*ite)) // Point inside the image
					&& (ball(*ite) > 0)					   // Point inside the ball
					&& (granuloImage(*ite) < radius)) 	   // Granulometric value has to be updated
				{
					granuloImage.setValue(*ite,radius);
				}
			}
		}
	return nbPoints;
}

/**
* Compute the granulometric function with a smarter algorithm that uses the medial axis (see report for more details)
*
* @param image The input image 
*
* @param granuloImage The image where we store the granulometric function values
*/
unsigned int granuloWithMedialAxis(myLittleImage& image, myLittleImage& granuloImage)
{
	PointPredicate predicate(image,0);
	Z2i::L2PowerMetric l2power;
	unsigned int nbBalls = 0;
	unsigned int nbPoints = 0;
	DTL2 dtL2(image.domain(), predicate, l2Metric);
    Map power(image.domain(), dtL2, l2power);
	RMA::Type rma = RMA::getReducedMedialAxisFromPowerMap(power);
	for (myLittleImage::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
	{
		if (dtL2(*it) > 0) // Inside the image
			nbPoints++;

		if (rma(*it) > 0) // Center of a ball
		{
			nbBalls++;
			unsigned int radius = static_cast<unsigned int>(dtL2(*it));
			RealPoint center = *it;
			ImplicitBall<Space> ball(center,radius);
			// To iterate on the sphere, we build a tangent bounding box around it and iterate into the box
			Point top = center + Point::diagonal(radius +1);
			Point bottom = center - Point::diagonal(radius +1);
			Domain sphereDomain(bottom,top);
			for (Domain::ConstIterator ite = sphereDomain.begin(); ite != sphereDomain.end(); ++ite)
			{
				if ((granuloImage.domain().isInside(*ite)) // Point inside the image
					&& (ball(*ite) > 0)					  // Point inside the ball
					&& (granuloImage(*ite) < radius)) 	  // Granulometric value has to be updated
				{
					granuloImage.setValue(*ite,radius);
				}
			}
		}
	}
	trace.info() << "Granulometric function computed with " << nbBalls << " balls" << endl;
	return nbPoints;
}

/**
* Save the granometric function values stored in an image into an EPS file
*
* @param granuloImage The image containing the granulometric function values
*
* @param maxGranulo The maximum value of the granulometric function
*
* @param fileName A string containing the filename where we store the image
*/
void saveGranulo(myLittleImage& granuloImage, unsigned int maxGranulo, string fileName)
{
	Board2D board;
	HueTwice colorMap(1,maxGranulo+1);
	Point O(0,0);
	string specificStyle =  O.className() + "/Paving";
	Color white(255,255,255);
    board << SetMode(granuloImage.domain().className(), "Paving")
      	  << granuloImage.domain()
          << SetMode(O.className(), "Paving");

   	for (myLittleImage::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
    {
    	if (granuloImage(*it) > 0)
		{
			Color c = colorMap(granuloImage(*it));
       		board << CustomStyle(specificStyle, new CustomColors(c,c)) << *it;
		}
		else
			board << CustomStyle(specificStyle, new  CustomColors(white,white)) << *it;
    }
    board.saveEPS(fileName.c_str());
}

/**
* Compute the histogram of a granulometric image and save it in a file
*
* @param granuloImage The image containing the granulometric values
*
* @param maxGranulo The maximum of the granulometric values
*
* @param pas The number of bins of the histogram
*
* @param nbPoints The number of points inside the object contained in granuloImage
*
* @param fileName The filename where we store the histogram
*/
void buildHistogram(myLittleImage& granuloImage, unsigned int maxGranulo, unsigned int pas, unsigned int nbPoints, string fileName)
{
	vector<double> histo(pas+1,0.0);
	double cast_max = static_cast<double>(maxGranulo);
	double cast_pas = static_cast<double>(pas);
	double cast_nbPoints = static_cast<double>(nbPoints);
	for (myLittleImage::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
	{
		if (granuloImage.domain().isInside(*it)) // inside the image
		{
			unsigned int normalizedValue = static_cast<unsigned int>(static_cast<double>(granuloImage(*it)) / cast_max * cast_pas);
			histo[normalizedValue]++;
		}
	}
	ofstream file(fileName.c_str());
    if (file.is_open())
	{
		// for (unsigned int i = 0; i <= pas; ++i)
		// it seems that the points that doesn't belong to the object are considering in
		// DT are in fact in the histogram... Just not consider them before close the problem
		// Another advantage of this is that we use a kind a filter of "bad" balls which can
		// appear in the border
		for (unsigned int i= 1; i <= pas; ++i)
			file << i << " " << histo[i] / cast_nbPoints << endl;
	}
}

/**
* Compute the histogram of a granulometric image, as previously, but return it
*
* @param granuloImage The image containing the granulometric values
*
* @param maxGranulo The maximum of the granulometric values
*
* @param pas The number of bins of the histogram
*
* @param nbPoints The number of points inside the object contained in granuloImage
*
* @return The histogram as a vector of doubles
*/
vector<double> buildHistogram(myLittleImage& granuloImage, unsigned int maxGranulo, unsigned int pas, unsigned int compteur)
{
	vector<double> histo(pas+1,0.0);
	double cast_max = static_cast<double>(maxGranulo);
	double cast_pas = static_cast<double>(pas);
	double cast_compteur = static_cast<double>(compteur);
	for (myLittleImage::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
	{
		if (granuloImage.domain().isInside(*it)) // inside the image
		{
			unsigned int normalizedValue = static_cast<unsigned int>(static_cast<double>(granuloImage(*it)) / cast_max * cast_pas);
			histo[normalizedValue]++;
		}
	}

	for(size_t i=0; i<histo.size(); ++i)
        histo[i] /= cast_compteur;

    histo.erase(histo.begin());

	return histo;
}

/**
* Test the speed of any algorithm which computes the granulometric function (used in testEfficiency.cpp)
*
* @param granulo The granulometric algorithm used
*
* @param image The input image of the algorithm
*/
void testSpeed(function<unsigned int(myLittleImage&, myLittleImage&)> &granulo, myLittleImage& image)
{
	trace.beginBlock ("Test");

	myLittleImage granuloImage (image.domain());
	unsigned int nbBalls = granulo(image,granuloImage);
	trace.info() << "Granulometric function computed with " << nbBalls << " balls" << endl;
	trace.endBlock();
	return;
}

/**
* Test the speed of the naive algorithm
*
* @param image The input image of the algorithm
*/
void testSpeedNaive(myLittleImage& image)
{
	function<unsigned int(myLittleImage&, myLittleImage&)> f = &buildNaiveGranulo;
	testSpeed(f,image);
}

/**
* Test the speed of the second algorithm
*
* @param image The input image of the algorithm
*/
void testSpeedQuick(myLittleImage& image)
{
	function<unsigned int(myLittleImage&, myLittleImage&)> f = &granuloWithMedialAxis;
	testSpeed(f,image);
}
