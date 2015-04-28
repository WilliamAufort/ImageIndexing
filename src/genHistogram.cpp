#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<regex>
#include "DGtal/base/Common.h"
#include "DGtal/io/readers/GenericReader.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/Color.h"
#include "DGtal/io/colormaps/HueShadeColorMap.h"
#include "DGtal/images/ImageSelector.h"
#include "DGtal/images/SimpleThresholdForegroundPredicate.h"
#include "DGtal/geometry/volumes/distance/DistanceTransformation.h"
#include "DGtal/shapes/implicit/ImplicitBall.h"
#include "DGtal/io/boards/Board2D.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

typedef ImageSelector<Domain, unsigned int>::Type Image;

// Colormap used for the output
typedef HueShadeColorMap<long int, 2> HueTwice;

// Point predicate
typedef functors::SimpleThresholdForegroundPredicate<Image> PointPredicate;

// Distance transformation
typedef DistanceTransformation<Space, PointPredicate, L2Metric> DTL2;

//////////////////////////////////////////////////////////////////////
/* Returns a list of files in a directory (except the ones that begin with a dot) */

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
} // GetFilesInDirectory

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


void buildHistogram(Image& granuloImage, unsigned int maxGranulo, unsigned int pas, unsigned int compteur, string fileName)
{
	vector<double> histo(pas+1,0.0);
	double cast_max = static_cast<double>(maxGranulo);
	double cast_pas = static_cast<double>(pas);
	double cast_compteur = static_cast<double>(compteur);
	for (Image::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
	{
		if (granuloImage.domain().isInside(*it)) // inside the image
		{
			unsigned int normalizedValue = static_cast<unsigned int>(static_cast<double>(granuloImage(*it)) / cast_max * cast_pas);
			histo[normalizedValue]++;
		}
	}
	ofstream file(fileName.c_str());
    if (file.is_open())
	{
		// for (unsigned int i = 0; i <= pas; ++i)
		// it seems that the points that doesn't belong to the object are considering in
		// DT are in fact in the histogram... Just not consider them before close the problem
		// Another advantage of this is that we use a kind a filter of "bad" balls which can
		// appear in the border
		for (unsigned int i= 1; i <= pas; ++i)
		{
			histo[i] /= cast_compteur;
			file << i << " " << histo[i] << endl;
		}
	}
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
        cout << "id = " << id << "  pos = " << pos << "  filename = " << liste[pos] << endl;
        ++pos;
        string filename = liste[pos-1];
        mtx.unlock();
        calcul(filename);
        sleep(30);
    }
}

void calcul(string filename)
{
    Image image = GenericReader<Image>::import(filename);

	PointPredicate predicate(image,0);
	DTL2 dtL2(image.domain(), predicate, l2Metric);

	Image granuloImage (image.domain());
	for (Image::Range::Iterator it = granuloImage.range().begin(); it != granuloImage.range().end(); ++it)
		*it = 0;

	unsigned int compteur = 0; // number of points in the image
	{
	for (Image::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
		if (dtL2(*it) > 0) // inside the image
		{
			compteur++;
			// Build a sphere inside the object of radius dtL2(*it)
			unsigned int radius = static_cast<unsigned int>(dtL2(*it));
			RealPoint center = *it;
			ImplicitBall<Space> ball(center,radius);
			// To iterate on the sphere, we build a tangent bounding box around it and iterate into the box
			Point top = center + Point::diagonal(radius +1);
			Point bottom = center - Point::diagonal(radius +1);
			Domain sphereDomain(bottom,top);
			for (Domain::ConstIterator it = sphereDomain.begin(); it != sphereDomain.end(); ++it)
			{
				if ((granuloImage.domain().isInside(*it)) // Point inside the image
					&& (ball(*it) > 0)					  // Point inside the ball
					&& (granuloImage(*it) < radius)) 	  // Granulometric value has to be updated
				{
					granuloImage.setValue(*it,radius);
				}
			}
		}
	}

	unsigned int maxGranulo = 0;
	for (Image::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
		if (granuloImage(*it) > maxGranulo)
			maxGranulo = granuloImage(*it);

	unsigned int pas = 20;
	string fileName = "histograms/"+changeExtension(filename);
	buildHistogram(granuloImage, maxGranulo, pas, compteur, fileName);
	append(filename);
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cerr << "Use: ./granulometry folder nb_thread" << endl;
		exit (1);
	}

	GetFilesInDirectory(liste, argv[1]);
 /**
    for(string s : liste)
        cout<<s<<" ";
    cout<<endl;
 **/
    cout<<liste.size()<<endl;
    liste = filtre(liste, ".*pgm"); ///.*bird.*pgm
    liste = difference(liste, filename_done());
    cout<<liste.size()<<endl;
 /**
    for(string s : liste)
        cout<<s<<" ";
    cout<<endl;
 **/
    pos = 0;
    vector<thread*> th;
    size_t i;
    size_t NB_CPU = atol(argv[2]);

    for(size_t i=0; i<NB_CPU; ++i)
        th.push_back(new thread(threadWork, i));
    for(size_t i=0; i<NB_CPU; ++i)
            th[i]->join();
    for(size_t i=0; i<NB_CPU; ++i)
            delete th[i];

/**
    for(i = 0; i < liste.size()/NB_CPU; i+=NB_CPU)
    {
        th.clear();
        for(int j=0;j<NB_CPU;++j)
            th.push_back(new thread(calcul, liste[NB_CPU*i+j]));
        for(int j=0;j<NB_CPU;++j)
            th[j]->join();
        for(int j=0;j<NB_CPU;++j)
            delete th[j];
    }
    th.clear();
    for(size_t j = i; j<liste.size();++j)
        th.push_back(new thread(calcul, liste[j]));
    for(size_t j = i; j<liste.size();++j)
        th[j-i]->join();
    for(size_t j = i; j<liste.size();++j)
        delete th[j-i];
**/
	return 0;
}
