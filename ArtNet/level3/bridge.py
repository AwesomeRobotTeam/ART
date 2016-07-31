import serial

# configure the serial connection
def connect(port='/dev/ttyUSB1',baudrate=9600):
    ser = serial.Serial(port, baudrate,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.SEVENBITS)
    ser.isOpen()
    print 'connection start'   
    return ser

# send message to laser gun
def sendmsg(ser, message):
    ser.write(message)

# close connection
def close(ser):
    ser.close()
