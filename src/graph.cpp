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
    vector<string> fileList = GetFilesInDirectory(argv[1]);
    vector<string> fileListClass = filtre(fileList, string(argv[1])+"/"+string(argv[2])+"-.*\\.pgm");
    string choosenFile = fileListClass[rd()%fileListClass.size()];
    vector<string> order = readFile(".classification.order");
    map<string, int> reverse;
    for(size_t i=0; i<order.size(); ++i)
        reverse[order[i]] = i;

    cout<<choosenFile<<" "<<getFileName(choosenFile)<<" "<<extractClass(getFileName(choosenFile))<<" "<<reverse[extractClass(getFileName(choosenFile))]<<endl;

    modifyImage(choosenFile, "temp.pgm");
    fileList = GetFilesInDirectory(argv[3]);
    vector<double> histoTmp = imageToHistogram("temp.pgm", true);


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
    file<<"min avg fancy1 fancy2 AVG";
    file<<endl;


    for(size_t i=0;i<70;++i)
    {
        file << i <<" ";
        double inf = 1000000;
        double avg = 0;
        double fancy1 = 0;
        double fancy2 = 0;
        for(size_t j=0;j<15;++j)
        {
            file<<notations[i][j]<<" ";
            if(notations[i][j] < inf)
                inf = notations[i][j];
            avg += notations[i][j];
            fancy1 += sqrt(notations[i][j]);
            fancy2 += pow(notations[i][j]/2,2);
        }
        file<<inf<<" "<<avg/15<<" "<<fancy1/15<<" "<<fancy2/25<<" "<<((fancy2/25)+fancy1+(avg/15)+inf)/4;
        file<<endl;
    }



    file.close();
    system("gnuplot -persist plot/plot.p");

	return 0;
}
