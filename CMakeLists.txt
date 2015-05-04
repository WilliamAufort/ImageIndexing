PROJECT(ImageIndexing)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# We need Boost Program Options library to test
SET(Boost_USE_STATIC_LIBS   ON)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
SET(Boost_FOUND FALSE)
FIND_PACKAGE(Boost 1.46.0 REQUIRED COMPONENTS program_options)
if ( Boost_FOUND )
  message(STATUS "Boost and boost_program_options found.")
  include_directories( ${Boost_INCLUDE_DIRS} )
 endif( Boost_FOUND )

# DGtal requirement

FIND_PACKAGE(DGtal REQUIRED)
INCLUDE_DIRECTORIES(${DGTAL_INCLUDE_DIRS})
LINK_DIRECTORIES(${DGTAL_LIBRARY_DIRS})

SET(SRCs
  # tests
  imgRotate
  imgScale
  imgAddNoise
  getRank

  # our executables
  addNoise
  genHistogram
  testEfficiency
  indexing
  retrieval
  fastRetrieval
  graph)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

# Indexing

add_executable(indexing src/indexing src/granulometry src/toolbox)
target_link_libraries(indexing ${DGTAL_LIBRARIES})

# Retrieval

add_executable(retrieval src/retrieval src/distance src/toolbox src/granulometry src/transforms)
target_link_libraries(retrieval ${DGTAL_LIBRARIES})

# Fadt Retrieval

add_executable(fastRetrieval src/fastRetrieval src/distance src/toolbox src/granulometry src/transforms)
target_link_libraries(fastRetrieval ${DGTAL_LIBRARIES})

# genHistogram

add_executable(genHistogram src/genHistogram src/granulometry src/toolbox)
target_link_libraries(genHistogram ${DGTAL_LIBRARIES})

# testEfficiency

add_executable(testEfficiency src/testEfficiency src/granulometry)
target_link_libraries(testEfficiency ${DGTAL_LIBRARIES})

# addNoise

add_executable(addNoise src/addNoise src/toolbox)
target_link_libraries(addNoise ${DGTAL_LIBRARIES})

# graph

add_executable(graph src/graph src/toolbox src/distance src/granulometry src/transforms)
target_link_libraries(graph ${DGTAL_LIBRARIES})

#tests

add_executable(imgRotate imgRotate.cpp)
target_link_libraries (imgRotate ${DGTAL_LIBRARIES}  ${Boost_LIBRAIRIES}  ${Boost_PROGRAM_OPTIONS_LIBRARY})
add_executable(imgScale imgScale.cpp)
target_link_libraries (imgScale ${DGTAL_LIBRARIES}  ${Boost_LIBRAIRIES}  ${Boost_PROGRAM_OPTIONS_LIBRARY})
add_executable(imgAddNoise imgAddNoise.cpp)
target_link_libraries (imgAddNoise ${DGTAL_LIBRARIES}  ${Boost_LIBRAIRIES}  ${Boost_PROGRAM_OPTIONS_LIBRARY})
add_executable(getRank getRank.cpp)

