PROJECT(ImageIndexing)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

FIND_PACKAGE(DGtal REQUIRED)
INCLUDE_DIRECTORIES(${DGTAL_INCLUDE_DIRS})
LINK_DIRECTORIES(${DGTAL_LIBRARY_DIRS})

SET(SRCs
  helloworld
  granulometry
  genHistogram
  testEfficency)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

# Indexing

add_executable(indexing src/indexing src/granulometry)
target_link_libraries(indexing ${DGTAL_LIBRARIES})

# Retrieval

add_executable(retrieval src/retrieval src/distance src/granulometry src/transforms)
target_link_libraries(retrieval ${DGTAL_LIBRARIES})

# genHistogram

add_executable(genHistogram src/genHistogram src/granulometry)
target_link_libraries(genHistogram ${DGTAL_LIBRARIES})

# testEfficiency

add_executable(testEfficiency src/testEfficiency src/granulometry)
target_link_libraries(testEfficiency ${DGTAL_LIBRARIES})


