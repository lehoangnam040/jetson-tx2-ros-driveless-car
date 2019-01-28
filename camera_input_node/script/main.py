#!/usr/bin/env python

import rospy
from std_msgs.msg import Float32
from std_msgs.msg import String
from sensor_msgs.msg import CompressedImage, Image
import cv2
import numpy as np
import sys
from cv_bridge import CvBridge, CvBridgeError
import time

rgb_frame = None
depth_frane = None

class NodeController:
	def __init__(self):
        	self.bridge = CvBridge()
        	self.rgb_image_subscriber = rospy.Subscriber("/camera/rgb/image_raw/compressed", CompressedImage, self.rgbCallback, queue_size = 1)
        	self.depth_image_subscriber = rospy.Subscriber("/camera/depth/image_raw",Image, self.depthCallback, queue_size = 1)
		
	def rgbCallback(self, rgb_data):
		global rgb_frame
        	np_array = np.fromstring(rgb_data.data, np.uint8)
		rgb_frame = cv2.imdecode(np_array, cv2.IMREAD_COLOR) 
        
    	def depthCallback(self, depth_data):
		global depth_frame
        	depth_image = self.bridge.imgmsg_to_cv2(ros_data,"32FC1")
        	depth_array = np.array(depth_image, dtype=np.float32)      
        	# Normalize the depth image to fall between 0 (black) and 1 (white)
        	cv2.normalize(depth_array, depth_array, 0, 1, cv2.NORM_MINMAX)
      		depth_frame = (depth_array*255).astype(np.uint8)
      
def main(args):
	global rgb_frame
	global depth_frame
	rospy.init_node('camera_depth_input_node', anonymous=True)
	node_controller = NodeController()
	rate = rospy.Rate(10)	# rate can be changed
        try:
        	while not rospy.is_shutdown():
			#now = time.time()	#uncomment if collect data
			if rgb_frame is not None:
				#cv2.imwrite(str(now) + "_rgb.png", rgb_frame)	#uncomment if collect data
			if depth_frame is not None:
				#cv2.imwrite(str(now) + "_depth.png", depth_frame) #uncomment if collect data
			##### BinhNA Code ######
			##### BinhNA Code ######
			rate.sleep()
        	rospy.spin()
    	except KeyboardInterrupt:
        	print("Shutting down ROS Image feature detector module")
    	cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)

