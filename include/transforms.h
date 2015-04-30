#ifndef Transforms_DEF
#define Transforms_DEF

#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/images/ImageSelector.h"

typedef DGtal::ImageSelector<DGtal::Z2i::Domain, unsigned char>::Type myLittleImage;

unsigned int getValue(myLittleImage& image, DGtal::Z2i::Point p);
void applyFilter(myLittleImage& image, myLittleImage& filteredImage);

#endif
