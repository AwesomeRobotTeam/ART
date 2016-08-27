import serial
import time
# configure the serial connection
def connect(port='/dev/ttyACM0',baudrate=115200):
    ser = serial.Serial(port, baudrate,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.SEVENBITS)
    ser.isOpen()
    print 'connection start'   
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
