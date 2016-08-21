EXEC = fetcher.so
all: $(EXEC)

CC ?= g++
CXXFLAGS =   
INCLUDE = -I /usr/include/python2.7/ -I /usr/local/opencv2.4/include/
LDFLAGS = -L $libboot -lboost_python-py27 \
             -lopencv_core -lopencv_highgui -lopencv_imgproc

$(EXEC): Fetcher.cpp opencv-ndarray-conversion/conversion.cpp
	$(CC) $(CXXFLAGS) $(INCLUDE) -o $@ $^ $(LDFLAGS) -shared -fPIC

install:
	cp fetcher.so /usr/local/lib/python2.7/dist-packages/

clean:
	$(RM) $(EXEC) 
