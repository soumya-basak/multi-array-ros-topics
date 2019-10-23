Write a ROS node using C / C++ to read multiple inputs and sequentially publish as
one output in an array. Justify your choice for C or C++.
Inputs topics:
/input/position
/input/linear_velocity
/input/linear_acceleration
/input/euler_angles
/input/angular_velocity
/input/angular_acceleration
Output topic:
/output




Fixed:
use ApproximateTime API to synchronise time to read multiple input simultaniously. [http://wiki.ros.org/message_filters/ApproximateTime], because Timesynchronizer() function thrown build error.
fixed CmakeList.txt to add packages for build dependencies geometry_msgs, sensor_msgs, message_filters.

Thanks & Regards,

Soumya.
