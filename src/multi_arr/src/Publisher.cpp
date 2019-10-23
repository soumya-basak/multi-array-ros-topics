#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <geometry_msgs/PoseStamped.h>
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Float64MultiArray.h"
#include <geometry_msgs/Vector3.h>
#include <message_filters/time_synchronizer.h>
#include <geometry_msgs/Point.h>



ros::Publisher node_output_array;
std_msgs::Float64MultiArray arr;
//using namespace sensor_msgs;
using namespace message_filters;
using namespace geometry_msgs;
void arrayCallback(const geometry_msgs::PoseStampedConstPtr& p, const geometry_msgs::Vector3StampedConstPtr& Lv, 
                        const geometry_msgs::Vector3StampedConstPtr& La, const geometry_msgs::PoseStampedConstPtr& Q, 
                        const geometry_msgs::Vector3StampedConstPtr& Av, const geometry_msgs::Vector3StampedConstPtr& Aa)
{
    
    //arr.data.clear();
    Point post, euler;
    post.x = p->pose.position.x;
    euler.x = Q->pose.position.x;
    
    Vector3 lin_vel, lin_acc;
    lin_vel.x = Lv->vector.x;
    lin_acc.x = La->vector.x;
    
    Vector3 ang_vel, ang_acc;
    ang_vel.x = Av->vector.x;
    ang_acc.x = Aa->vector.x;

    arr.data.push_back((double)post.x);
    arr.data.push_back((double)euler.x);
    arr.data.push_back((double)lin_vel.x);
    arr.data.push_back((double)lin_acc.x);
    arr.data.push_back((double)ang_vel.x);
    arr.data.push_back((double)ang_acc.x);
    
    

    //pub3.publish(arr);

    ros::spinOnce();

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "arrayPublisher");
    ros::NodeHandle n;
  
    message_filters::Subscriber<geometry_msgs::PoseStamped> Pose(n,"/input/position",1);
    message_filters::Subscriber<geometry_msgs::Vector3Stamped> Lin_vel(n,"/input/linear_velocity",1);
    message_filters::Subscriber<geometry_msgs::Vector3Stamped> Lin_acc(n,"/input/linear_acceleration",1);
    message_filters::Subscriber<geometry_msgs::PoseStamped> Quat(n,"/input/euler_angles",1);
    message_filters::Subscriber<geometry_msgs::Vector3Stamped> Ang_vel(n,"/input/angular_velocity",1);
    message_filters::Subscriber<geometry_msgs::Vector3Stamped> Ang_acc(n,"/input/angular_acceleration",1);
    
    node_output_array = n.advertise<std_msgs::Float64MultiArray>("topic_synchronized", 1000);

    //TimeSynchronizer<PoseStamped, Vector3Stamped, Vector3Stamped, PoseStamped, Vector3Stamped, Vector3Stamped> sync(Pose, Lin_vel, Lin_acc, Quat, Ang_vel, Ang_acc, 10);
    typedef sync_policies::ApproximateTime<PoseStamped, Vector3Stamped, Vector3Stamped, PoseStamped, Vector3Stamped, Vector3Stamped> MySyncPolicy;


    Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), Pose, Lin_vel, Lin_acc, Quat, Ang_vel, Ang_acc);
    sync.registerCallback(boost::bind(&arrayCallback, _1, _2, _3, _4, _5, _6));
    //ros::Rate loop_rate(10);

    node_output_array.publish(arr);
    ros::spin();
    return 0;
}

    


