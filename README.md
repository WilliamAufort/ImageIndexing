# ImageIndexing

A M1 mini-project to create an image indexing algorithm.

This project is esentially based on DGtal, a generic open source library for Digital Geometry programming.

# Install DGtal on your machine

You will need development packages for boost and boost-programs_option. Then, get the code and compile the library:

	git clone https://github.com/DGtal-team/DGtal.git
	cd DGtal && mkdir build && cd build
	cmake .. -DDGTAL_BUILD_TESTING=OFF
	make

# Compile the project

Get the code of the project and compile it using cmake :

	git clone https://github.com/WilliamAufort/ImageIndexing.git
	cd ImageIndexing && mkdir build & cd build
	cmake .. -DDGtal_DIR=folder
	make

where "folder" is your DGtal build folder.

# Execute

In the build folder, you will find the different executables. Run them with the classical way, for example

	./granulometry arguments

To know how the executables work, just run them without arguments.
