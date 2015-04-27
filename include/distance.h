#ifndef Distance_DEF
#define Distance_DEF

#include <vector>
#include <string>

std::vector<double> readHisto (std::string fileName);
double EMD(std::vector<double> histo1, std::vector<double> histo2);

#endif
