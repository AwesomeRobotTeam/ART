#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "term2Arduino.h"

int main( int argc, char* argv[])
{
  int fd;
  char bridge[20];
  if( argc == 1) strcpy( bridge, defaultBridge);
  else strcpy( bridge, argv[1]);


  speed_t baudrate = B9600;
  p_termios option = (p_termios) malloc( sizeof( struct termios));
  fd = shake_hand( bridge, baudrate, option);

	int rdbyte;

  char* ws = "Hello_Arduino";
  printf("I say Hello to u, strlen(ws) = %d\n", strlen(ws));
  host_write( fd, ws, strlen( ws)); //+ 1);

  /*char rbuf[20];
  while( rdbyte = host_read( fd, rbuf, 20) )
	{
		if( rdbyte == -1)
		{
			printf("Read Nothing\n");
			sleep(1);
		}
		else
		{
			rbuf[rdbyte+1] = '\0';
  		printf("host_read: %d byte : %s\n", rdbyte, rbuf);
		}
		memset( rbuf, 0, 20);
	}

	rbuf[rdbyte+1] = '\0';
  printf("host_read: %d byte : %s\n", rdbyte, rbuf);*/
	return 0;
}
