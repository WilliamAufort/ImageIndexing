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
#include "../include/toolbox.h"


using namespace std;
using namespace DGtal;
using namespace Z2i;

void calcul(string filename);
vector<string> liste;
mutex mtx;
size_t pos;

/**
* Append a line in .hist.done
*
* @param str A string to append to .hist.done
*/
void append(string& str)
{
    ofstream outfile;
    outfile.open(".hist.done", ios_base::app);
    outfile << str << endl;
    outfile.close();
}

/**
* Read .hist.done
*
* @return The list of all line of .hist.done
*/
vector<string> filename_done()
{
    return readFile(".hist.done");;
}

/**
* What a thread have to do. They compute histograms of all images in liste. Each thread takes a filename in liste, does the computation and continues...
* When there is no more untreated file in liste, the thread finishes. We use mutex to avoid race condition.
*
* @param id The id of the thread
*/
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

/**
* Compute the histogram of an image
*
* @param filename The image whose we want to compute the histogram
*/
void calcul(string filename)
{
    myLittleImage image = GenericReader<myLittleImage>::import(filename);
	myLittleImage granuloImage (image.domain());

	PointPredicate predicate(image,0);
	DTL2 dtL2(image.domain(), predicate, l2Metric);
	unsigned int nbPoints = buildNaiveGranulo(image,granuloImage);
	unsigned int maxGranulo = 0;
	for (myLittleImage::Domain::ConstIterator it = granuloImage.domain().begin(); it != granuloImage.domain().end(); ++it)
		if (granuloImage(*it) > maxGranulo)
			maxGranulo = granuloImage(*it);
	unsigned int pas = 20;
	string fileName = "histograms/"+changeExtension(filename);
	buildHistogram(granuloImage, maxGranulo, pas, nbPoints, fileName);
}

/**
* Generate all histograms of a set of images optionally matching a regex
*
* @param folder The folder containing the images
*
* @param nb_thread The number of thread
*
* @param regex An optional argument. All file treated have to match the regex
*/
int main(int argc, char* argv[])
{
	if (argc < 3 || argc > 4)
	{
		cerr << "Use: ./genHistogram folder nb_thread [regex]" << endl;
		exit (1);
	}

	liste = GetFilesInDirectory(argv[1]);

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
