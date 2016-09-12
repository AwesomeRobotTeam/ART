#!/usr/bin/env python
import sys
sys.path.append("/home/rche/ART/ROS_wrapper/challenge3/src")
import rospy
from challenge3.msg import Laser_fort
import tty, termios
import math, pylab
import time

anglePerStep = (2 * math.pi) / 2048

class laser:
    #laser constructor
    def __init__(self, distance=15, sideLength=17.6, searchTable=True, precision=5):
        self.precision = precision
        self.table = self.readTableFile('./steps.txt') if searchTable else None
        self.laser2wall = distance
        self.gridLength = sideLength / 16
        self.laser = 0
        
        # ROS init start
        rospy.init_node("Center_Node")
        self.pubStepper = rospy.Publisher("Arduino_Laser_fort", Laser_fort, queue_size=10)
        # ROS init end
                
        self.controlLaser()

    # laser to center
    def controlLaser(self):
        print '1. Press space to light up/down the laser.'
        print '2. Press \'q\' to exit.'
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
                print 'Exit controlling.'
                return
            else:
                continue

    def calculateSteps(self, axis):
        print 'Press \'s\' to start.'
        while getchar() != 's':
            continue
        if axis == 'X':            
            print 'Move laser fort vertically.'
            steps = self.measure()['X-Axis']
            print 'x : %d' % steps
            return steps*2
        elif axis == 'Y':
            print 'Move laser fort horizontally.'
            steps = self.measure()['Y-Axis']
            print 'y : %d' % steps  
            return steps*2
        else:
            return -1
        return 0

    def measure(self):
        x_moves = 0
        y_moves = 0
        while True: 
            ch = getchar()
            if ch == '\x1b' and '[' == getchar():
                ch = getchar()
                if ch == 'A':
                    self.move(0, self.precision, self.laser)
                    x_moves = x_moves + 1
                elif ch == 'B':
                    self.move(0, -self.precision, self.laser)
                    x_moves = x_moves - 1
                elif ch == 'C':
                    self.move(self.precision, 0, self.laser)
                    y_moves = y_moves + 1
                elif ch == 'D':
                    self.move(-self.precision, 0, self.laser)
                    y_moves = y_moves - 1
            elif ch == 'q':
                return {'X-Axis':x_moves, 'Y-Axis':y_moves}
            else:
                continue

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
        base = abs(x-datum_point_x) * self.gridLength - (self.gridLength / 2)
        height = abs(y-datum_point_y) * self.gridLength - (self.gridLength / 2)
        laser2target = math.sqrt(pylab.square(base) + pylab.square(height) + pylab.square(self.laser2wall))

        yaw = math.asin(x / laser2target)
        pitch = math.acos(y / laser2target)
        units2yaw = yaw / anglePerStep
        units2pitch = pitch / anglePerStep
        quadrant = getQuadrant(x,y)
        print 'quadrant : %d' % quadrant
        if quadrant == 1 or quadrant == 3:
            units2yaw = -units2yaw 
        if quadrant == 3 or quadrant == 4:
            units2pitch = -units2pitch
        print 'steps to yaw %f \t steps to pitch %f' % (units2yaw, units2pitch)
        self.move(units2yaw*self.precision, units2pitch*self.precision, 1, 3000)
        time.sleep(5)
        # return to the datum point
        self.move(-units2yaw*self.precision, -units2pitch*self.precision, 1, 0)
    
    def shutdown(self):
        self.move(0, 0, 0, 0)

    def move(self, x, y, h, t=0):
        try:
            if not rospy.is_shutdown(): 
                self.pubStepper.publish( rotsteps1 = x, rotsteps2 = y, hit = h, delay = t)
        except rospy.ROSInterruptException: 
            pass
    
    def readTableFile(self, path):
        table = list()
        with open(path) as f:
            contents = f.readlines()
            if len(contents) != 256:
                print 'File format error: less than 256 lines'
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
def getQuadrant(x, y):
    if x <= 7 and y <= 7:
        return 1
    elif x <= 7 and y > 7:
        return 2
    elif x > 7 and y <= 7:
        return 3
    else:
        return 4

if __name__ == '__main__':
    laser = laser()
    while True:
        coordinate = raw_input().split()
        if coordinate[0] == 'q':
            break
        else:
            print "x : %s \t y : %s" % (coordinate[0], coordinate[1])
            laser.shoot2(int(coordinate[0]), int(coordinate[1]))
