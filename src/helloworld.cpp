#include <DGtal/base/Common.h>
#include "../include/toolbox.h"

int main(int argc, char* argv[])
{
	DGtal::trace.info() << "Helloworld from DGtal ";
	DGtal::trace.emphase() << "(version "<< DGTAL_VERSION << ")"<< std::endl;
	std::string filename = "database/tree-1.pgm";
	DGtal::trace.info() << extractClass(filename) << std::endl;
	std::string histoname = "histograms/database/device3-10.hist";
	DGtal::trace.info() << extractClass(histoname) << std::endl;

	// test modifyImage
	modifyImage("apple-1.pgm","test.pgm");
	return 0;
}
