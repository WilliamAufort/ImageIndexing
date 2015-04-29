#include <iostream>
#include <vector>
#include "DGtal/io/boards/Board2D.h"
#include "../include/granulometry.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

/// Naive algorithm

unsigned int buildNaiveGranulo(myLittleImage& image, myLittleImage& granuloImage)
{
	PointPredicate predicate(image,0);
	DTL2 dtL2(image.domain(), predicate, l2Metric);
	unsigned int compteur = 0; // number of points in the image
	for (myLittleImage::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
		if (dtL2(*it) > 0) // inside the image
		{
			compteur++;
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
					&& (ball(*ite) > 0)					  // Point inside the ball
					&& (granuloImage(*ite) < radius)) 	  // Granulometric value has to be updated
				{
					granuloImage.setValue(*ite,radius);
				}
			}
		}
	return compteur;
}

/// Advanced granulometric algorithm

unsigned int granuloWithMedialAxis(myLittleImage& image, myLittleImage& granuloImage)
{
	PointPredicate predicate(image,0);
	Z2i::L2PowerMetric l2power;
	unsigned int compteurBalls = 0;
	unsigned int compteurPoints = 0;
	DTL2 dtL2(image.domain(), predicate, l2Metric);
    Map power(image.domain(), dtL2, l2power);
	RMA::Type rma = RMA::getReducedMedialAxisFromPowerMap(power);
	for (myLittleImage::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
	{
		if (dtL2(*it) > 0)
			compteurPoints++;

		if (rma(*it) > 0)
		{
			compteurBalls++;
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
	trace.info() << "Granulometric function computed with " << compteurBalls << " balls" << endl;
	return compteurPoints;
}

/// A printer for granulometric images

void saveGranulo(myLittleImage& granuloImage, unsigned int maxGranulo, string fileName)
{
	Board2D board;
	HueTwice colorMap(1,maxGranulo+1);
	// Some constants to do the drawing
	Point O(0,0);
	string specificStyle =  O.className() + "/Paving"; // Point style
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

/// Build the histograms used in indexing

void buildHistogram(myLittleImage& granuloImage, unsigned int maxGranulo, unsigned int pas, unsigned int compteur, string fileName)
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
	ofstream file(fileName.c_str());
    if (file.is_open())
	{
		// for (unsigned int i = 0; i <= pas; ++i)
		// it seems that the points that doesn't belong to the object are considering in
		// DT are in fact in the histogram... Just not consider them before close the problem
		// Another advantage of this is that we use a kind a filter of "bad" balls which can
		// appear in the border
		for (unsigned int i= 1; i <= pas; ++i)
			file << i << " " << histo[i] / cast_compteur << endl;
	}
}

/// To test the speed of the two algorithms (used in testEfficiency.cpp)

void testSpeed(function<unsigned int(myLittleImage&, myLittleImage&)> &granulo, myLittleImage& image, const char* inputFile)
{
	trace.beginBlock ("Test");

	myLittleImage granuloImage (image.domain());
	unsigned int nbBalls = granulo(image,granuloImage);
	trace.info() << "Granulometric function computed with " << nbBalls << " balls" << endl;
	trace.endBlock();
	return;
}

void testSpeedNaive(myLittleImage& image, const char* inputFile)
{
	function<unsigned int(myLittleImage&, myLittleImage&)> f = &buildNaiveGranulo;
	testSpeed(f,image,inputFile);	
}

void testSpeedQuick(myLittleImage& image, const char* inputFile)
{
	function<unsigned int(myLittleImage&, myLittleImage&)> f = &granuloWithMedialAxis;
	testSpeed(f,image,inputFile);	
}

