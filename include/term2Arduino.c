#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "term2Arduino.h"

//#define fdn int


int shake_hand( char* bridge, speed_t baudrate, p_termios t_option)
{
  int fd;
	//O_NOCTTY => TTY: O_NONBLOCK => NBLK
  fd = open( bridge, O_RDWR | O_NOCTTY); //| O_NONBLOCK);
  
  if( fd < 0){ perror("shake hand fault\n"); return -1;}
  else{ printf("Shake Hand Success!\n");}

  //usleep( 350000);

  //get current serial port setting
  tcgetattr( fd, t_option);
  
  //set I/O baudrate
  cfsetispeed( t_option, baudrate);
  cfsetospeed( t_option, baudrate);

  //8-bit, no parity, no stop bit
  //TODO:survey how to get more
  /*t_option->c_cflag &= ~PARENB;
  t_option->c_cflag &= ~CSTOPB;
  t_option->c_cflag &= ~CSIZE;
  t_option->c_cflag |= CS8;
  t_option->c_cflag |= ICANON;//Canonical mode
*/

	/*SET FLAG*/
	//CLOCAL => ignore mistake: CREAD => enable Read
	t_option->c_cflag = baudrate|CS8|CLOCAL|CREAD;
	t_option->c_iflag = IGNPAR;
	t_option->c_oflag = 0;
	t_option->c_lflag = 0;
	t_option->c_cc[VTIME] = 0;
	t_option->c_cc[VMIN] = 1;

  //commit the serial port setting
  tcsetattr( fd, TCSANOW, t_option);
	
	//reset
	bzero( &t_option, sizeof(t_option));

  return fd;
}

int host_read( int fd, char* rbuf, int len)
{
  int n = read( fd, rbuf, len);
  rbuf[n] = 0;
  return n;
}

void host_write( int fd, char* wbuf, int len)
{
  write( fd, wbuf, len);
}
