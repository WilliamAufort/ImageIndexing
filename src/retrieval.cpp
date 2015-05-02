#include "../include/distance.h"
#include "../include/granulometry.h"
#include "../include/toolbox.h"
#include <iostream>
#include <random>

using namespace std;
using namespace DGtal;
using namespace Z2i;
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

int main (int argc, char* argv[])
{
	if (argc != 2)
	{
		cerr << "Use: ./retrieval image.pgm" << endl;
		exit (1);
	}
	random_device rd;
    vector<string> fileList = GetFilesInDirectory("../histograms/database");
    vector<double> final = buildScore(fileList, argv[1]);

    for(size_t i=0; i<70; ++i)
        cout << final[i] << endl;

	return 0;
}
