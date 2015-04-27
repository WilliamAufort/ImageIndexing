#ifndef Granulo_DEF
#define Granulo_DEF

#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/Color.h"
#include "DGtal/io/colormaps/HueShadeColorMap.h"
#include "DGtal/images/ImageSelector.h"
#include "DGtal/images/SimpleThresholdForegroundPredicate.h"
#include "DGtal/geometry/volumes/distance/DistanceTransformation.h"
#include "DGtal/shapes/implicit/ImplicitBall.h"

typedef DGtal::ImageSelector<DGtal::Z2i::Domain, unsigned int>::Type Image;

// Colormap used for the output
typedef DGtal::HueShadeColorMap<long int, 2> HueTwice;

// Point predicate
typedef DGtal::functors::SimpleThresholdForegroundPredicate<Image> PointPredicate;

// Distance transformation
typedef DGtal::DistanceTransformation<DGtal::Z2i::Space, PointPredicate, DGtal::Z2i::L2Metric> DTL2;

void buildHistogram(Image& granuloImage, unsigned int maxGranulo, unsigned int pas, unsigned int compteur, std::string fileName);
void saveGranulo(Image& granuloImage, unsigned int maxGranulo, std::string fileName);
std::string changeExtension(std::string fileName);
unsigned int buildGranulo(Image& image, Image& granuloImage);

#endif
