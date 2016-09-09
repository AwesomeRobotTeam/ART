#!/usr/bin/env python
import sys
sys.path.append("/home/rche/ART/ROS_wrapper/challenge3/src")
import rospy
from challenge3.msg import Laser_fort
import tty, termios, sys
import signal

class laser:
    #constructor
    def __init__(self, distance, width=-1, height=-1, precision=5, table=list()):
        self.precision = precision
        self.table = table
        self.laser2wall_distance = distance
        self.laser = 0
        
        # ROS init start
        rospy.init_node("Center_Node")
        self.pubStepper = rospy.Publisher("Arduino_Laser_fort", Laser_fort, queue_size=10)
        # ROS init end
                
        self.center()
        if width >= 0:
            self.width_steps = width
        else:
            self.width_steps = self.calculateSteps('X')
        if height >= 0:
            self.height_steps = height
        else:
            self.height_steps = self.calculateSteps('Y')
 
        # initialize the stepper and locate the 4 corner
    def initLaser(self):
        print 'Start to initialize the laser fort...please control the stepper to target on 4 corners'
        print 'Press \'q\' to exit the init'
        i = 1
        while getchar() != 'o': 
            continue
        self.laser = 1
        self.move(0, 0, self.laser)
        while i <= 4: 
            ch = getchar()
            if ch == '\x1b' and '[' == getchar():
                ch = getchar() 
                if ch == 'A':
                    print 'up'
                    self.move(0, self.precision, self.laser)
                elif ch == 'B':
                    print 'down'
                    self.move(0, -self.precision, self.laser)
                elif ch == 'C':
                    print 'right'
                    self.move(self.precision, 0, self.laser)
                elif ch == 'D':
                    print 'left'
                    self.move(-self.precision, 0, self.laser)
            elif ch == ' ':
                print 'set %d\'s corner' % i
                #TODO transmit the message to arduino
                i = i + 1
            elif ch == 'q':
                self.laser = 0
                self.move(0, 0, self.laser)
                return
            else:
                continue
        print 'All corners are set'
        #TODO transmit the message to arduino

    # laser to center
    def center(self):
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

    def shoot(self, x, y):
        #search table
        print 'shoot'

    def back2center(self):
        print 'back2center'

    def move(self, x,y,h):
        try:
            if not rospy.is_shutdown(): 
                self.pubStepper.publish( rotsteps1 = x, rotsteps2 = y, hit = h)
        except rospy.ROSInterruptException: 
            pass
        
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

laser = laser(10 , 0, 0)
