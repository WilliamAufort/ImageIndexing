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

string changeExtension(string fileName)
{
	int lastIndex = fileName.find_last_of(".");
	string newFile = fileName.substr(0, lastIndex) + ".hist";
	return newFile;
}

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

double distanceImageToHisto(string imageFile, string histoFile)
{
	vector<double> histo1 = imageToHistogram(imageFile);
	vector<double> histo2 = readHisto(histoFile);
	return EMD(histo1,histo2);
}

double distanceImageToImage(string imageFile1, string imageFile2)
{
	saveHistogramOfImage(imageFile2);
	return distanceImageToHisto(imageFile1,changeExtension(imageFile2));
}
