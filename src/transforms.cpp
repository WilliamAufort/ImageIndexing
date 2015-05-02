#include "../include/transforms.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

/**
* Decide whether a point belongs to an object or not 
*
* @param image The image containing the object
*
* @param P a point
*
* @return 1 if P belongs to the object, 0 otherwise
*/
unsigned int getValue(myLittleImage& image, Point p)
{
	if (image.domain().isInside(p))
		return (image(p) > 0);
	return 0;
}

/**
* Apply the filter:
*
*	--		  --
*	| 1  2  1  |
*	| 2  4  2  |
*	| 1  2  1  | 
*	--        --
*
* on the image to delete the noise
*
* @param image The input image
*
* @param filteredImage The image where we have to store the filtered image
*/
void applyFilter(myLittleImage& image, myLittleImage& filteredImage)
{	
	for (myLittleImage::Domain::ConstIterator it = image.domain().begin(); it != image.domain().end(); ++it)
	{
		Point ref = *it;
		int value = getValue(image,ref+Point(-1,1)) + 2*getValue(image,ref+Point(0,1)) + getValue(image,ref+Point(1,1))
					+ 2*getValue(image,ref+Point(-1,0)) + 4*getValue(image,ref) + 2*getValue(image,ref+Point(1,0))
					+ getValue(image,ref+Point(-1,-1)) + 2*getValue(image,ref+Point(0,-1)) + getValue(image,ref+Point(1,-1));
		if (value >= 8)
			filteredImage.setValue(ref,255);
		else
			filteredImage.setValue(ref,0);
	}
}

