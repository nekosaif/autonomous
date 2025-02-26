#!/usr/bin/env python3

import rospy
import math
from sbg_driver.msg import SbgEkfEuler

def heading_callback(msg):
    # Convert the yaw from radians to degrees
    yaw_deg = math.degrees(msg.angle.z)
    rospy.loginfo(f"Heading: {yaw_deg:.2f} deg")

rospy.init_node('heading_listener')
rospy.Subscriber("/sbg/ekf_euler", SbgEkfEuler, heading_callback)
rospy.spin()
