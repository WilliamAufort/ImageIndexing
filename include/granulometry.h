#ifndef Granulo_DEF
#define Granulo_DEF

#include <functional>
#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/Color.h"
#include "DGtal/io/colormaps/HueShadeColorMap.h"
#include "DGtal/images/ImageSelector.h"
#include "DGtal/images/SimpleThresholdForegroundPredicate.h"
#include "DGtal/geometry/volumes/distance/DistanceTransformation.h"
#include "DGtal/shapes/implicit/ImplicitBall.h"
#include "DGtal/geometry/volumes/distance/PowerMap.h"
#include "DGtal/geometry/volumes/distance/ReducedMedialAxis.h"
#include "DGtal/geometry/volumes/distance/ExactPredicateLpPowerSeparableMetric.h"

typedef DGtal::ImageSelector<DGtal::Z2i::Domain, unsigned int>::Type myLittleImage;
typedef DGtal::HueShadeColorMap<long int, 2> HueTwice;
typedef DGtal::functors::SimpleThresholdForegroundPredicate<myLittleImage> PointPredicate;
typedef DGtal::DistanceTransformation<DGtal::Z2i::Space, PointPredicate, DGtal::Z2i::L2Metric> DTL2;
typedef DGtal::PowerMap<DTL2, DGtal::Z2i::L2PowerMetric> Map;
typedef DGtal::ReducedMedialAxis<Map> RMA;

unsigned int buildNaiveGranulo(myLittleImage& image, myLittleImage& granuloImage);
unsigned int granuloWithMedialAxis(myLittleImage& image, myLittleImage& granuloImage);
void saveGranulo(myLittleImage& granuloImage, unsigned int maxGranulo, std::string fileName);
void buildHistogram(myLittleImage& granuloImage, unsigned int maxGranulo, unsigned int pas, unsigned int compteur, std::string fileName);

/// Test efficiency of the two algorithms

void testSpeed(std::function<unsigned int(myLittleImage&, myLittleImage&)> &granulo, myLittleImage& image, const char* inputFile);
void testSpeedNaive(myLittleImage& image, const char* inputFile);
void testSpeedQuick(myLittleImage& image, const char* inputFile);

#endif
