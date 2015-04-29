#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <regex>
#include "DGtal/io/readers/GenericReader.h"
#include "../include/granulometry.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

void calcul(string filename);
vector<string> liste;
mutex mtx;
size_t pos;

void GetFilesInDirectory(vector<string> &out, const string &directory)
{
    DIR *dir;
    struct dirent *ent;
    struct stat st;

    dir = opendir(directory.c_str());
    while ((ent = readdir(dir)) != NULL)
    {
        const string file_name = ent->d_name;
        const string full_file_name = directory + "/" + file_name;

        if (file_name[0] == '.')
            continue;

        if (stat(full_file_name.c_str(), &st) == -1)
            continue;

        const bool is_directory = (st.st_mode & S_IFDIR) != 0;

        if (is_directory)
            continue;

        out.push_back(full_file_name);
    }
    closedir(dir);
}

vector<string> filtre(const vector<string>& liste, const string& regexpr)
{
    regex e(regexpr);
    vector<string> out;

    for(string s : liste)
        if(regex_match(s, e))
            out.push_back(s);

    return out;
}

vector<string> difference(const vector<string>& liste, const vector<string>& done)
{
    vector<string> out;

    for(string s : liste)
    {
        bool test = false;
        for(string t : done)
            if(t == s)
            {
                test = true;
                break;
            }
        if(!test)
            out.push_back(s);
    }

    return out;
}

string changeExtension(string fileName)
{
	int lastIndex = fileName.find_last_of(".");
	string newFile = fileName.substr(0, lastIndex) + ".hist";
	return newFile;
}

void append(string& filename)
{
    ofstream outfile;
    outfile.open(".hist.done", ios_base::app);
    outfile << filename << endl;
    outfile.close();
}

vector<string> filename_done()
{
    ifstream infile;
    infile.open(".hist.done");
    string line;
    vector<string> out;

    while (getline(infile, line))
        out.push_back(line);
    infile.close();

    return out;
}

void threadWork(size_t id)
{
    while(pos < liste.size())
    {
        mtx.lock();
        string filename = liste[pos];
        cout << "id = " << id << "  pos = " << pos << "  filename = " << filename << endl;
        ++pos;
        mtx.unlock();
        calcul(filename);
        sleep(2);
        mtx.lock();
        append(filename);
        mtx.unlock();
    }
    cout << "Ay sir! Thread " << id << " is finished."<<endl;
}

void calcul(string filename)
{
    myLittleImage image = GenericReader<myLittleImage>::import(filename);

	PointPredicate predicate(image,0);
	DTL2 dtL2(image.domain(), predicate, l2Metric);
	/// Initialize

	myLittleImage granuloImage (image.domain());
	/// Granulometric function calculations

	unsigned int nbPoints = buildNaiveGranulo(image,granuloImage);
	/// Maximal value

	unsigned int maxGranulo = 0;
	for (myLittleImage::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
		if (granuloImage(*it) > maxGranulo)
			maxGranulo = granuloImage(*it);
	/// Histogramm

	unsigned int pas = 20;
	string fileName = "histograms/"+changeExtension(filename);
	buildHistogram(granuloImage, maxGranulo, pas, nbPoints, fileName);
}

int main(int argc, char* argv[])
{
	if (argc < 3 || argc > 4)
	{
		cerr << "Use: ./genHistogram folder nb_thread [regex]" << endl;
		exit (1);
	}

	GetFilesInDirectory(liste, argv[1]);

    cout<<liste.size()<<endl;

    if(argc == 4)
        liste = filtre(liste, argv[3]); ///.*bird.*pgm
    else
        liste = filtre(liste, ".*pgm");

    liste = difference(liste, filename_done());
    cout<<liste.size()<<endl;
    pos = 0;
    vector<thread*> th;
    size_t i;
    size_t NB_CPU = atol(argv[2]);

    size_t borne = (liste.size() < NB_CPU) ? liste.size() : NB_CPU;

    for(size_t i=0; i<borne; ++i)
        th.push_back(new thread(threadWork, i));
    for(size_t i=0; i<borne; ++i)
            th[i]->join();
    for(size_t i=0; i<borne; ++i)
            delete th[i];
	return 0;
}
