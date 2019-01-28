#!/usr/bin/env python
import rospy
import sys
from std_msgs.msg import Float32
from sensor_msgs.msg import Joy

class Node_Controller:
	def __init__(self):
		self.joy_sub = rospy.Subscriber("/joy", Joy, self.callback, queue_size=1)
	def callback(self, ros_data):
		 print(">>>>",ros_data.axes[0])


def main(args):
	node_control = Node_Controller()
	rospy.init_node("joy_node", anonymous=True)
	try:
		rospy.spin()
	except KeyboardInterrupt:
		print("turn off node joy ")

if __name__=='__main__':
	main(sys.argv)
