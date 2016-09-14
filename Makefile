#  Configuration  #

CC = g++
CFLAGS =\
  -g -Wall -ansi -std=c++11
LDFLAGS=\
	-lopencv_core -lopencv_highgui -lopencv_imgproc

CPP = g++
CXX_FLAGS=\
	-ansi -g -Wall -std=c++11

# YOU SHOULD CHECK THIS #
INC_PATH= -I include

ARDO := include/term2Arduino.o

test1 := arduino_termio_test
test1o:= test/arduino_termio_test.o

test2 := robot_navigation_via_color_differentiation
test2cpp:= test/navigation_test.cpp

test3 := demo_navigation
test3cpp:= test/demo_navigation.cpp

test4 := detect_trafficLight
test4cpp:= test/traffic_light_test.cpp

test5 := image_transform
test5cpp := test/image_transform.cpp

#test6 := Fetcher_v2
#test6cpp := test/Fetcher_v2.cpp

RMOBJ :=\
	$(ARDO) $(test1o) $(test1)\
	$(test2) $(test2o) \
	$(test3) $(test4) $(test5) $(test6) \

.PHONY: all
all: 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS) $(INC_PATH)

$(test1): $(test1o) $(ARDO)
	$(CC) $(CFLAGS) -o $@ $(test1o) $(ARDO) 

$(test2): $(test2cpp)
	$(CPP) $(CXX_FLAGS) $< -o $@ $(LDFLAGS) $(INC_PATH)

$(test3): $(test3cpp) $(ARDO)
	$(CPP) $(CXX_FLAGS) $< -o $@ $(LDFLAGS) $(INC_PATH)

$(test4): $(test4cpp)
	$(CPP) $(CXX_FLAGS) $< -o $@ $(LDFLAGS) $(INC_PATH)

$(test5): $(test5cpp)
	$(CPP) $(CXX_FLAGS) $< -o $@ $(LDFLAGS) $(INC_PATH)

#$(test6): $(test6cpp)
#	$(CPP) $(CXX_FLAGS) $< -o $@ $(LDFLAGS) $(INC_PATH)

clean:
	$(RM) -f $(RMOBJ)
