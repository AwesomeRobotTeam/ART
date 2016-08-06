#include "ros/ros.h"

#define r_init( nodeName) \
	ros::init( argc, argv, nodeName);

#define r_hdlr( hdlName) \
	ros::NodeHandle hdlName;

#define r_newPub( hdlName, msgType, msgName, msgSize) \
	hdlName.advertise< msgType>( #msgName, msgSize);

#define r_newSub( hdlName, msgName, msgSize, callbackFunc) \
	hdlName.subscribe( #msgName, msgSize, callbackFunc);

#define r_newSrvServ( hdlName, srvName, srvFunc) \
	hdlName.advertiseService( #srvName, srvFunc);

#define r_newServClnt( hdlName, srvType, srvName)\
	hdlName.serviceClient<srvType>( #srvName);
