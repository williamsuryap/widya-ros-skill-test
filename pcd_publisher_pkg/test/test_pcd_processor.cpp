#include <gtest/gtest.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/common/transforms.h>
#include <cmath>

// Test 1: Menguji kalkulasi rotasi sumbu Y sebesar 90 derajat
TEST(PCDProcessorTest, TestYAxisRotation90Deg) {
    pcl::PointCloud<pcl::PointXYZ> cloud_in;
    
    // Titik awal pada koordinat (1.0, 0.0, 0.0)
    pcl::PointXYZ point_in(1.0f, 0.0f, 0.0f);
    cloud_in.push_back(point_in);

    pcl::PointCloud<pcl::PointXYZ> cloud_out;
    float angle_rad = 90.0f * M_PI / 180.0f; // 90 derajat ke radian

    // Matriks rotasi sumbu Y
    Eigen::Affine3f transform = Eigen::Affine3f::Identity();
    transform.rotate(Eigen::AngleAxisf(angle_rad, Eigen::Vector3f::UnitY()));

    pcl::transformPointCloud(cloud_in, cloud_out, transform);

    // Hasil rotasi sumbu Y sebesar 90 derajat pada titik (1,0,0) seharusnya menjadi (0,0,-1)
    EXPECT_NEAR(cloud_out.points[0].x, 0.0f, 1e-3);
    EXPECT_NEAR(cloud_out.points[0].y, 0.0f, 1e-3);
    EXPECT_NEAR(cloud_out.points[0].z, -1.0f, 1e-3);
}

// Test 2: Menguji rotasi 360 derajat (kembali ke posisi awal)
TEST(PCDProcessorTest, TestFullRotation360Deg) {
    pcl::PointCloud<pcl::PointXYZ> cloud_in;
    pcl::PointXYZ point_in(2.5f, 1.2f, -3.4f);
    cloud_in.push_back(point_in);

    pcl::PointCloud<pcl::PointXYZ> cloud_out;
    float angle_rad = 360.0f * M_PI / 180.0f;

    Eigen::Affine3f transform = Eigen::Affine3f::Identity();
    transform.rotate(Eigen::AngleAxisf(angle_rad, Eigen::Vector3f::UnitY()));

    pcl::transformPointCloud(cloud_in, cloud_out, transform);

    EXPECT_NEAR(cloud_out.points[0].x, cloud_in.points[0].x, 1e-3);
    EXPECT_NEAR(cloud_out.points[0].y, cloud_in.points[0].y, 1e-3);
    EXPECT_NEAR(cloud_out.points[0].z, cloud_in.points[0].z, 1e-3);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
