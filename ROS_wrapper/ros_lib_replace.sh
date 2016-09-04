rm -rf ~/sketchbook/libraries/ros_lib/ ~/ros_lib/
rosrun rosserial_arduino make_libraries.py ~
mv ~/ros_lib/ ~/sketchbook/libraries/ros_lib/
