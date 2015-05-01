#pragma once

#include <vector>
#include <string>

std::vector<double> readHisto (std::string fileName);
double EMD(std::vector<double> histo1, std::vector<double> histo2);
void saveHistogramOfImage(std::string filename);
double distanceImageToHisto(std::string imageFile, std::string histoFile);
double distanceImageToImage(std::string imageFile1, std::string imageFile2);
std::string changeExtension(std::string fileName);
std::vector<double> imageToHistogram(std::string filename, bool smooth = false);
