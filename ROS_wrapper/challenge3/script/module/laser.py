#!/usr/bin/env python
import sys
sys.path.append("/home/rche/ART/ROS_wrapper/challenge3/src")
import rospy
from challenge3.msg import Laser_fort
import tty, termios
import signal
import time

class laser:
    #laser constructor
    def __init__(self, distance, sideLength, height, width, searchTable=True, precision=5):
        self.precision = precision
        self.table = self.readTableFile('./steps.txt') if searchTable else None
        self.laser2wall_distance = distance
        self.gridLength = sideLength / 16
        self.laser = 0
        
        # ROS init start
        rospy.init_node("Center_Node")
        self.pubStepper = rospy.Publisher("Arduino_Laser_fort", Laser_fort, queue_size=10)
        # ROS init end
                
        self.controlLaser()
        self.width_steps = width if width >= 0 else self.calculateSteps('X')
        self.height_steps = height if height >= 0 else self.calculateSteps('Y') 

    # laser to center
    def controlLaser(self):
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
        self.move(self.table[index]['Y-Axis']*self.precision, self.table[index]['X-Axis']*self.precision, 1)
        time.sleep(3)
        #self.move(0 ,0 , 0)
        self.move(-self.table[index]['Y-Axis']*self.precision, -self.table[index]['X-Axis']*self.precision, 1)

    def shoot2(self, x, y):
        # calculate the anglei
        None

    def move(self, x, y, h):
        try:
            if not rospy.is_shutdown(): 
                self.pubStepper.publish( rotsteps1 = x, rotsteps2 = y, hit = h)
        except rospy.ROSInterruptException: 
            pass

    def shutdown(self):
        self.move(0, 0, 0)

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
