#include "../include/distance.h"
#include "../include/granulometry.h"
#include "../include/toolbox.h"
#include <iostream>
#include <random>

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
/**
* Build the list of score for a given image
*
* @param fileList A vector of strings containing the filenames of histograms with which we want to compute the score of imageFileName
*
* @param imageFileName The filename of the image in which we are interested
*
* @return A vector of double containing the score for each class in the order of .classification.order. 1 is similar, 0 is different.
*/
vector<double> buildScore(const vector<string>& fileList, const string& imageFileName)
{
    vector<string> order = readFile("../.classification.order");
    map<string, int> reverse;
    for(size_t i=0; i<order.size(); ++i)
        reverse[order[i]] = i;

    vector<vector<double>> notations(70);
    vector<double> final(70);
    int tre = 0;

    vector<double> histoTmp = imageToHistogram(imageFileName, true);
    for(string s : fileList)
    {
        vector<double> histo = readHisto(s);
        notations[reverse[extractClass(getFileName(s))]].push_back(EMD(histoTmp, histo));
    }

    double sup = 0;
    for(size_t i=0;i<70;++i)
    {
        double inf = 1000000;
        for(size_t j=0;j<15;++j)
            if(notations[i][j] < inf)
                inf = notations[i][j];
        final[i] = inf;
        if(sup < inf)
            sup = inf;
    }

    for(size_t i=0; i<70; ++i)
        final[i] = 1-(final[i]/sup);

    return final;
}

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
	if (argc != 2)
	{
		cerr << "Use: ./fastRetrieval image.pgm" << endl;
		exit (1);
	}
	scaling(argv[1],"tmp_scaled.pgm");
	random_device rd;
    vector<string> fileList = GetFilesInDirectory("../histograms/database");
	
    vector<double> final = buildScore(fileList, "tmp_scaled.pgm");

    for(size_t i=0; i<70; ++i)
        cout << final[i] << endl;
    
	return 0;
}
