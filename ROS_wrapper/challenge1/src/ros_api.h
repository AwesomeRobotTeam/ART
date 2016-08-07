#include "ros/ros.h"

#define r_init( nodeName) \
	ros::init( argc, argv, #nodeName);

#define r_hdlr( hdlName) \
	ros::NodeHandle hdlName;

#define r_newPub( pubName, hdlName, msgType, msgName, msgSize) \
	ros::Publisher pubName = hdlName.advertise<msgType>( #msgName, msgSize);

#define r_newSub( subName, hdlName, msgName, msgSize, callbackFunc) \
	ros::Subscriber subName = hdlName.subscribe( #msgName, msgSize, callbackFunc);

#define r_newSrvServ( servName,hdlName, srvName, srvFunc) \
	ros::ServiceServer servName = hdlName.advertiseService( #srvName, srvFunc);

#define r_newServClnt( clntName, hdlName, srvType, srvName)\
	ros::ServiceClient clntName = hdlName.serviceClient<srvType>( #srvName);
