#include <iostream>
#include <fstream>
#include <cmath>
#include "../include/distance.h"
#include "../include/granulometry.h"
#include "../include/transforms.h"
#include "DGtal/io/readers/GenericReader.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

/**
* Read a histogram from a file
*
* @param fileName The filename of the histogram
*
* @return The histogram as a vector of double
*/
vector<double> readHisto(string fileName)
{
	vector<double> histo;
	ifstream file(fileName.c_str());
	if (file.is_open())
	{
		bool isData = false;
		string word;
		while(file >> word)
		{
			if (isData)
				histo.push_back(stod(word));
			isData = not(isData);
		}
		return histo;
	}
}

/**
* The distance between two histograms. This is an edit distance between histograms based on the displacements needed to reach the second
* histogram from the first
*
* @param histo1 An histogram
*
* @param histo2 An histogram
*
* @return The distance between the two histograms
*/
double EMD(vector<double> histo1, vector<double> histo2)
{
	double EMD = 0;
	double tmp = 0;
	for (unsigned int i = 0; i < histo1.size(); ++i)
	{
		tmp += histo1[i] - histo2[i];
		EMD += abs(tmp);
	}
	return EMD;
}

/**
* Given an image, this function computes the histogram and save it in a file. The histogram filename is the same as the image filename
* except the extension is changed for .hist
*
* @param filename The filename of the image
*/
void saveHistogramOfImage(string filename)
{
    myLittleImage image = GenericReader<myLittleImage>::import(filename);
	PointPredicate predicate(image,0);
	DTL2 dtL2(image.domain(), predicate, l2Metric);
	myLittleImage granuloImage (image.domain());
	unsigned int nbPoints = granuloWithMedialAxis(image,granuloImage);
	unsigned int maxGranulo = 0;
	for (myLittleImage::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
		if (granuloImage(*it) > maxGranulo)
			maxGranulo = granuloImage(*it);
	unsigned int pas = 20;
	string fileName = changeExtension(filename);
	buildHistogram(granuloImage, maxGranulo, pas, nbPoints, fileName);
}

/**
* Given an image, this function computes the histogram
*
* @param filename The filename of the image
*
* @param smooth A boolean to set the activation of the anti-noise filter on the image
*
* @return The histogram of the image
*/
vector<double> imageToHistogram(string filename, bool smooth)
{
    myLittleImage image = GenericReader<myLittleImage>::import(filename);
    myLittleImage filteredImage (image.domain());
    if(smooth)
    {
        applyFilter(image, filteredImage);
        image = filteredImage;
    }
	PointPredicate predicate(image,0);
	DTL2 dtL2(image.domain(), predicate, l2Metric);
	myLittleImage granuloImage (image.domain());
	unsigned int nbPoints = granuloWithMedialAxis(image,granuloImage);
	unsigned int maxGranulo = 0;
	for (myLittleImage::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
		if (granuloImage(*it) > maxGranulo)
			maxGranulo = granuloImage(*it);
	unsigned int pas = 20;
	return buildHistogram(granuloImage, maxGranulo, pas, nbPoints);
}

/**
* Compute the distance between an image and a histogram. This function computes the histogram of the image and returns the distance between
* the two histograms
*
* @param imageFile The filename of the image
*
* @param histoFile The filename of the histogram
*
* @return The distance
*/
double distanceImageToHisto(string imageFile, string histoFile)
{
	vector<double> histo1 = imageToHistogram(imageFile);
	vector<double> histo2 = readHisto(histoFile);
	return EMD(histo1,histo2);
}

/**
* Compute the distance between two images. This function compute the histograms of the images and returns the distance between
* the two histograms
*
* @param imageFile1 The filename of the first image
*
* @param imageFile1 The filename of the second image
*
* @return The distance
*/
double distanceImageToImage(string imageFile1, string imageFile2)
{
	saveHistogramOfImage(imageFile2);
	return distanceImageToHisto(imageFile1,changeExtension(imageFile2));
}
