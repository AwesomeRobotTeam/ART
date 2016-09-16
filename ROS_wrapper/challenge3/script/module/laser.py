#!/usr/bin/env python
import sys, os
sys.path.append("/home/"+os.popen("whoami").read().strip('\n')+"/ART/ROS_wrapper/challenge3/src")
import rospy
from challenge3.msg import Laser_fort
import tty, termios
import math, pylab
import time

anglePerStep = (2 * math.pi) / 2048

class laser:
    #laser constructor
    def __init__(self, distance=17, sideLength=17.6, searchTable=True, precision=1):
        self.precision = precision
        self.table = self.readTableFile('./configure/steps.txt') if searchTable else None
        self.laser2wall = distance
        self.gridLength = sideLength / 16
        self.laser = 0
        
        # ROS init start
        rospy.init_node("Center_Node")
        self.pubStepper = rospy.Publisher("Arduino_Laser_fort", Laser_fort, queue_size=10)
        # ROS init end 

    # laser to center
    def controlLaser(self):
        print '[System]1. Press space to light up/down the laser.'
        print '[System]2. Press \'q\' to exit.'
        precision = self.precision 
        self.precisoin = 5
        while True: 
            ch = getchar()
            if ch == '\x1b' and '[' == getchar():
                ch = getchar()
                if ch == 'A':
                    self.move(0, self.precision, self.laser)
                elif ch == 'B':
                    self.move(0, -self.precision, self.laser)
                elif ch == 'C':
                    self.move(self.precision, 0, self.laser)
                elif ch == 'D':
                    self.move(-self.precision, 0, self.laser)
            elif ch == ' ':
                self.laser = self.laser ^ 1
                self.move(0, 0, self.laser)
            elif ch == 'q':
                print '[System]Exit controlling.'
                break
            else:
                continue
        self.precision = precision
        self.X_Axis = 0
        self.Y_Axis = 0
        return self.X_Axis, self.Y_Axis

    def getCurrentPosition(self):
        return self.X_Axis, self.Y_Axis    
        
    def calculateSteps(self):
        print "[System]Now you can move the laser. Press white spcae to record the point"
        self.measure()
               
    def measure(self):
        message = ""
        while True: 
            ch = getchar()
            if ch == '\x1b' and '[' == getchar():
                ch = getchar()
                if ch == 'A':
                    self.move(0, self.precision, 1)
                    self.X_Axis = self.X_Axis + 1                    
                elif ch == 'B':
                    self.move(0, -self.precision, 1)
                    self.X_Axis = self.X_Axis - 1
                elif ch == 'C':
                    self.move(self.precision, 0, 1)
                    self.Y_Axis = self.Y_Axis + 1
                elif ch == 'D':
                    self.move(-self.precision, 0, 1)
                    self.Y_Axis = self.Y_Axis - 1
            elif ch == ' ':
                break
            else:
                continue
            message = "X:%5.1d\tY:%5.1d" % (self.X_Axis, self.Y_Axis)
            print "\r%s" % message , 
        print "\n[System]Save point"

    def shoot1(self, x, y):
        # search table
        index = x * 16 + y
        print str(self.table[index]['X-Axis']*self.precision) + '   ' + str(self.table[index]['Y-Axis']*self.precision)
        # the process must wait for seconds until laser daley completed, otherwise the system will get error result
        self.move(self.table[index]['Y-Axis']*self.precision, self.table[index]['X-Axis']*self.precision, 1, 3000)
        time.sleep(5)
        #return to the datum point
        self.move(-self.table[index]['Y-Axis']*self.precision, -self.table[index]['X-Axis']*self.precision, 1, 0)

    def shoot2(self, x, y, datum_point_x=7, datum_point_y=7):
        # calculate the rotate angles
        quadrant = getQuadrant(x, y, datum_point_x, datum_point_y)
        print 'quadrant : %d' % quadrant
        xcoef = 1 if quadrant == 3 or quadrant == 4 else -1
        ycoef = 1 if quadrant == 2 or quadrant == 4 else -1
        base = abs(abs(x-datum_point_x) * self.gridLength - (xcoef * (self.gridLength / 2)))
        height = abs(abs(y-datum_point_y) * self.gridLength - (ycoef * (self.gridLength / 2)))
        laser2target = math.sqrt(pylab.square(base) + pylab.square(height) + pylab.square(self.laser2wall))
        print "base : %f \t height : %f" % (base, height)
        print laser2target
        yaw = math.asin(base / laser2target)
        pitch = math.asin(height / laser2target)
        print "yaw : %f \t pitch : %f" % (yaw, pitch)
        units2yaw = yaw / anglePerStep
        units2pitch = pitch / anglePerStep
        
        if quadrant == 1 or quadrant == 3:
            units2yaw = -units2yaw 
        if quadrant == 3 or quadrant == 4:
            units2pitch = -units2pitch
        print 'steps to yaw %f \t steps to pitch %f' % (units2yaw, units2pitch)
        self.move(units2yaw, units2pitch, 1, 3000)
        time.sleep(5)
        # return to the datum point
        self.move(-units2yaw, -units2pitch, 1, 0)
    
    def shutdown(self):
        self.move(0, 0, 0, 0)

    def move(self, yaw, pitch, h, t=0):
        try:
            if not rospy.is_shutdown(): 
                self.pubStepper.publish( rotsteps1 = yaw, rotsteps2 = pitch, hit = h, delay = t)
        except rospy.ROSInterruptException: 
            pass
    
    def readTableFile(self, path):
        table = list()
        with open(path) as f:
            contents = f.readlines()
            if len(contents) != 256:
                print '[Error]File format error: less than 256 lines'
                return
            else:
                for index in range(256):
                    content = contents[index].split()
                    table.insert(index,{'X-Axis':int(content[0]), 'Y-Axis':int(content[1])})
        return table
                
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

# Return the quadrant of (x,y)
def getQuadrant(x, y, datum_point_x, datum_point_y):
    if x <= datum_point_x and y <= datum_point_y:
        return 1
    elif x <= datum_point_x and y > datum_point_y:
        return 2
    elif x > datum_point_x and y <= datum_point_y:
        return 3
    else:
        return 4

if __name__ == '__main__':
    laser = laser()
    laser.controlLaser()
    while True:
        coordinate = raw_input().split()
        if coordinate[0] == "exit":
            break
        else:
            print "x : %s \t y : %s" % (coordinate[0], coordinate[1])
            laser.shoot2(int(coordinate[0]), int(coordinate[1]))
