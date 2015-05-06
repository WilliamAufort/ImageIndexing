#pragma once

std::string changeExtension(std::string fileName);
std::vector<std::string> GetFilesInDirectory(const std::string &directory);
std::vector<std::string> filtre(const std::vector<std::string>& liste, const std::string& regexpr);
std::vector<std::string> difference(const std::vector<std::string>& liste, const std::vector<std::string>& done);
std::vector<std::string> readFile(std::string filename);
std::string extractClass(std::string filename);
std::string getPath(const std::string& str);
std::string getFileName(const std::string& str);
void modifyImage(std::string inputFile, std::string outputFile, double noise = 0.5);
