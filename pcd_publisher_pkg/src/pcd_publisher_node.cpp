#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <std_msgs/Float32.h>
#include <pcd_publisher_pkg/SavePCD.h>      // Header dari Custom Service yang dikompilasi ROS
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/common/transforms.h>
#include <pcl_conversions/pcl_conversions.h>
#include <string>
#include <cmath>

float current_angle_deg = 0.0;
pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_original(new pcl::PointCloud<pcl::PointXYZ>);
pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_rotated(new pcl::PointCloud<pcl::PointXYZ>);

void angleCallback(const std_msgs::Float32::ConstPtr& msg) {
    current_angle_deg = msg->data;
    ROS_INFO("Sudut rotasi sumbu Y diubah ke: %.2f derajat", current_angle_deg);
}

bool savePcdCallback(pcd_publisher_pkg::SavePCD::Request& req, pcd_publisher_pkg::SavePCD::Response& res) {
    std::string save_path = req.data;
    
    if (save_path.empty()) {
        save_path = "output_rotated.pcd";
    }

    if (pcl::io::savePCDFileBinary(save_path, *cloud_rotated) == 0) {
        res.success = true;
        res.message = "Berhasil menyimpan point cloud terrotasi ke: " + save_path;
        ROS_INFO("%s", res.message.c_str());
    } else {
        res.success = false;
        res.message = "Gagal menyimpan file PCD ke: " + save_path;
        ROS_ERROR("%s", res.message.c_str());
    }
    return true;
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "pcd_publisher_node");
    ros::NodeHandle nh("~");
    ros::NodeHandle nh_public;

    std::string pcd_path;
    if (!nh.getParam("pcd_path", pcd_path)) {
        ROS_ERROR("Parameter 'pcd_path' tidak ditemukan!");
        return -1;
    }

    if (pcl::io::loadPCDFile<pcl::PointXYZ>(pcd_path, *cloud_original) == -1) {
        ROS_ERROR("Gagal membaca file PCD pada path: %s", pcd_path.c_str());
        return -1;
    }
    ROS_INFO("Berhasil memuat %zu data titik.", cloud_original->points.size());

    ros::Subscriber sub_angle = nh_public.subscribe("/rotation_angle", 10, angleCallback);
    ros::Publisher pub = nh_public.advertise<sensor_msgs::PointCloud2>("/pcl_output", 1);
    ros::ServiceServer service_save = nh_public.advertiseService("/save_pcd", savePcdCallback);

    ros::Rate loop_rate(10);

    while (ros::ok()) {
        ros::spinOnce();

        float angle_rad = current_angle_deg * M_PI / 180.0f;
        Eigen::Affine3f transform = Eigen::Affine3f::Identity();
        transform.rotate(Eigen::AngleAxisf(angle_rad, Eigen::Vector3f::UnitY()));

        pcl::transformPointCloud(*cloud_original, *cloud_rotated, transform);

        sensor_msgs::PointCloud2 ros_output;
        pcl::toROSMsg(*cloud_rotated, ros_output);
        ros_output.header.frame_id = "map";
        ros_output.header.stamp = ros::Time::now();

        pub.publish(ros_output);
        loop_rate.sleep();
    }

    return 0;
}
