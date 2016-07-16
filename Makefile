#  Configuration  #

CC = gcc
CFLAGS =\
  -g -Wall
LDFLAGS=

# YOU SHOULD CHECK THIS #
INC_PATH= -I include

ARDO := include/term2Arduino.o

test1 := arduino_termio_test
test1o:= test/arduino_termio_test.o

RMOBJ :=\
	$(EXEC) $(OBJS)\
	$(ARDO) $(test1o) $(test1)

.PHONY: all
all: 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS) $(INC_PATH)

$(test1): $(test1o) $(ARDO)
	$(CC) $(CFLAGS) -o $@ $(test1o) $(ARDO)

clean:
	$(RM) -f $(RMOBJ)
