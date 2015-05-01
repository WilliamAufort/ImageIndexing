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
	if (argc != 3)
	{
		cerr << "Use: ./graph database class histograms/databse" << endl;
		exit (1);
	}
    random_device rd;
    vector<string> fileList;
    GetFilesInDirectory(fileList, argv[1]);
    vector<string> fileListClass = filtre(fileList, string(argv[1])+"/"+string(argv[2])+"-.*\\.pgm");
    string choosenFile = fileListClass[rd()%fileListClass.size()];

    modifyImage(choosenFile, "tmp.pmg");

    GetFilesInDirectory(fileList, argv[3]);

    imageToHistogram("tmp.pmg");
	vector<double> histoTmp = readHisto(changeExtension("tmp.pmg"));

    map<string, vector<double>> notations;
    for(string s : fileList)
    {
        vector<double> histo = readHisto(histoFile);
        notations[extractClass(s)].push_back(EMD(histoTmp,histo));
    }


	return 0;
}
