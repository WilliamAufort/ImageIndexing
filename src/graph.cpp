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
	if (argc != 4)
	{
		cerr << "Use: ./graph database class histograms/databse" << endl;
		exit (1);
	}
    random_device rd;
    vector<string> fileList;
    GetFilesInDirectory(fileList, argv[1]);
    vector<string> fileListClass = filtre(fileList, string(argv[1])+"/"+string(argv[2])+"-.*\\.pgm");
    string choosenFile = fileListClass[rd()%fileListClass.size()];
    vector<string> order = readFile(".classification.order");
    map<string, int> reverse;
    for(size_t i=0; i<order.size(); ++i)
        reverse[order[i]] = i;

    cout<<choosenFile<<" "<<getFileName(choosenFile)<<" "<<extractClass(getFileName(choosenFile))<<" "<<reverse[extractClass(getFileName(choosenFile))]<<endl;

    fileList.clear();
    modifyImage(choosenFile, "temp.pgm");
    GetFilesInDirectory(fileList, argv[3]);
    imageToHistogram("temp.pgm");

	vector<double> histoTmp = readHisto(changeExtension("temp.pgm"));


    vector<vector<double>> notations(70);
    int tre = 0;
    for(string s : fileList)
    {
        vector<double> histo = readHisto(s);
        notations[reverse[extractClass(getFileName(s))]].push_back(EMD(histoTmp, histo));
    }

    ofstream file("plot/plot.dat");

    file<<"class ";
    for(size_t i=0;i<15;++i)
        file<<i<<" ";
    file<<endl;


    for(size_t i=0;i<70;++i)
    {
        file << i <<" ";
        for(size_t j=0;j<15;++j)
            file<<notations[i][j]<<" ";
        file<<endl;
    }

    file.close();

    system("gnuplot -persist plot/plot.p");

	return 0;
}
