import sys
import os
import cv2
sys.path.append('/home/'+os.popen("whoami").read().strip('\n')+'/ART/ROS_wrapper/challenge3/script/module/')
from laser import laser

line_num = 5

# laser config
laser = laser()

x, y = laser.controlLaser()
if (not x == 0) and (not y == 0):
    print "[Error]The point is not at center."
    sys_exit()
else:
    if (not laser.X_Axis == 0) and (not laser.Y_Axis == 0): 
        print "[Error]Not in the center"
    positions = list()
    for i in range(line_num):
        positions.append(laser.calculateSteps())
    laser.center()
    with open('steps.config', 'w') as fp:
        for index in range(256):
            if index >= line_num:
                fp.write("0 0\n")
            else:
                line = "%d %d\n" % (positions[index][0], positions[index][1])
                fp.write(line)

# laser test
laser.table = laser.readTableFile('steps.config')
for i in range(line_num):
    laser.shoot1(0, i)

laser.center()
print "[System]Test 4 corners"
laser.shoot1(0, 0)
laser.shoot1(0, 15)
laser.shoot1(15, 0)
laser.shoot1(15, 15)

# return to center and shutdown
laser.center()
laser.shutdown()
