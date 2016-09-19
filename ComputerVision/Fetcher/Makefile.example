all:
	export libboost=/usr/lib/arm-linux-gnueabihf
	g++ Fetcher.cpp opencv-ndarray-conversion0/conversion.cpp -I /usr/include/python2.7/ -L $libboost -lboost_python-py27 -lopencv_core -lopencv_highgui -lopencv_imgproc -o fetcher.so -shared -fPIC
