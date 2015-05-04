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
	cd ImageIndexing && mkdir build && cd build
	cmake .. -DDGtal_DIR=folder
	make
	mv ../scriptClassification.sh .

where "folder" is your DGtal build folder.

# Run the tests

Once you have done the previous commands, in the build directory, you have a script to test the classification on random examples. To run it, just use

	bash scriptClassification.sh

Be careful: don't use chmod or something loke that. For unknown reasons, there is a weird behaviour of $RANDOM which doesn't give a random number at each time anymore. Also, make sure you have moved the script in the build directory, otherwise you will have problems when trying to run it, because of relative paths of executables mostly.

# Execute

After compiling the project, in the build folder you will find the different executables.

More details about each of them.

	./indexing image.pgm

returns a vector of scalar quantities which describes the shape. Here, we just compute the distribution of the granulometric values on the shape.

	./retrieval image.pgm

outputs consecutively on the console the marks of each class with the image input, in the same order as in the file .classification.order

	./genHistograms <folder> <nb_thread> [<regex>]
	
generated all the histograms needed to do the shape retrieval. The argument <folder> is the folder containging the .pgm you want to analyse. <nb_thread> is the number of threads launched.
The argument <regex> is optional. When it is given, the histograms are generated only for files whose name matches the regular expression. If there is none, the default regex ".*pgm" is used. For instance:

	./genHistograms database 8 

computes the histograms of all pgm in database with 8 threads, or
	
	./genHistograms database 8 ".*snake.*pgm"

computes the histograms of the pgm in database whose filename matches ".*snake.*pgm" on 8 threads.
Be careful! This executable is too much long (very long even with parallelization, about 100h sequentially). That's why you can use directly the histograms in the directory histograms/database.

	./testEfficiency image.pgm

runs the two algorithms we discuss in the report to compute the granulometric values, and outputs their execution time, in order to compare them.

#Documents

We can also find in the folder "docs" the subject of the project and our report about the project, required by our school. You can compile this last one using simply:

	make
	make

(The second one is necessary to link refs and labels in the document.
