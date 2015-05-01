#pragma once

void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory);
std::vector<std::string> filtre(const std::vector<std::string>& liste, const std::string& regexpr);
std::vector<std::string> difference(const std::vector<std::string>& liste, const std::vector<std::string>& done);
std::vector<std::string> readFile(std::string filename);
