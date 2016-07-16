#  Configuration  #

CC = gcc
CFLAGS =\
  -g -Wall
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

RMOBJ :=\
	$(EXEC) $(OBJS)\
	$(ARDO) $(test1o) $(test1)\
	$(test2) $(test2o)

.PHONY: all
all: 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS) $(INC_PATH)

$(test1): $(test1o) $(ARDO)
	$(CC) $(CFLAGS) -o $@ $(test1o) $(ARDO)

$(test2): $(test2cpp)
	$(CPP) $(CXX_FLAGS) $< -o $@ $(LDFLAGS) $(INC_PATH)

clean:
	$(RM) -f $(RMOBJ)
