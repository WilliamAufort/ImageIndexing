#include "../include/distance.h"
#include "../include/granulometry.h"
#include "../include/toolbox.h"
#include <iostream>
#include <random>

using namespace std;
using namespace DGtal;
using namespace Z2i;

int main (int argc, char* argv[])
{
	if (argc != 2)
	{
		cerr << "Use: ./retrieval image.pgm" << endl;
		exit (1);
	}
	random_device rd;
    vector<string> fileList;
    GetFilesInDirectory(fileList, "histograms/database");
    vector<string> order = readFile(".classification.order");
    map<string, int> reverse;
    for(size_t i=0; i<order.size(); ++i)
        reverse[order[i]] = i;

    vector<double> histoTmp = imageToHistogram(argv[1]);


    vector<vector<double>> notations(70);
    vector<double> final(70);
    int tre = 0;
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
        cout << 1-(final[i]/sup) << endl;

	return 0;
}
