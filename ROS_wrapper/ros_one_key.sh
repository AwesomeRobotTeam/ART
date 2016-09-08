#Usage
echo "\n\nUsage: . ros_lib_replace challengeX\n\n"

. ros.cfg
rosmake $1
. ros_lib_replace.sh
