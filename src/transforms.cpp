#include "../include/transforms.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

/***************************************\
| Implementation of a simple smoothing 	|
| filter whose goal is to reduce 		|
| specular noise on an image 			|
\***************************************/

/*

The chosen filter is the binomial filter :

	--		  --
	| 1  2  1  |
	| 2  4  2  |
	| 1  2  1  |
	--        --

which seems sufficient for the work we are doing with it.

*/


// Return 1 if belongs P belongs to the object, 0 otherwise (not on the object or out of the border)

unsigned int getValue(myLittleImage& image, Point p)
{
	if (image.domain().isInside(p))
		return (image(p) > 0);
	return 0;
}

// Apply the filter on the image

void applyFilter(myLittleImage& image, myLittleImage& filteredImage)
{	
	for (myLittleImage::Domain::ConstIterator it = image.domain().begin(); it != image.domain().end(); ++it)
	{
		Point ref = *it;
		// the filter
		int value = getValue(image,ref+Point(-1,1)) + 2*getValue(image,ref+Point(0,1)) + getValue(image,ref+Point(1,1))
					+ 2*getValue(image,ref+Point(-1,0)) + 4*getValue(image,ref) + 2*getValue(image,ref+Point(1,0))
					+ getValue(image,ref+Point(-1,-1)) + 2*getValue(image,ref+Point(0,-1)) + getValue(image,ref+Point(1,-1));
		if (value >= 8)
			filteredImage.setValue(ref,255);
		else
			filteredImage.setValue(ref,0);
	}
}

