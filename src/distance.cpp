#include <iostream>
#include <fstream>
#include <cmath>
#include "../include/distance.h"

using namespace std;

vector<double> readHisto (std::string fileName)
{
	vector<double> histo;
	ifstream file(fileName.c_str());
	if (file.is_open())
	{
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
