import sys
import os
import cv2
sys.path.append('/home/'+os.popen("whoami").read().strip('\n')+'/ART/ROS_wrapper/challenge3/script/module/')
from laser import laser

def method1():
    line_num = 10
    # laser config
    laserfort = laser(precision=2)
    
    x, y = laserfort.controlLaser()
    if (not x == 0) and (not y == 0):
        print "[Error]The point is not at center."
        sys.exit()
    else:
        if (not laserfort.X_Axis == 0) and (not laserfort.Y_Axis == 0): 
            print "[Error]Not in the center"
        positions = list()
        for i in range(line_num):
            positions.append(laserfort.calculateSteps(i))
        laserfort.center()
        raw_input("[System]Press enter to write data to file")
        with open('steps.config', 'w') as fp:
            for index in range(256):
                if index >= line_num:
                    fp.write("0 0\n")
                else:
                    line = "%d %d\n" % (positions[index][0], positions[index][1])
                    fp.write(line)
    
    # laser test
    laserfort.table = laserfort.readTableFile('steps.config')
    for i in range(len(positions)):
        laserfort.shoot1(0, i, debug=True)
    
    # return to center and shutdown
    laserfort.shutdown()

def method2():
    # this method need to calibrate 16 points
    x, y = laserfort.controlLaser()
    if (not x == 0) and (not y == 0):
        print "[Error]The point is not at center."
        sys.exit()
    else:
        if (not laserfort.X_Axis == 0) and (not laserfort.Y_Axis == 0): 
            print "[Error]Not in the center"
        positions = list()
        datums = list()
        for i in range(4):
            print "[System]Calibrate the line %d points" %  i
            for j in range(4):
                datums.append(laserfort.calculateSteps())
        heads = (0, 7, 8, 15)
        for i in range(4):
            positions.insert(heads[i]*16+0, datums[i*4+0])
            positions.insert(heads[i]*16+7, datums[i*4+1])
            positions.insert(heads[i]*16+8, datums[i*4+2])
            positions.insert(heads[i]*16+15,datums[i*4+3])
        exdatums = list()
        for i in range(16):
            exdatums.append(datums[0])
            exdatums.append(datums[1])
            exdatums.append(datums[2])
            exdatums.append(datums[3])
                
def method3():
    line_num = 10
    # laser config
    laserfort = laser(precision=2)
    
    x, y = laserfort.controlLaser()
    if (not x == 0) and (not y == 0):
        print "[Error]The point is not at center."
        sys.exit()
    else:
        if (not laserfort.X_Axis == 0) and (not laserfort.Y_Axis == 0): 
            print "[Error]Not in the center"
        positions = list()
        for i in range(line_num):
            positions.append(laserfort.calculateSteps(i))
            laserfort.center()
        raw_input("[System]Press enter to write data to file")
        with open('steps.config', 'w') as fp:
            for index in range(256):
                if index >= line_num:
                    fp.write("0 0\n")
                else:
                    line = "%d %d\n" % (positions[index][0], positions[index][1])
                    fp.write(line)
    
    # laser test
    laserfort.table = laserfort.readTableFile('steps.config')
    for i in range(len(positions)):
        laserfort.shoot1((i/16), (i%16),debug=True)
    
    # return to center and shutdown
    laserfort.shutdown()

def method4():
    # laser config
    laserfort = laser(precision=2)
    
    x,y = laserfort.controlLaser()
    if (not x == 0) and (not y == 0):
        print "[Error]The point is not at center."
        sys.exit()
    else:
        if (not laserfort.X_Axis == 0) and (not laserfort.Y_Axis == 0): 
            print "[Error]Not in the center"
        while True:
            ay, ax= laserfort.calculateSteps()
            laserfort.center()
            #laserfort.move(ax,ay, 1, 3000)
            #laserfort.center()
   
def method5():
    laserfort = laser()
    laserfort.controlLaser()
    while True:
        coordinate = raw_input().split()
        if coordinate[0] == "exit":
            break
        else:
            print "x : %s \t y : %s" % (coordinate[0], coordinate[1])
            laserfort.move(int(coordinate[1]), int(coordinate[0]), 1, 3000)
            laserfort.center()
    laserfort.shutdown()
 
if __name__ == "__main__":
    if len(sys.argv) > 2:
        print "Must to indicate the method"
    method = int(sys.argv[1])
    if method == 1:
        method1()
    elif method == 2:
        method2()
    elif method == 3:
        method3()
    elif method == 4:
        method4()
    elif method == 5:
        method5()
