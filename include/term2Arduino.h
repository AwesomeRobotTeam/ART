#define defaultBridge "/dev/ttyACM0"

typedef struct termios *p_termios;

int shake_hand( char* bridge, speed_t baudrate, p_termios t_option);

int host_read( int fd, char* rbuf, int len);

void host_write( int fd, char* rbuf, int len);
