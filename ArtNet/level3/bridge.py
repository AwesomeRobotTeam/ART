import serial
import time
import tty, termios, sys

# Returns a single character from standard input
def getchar():     
   fd = sys.stdin.fileno()
   old_settings = termios.tcgetattr(fd)
   try:
      tty.setraw(sys.stdin.fileno())
      ch = sys.stdin.read(1)
   finally:
      termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
   return ch

# initialize the stepper and locate the 4 corner
def init():
    i = 1
    while i <= 4: 
        ch = getchar()
        if ch == '\x1b' and '[' == getchar():
            ch = getchar() 
            if ch == 'A':
                 print 'up'
            elif ch == 'B':
                print 'down'
            elif ch == 'C':
                print 'right'
            elif ch == 'D':
                print 'left'
        elif ch == ' ':
            print 'set %d\'s corner' % i
            #TODO transmit the message to arduino
            i = i + 1
        elif ch == 'q':
            return
        else:
            continue
    print 'All corners are set'
    #TODO transmit the message to arduino

# configure the serial connection
def connect(port='/dev/ttyACM0',baudrate=115200):
    ser = serial.Serial(port, baudrate,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.SEVENBITS)
    ser.isOpen()
    print 'connection start'
    init()   
    return ser

# send message to laser gun
def sendmsg(ser, message):
    time.sleep(1)
    msg = ''.join(message)
    ser.write(msg)
    time.sleep(1)
    msg2 = ser.read(len(msg)+1)
    print 'return : '+ msg

# close connection
def close(ser):
    ser.close()
