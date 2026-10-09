# ROS Noetic Point Cloud Processor Package (`pcd_publisher_pkg`)

This repository contains the technical solution for the ROS Engineer Skill Test at Widya Robotics. The package is built with the **ROS Noetic** framework and the **Point Cloud Library (PCL)** to load, visualize, and rotate Point Cloud (`.pcd`) data in real time, as well as save it back through a ROS Service interface.

---

## 🛠️ Key Features

1. **Load & Visualize PCD File**: Loads a `.pcd` file dynamically via a ROS private parameter (`_pcd_path:=...`) and publishes it to the `/pcl_output` topic for display in RViz.
2. **Real-Time Y-Axis Rotation**: Rotates the point cloud around the Y axis on the fly — no node restart required — by sending an angle value to the `/rotation_angle` topic.
3. **ROS Service Export**: Provides the `/save_pcd` service to export the rotated point cloud to a new `.pcd` file.
4. **Unit Test**: Includes automated Google Test (`gtest`) testing to guarantee the accuracy of the point cloud transformation math.

---

## 💻 Prerequisites & Dependencies

* **OS**: Ubuntu 20.04 LTS
* **ROS**: ROS Noetic Desktop Full
* **Libraries**: `PCL (Point Cloud Library)`, `roscpp`, `sensor_msgs`, `std_msgs`, `std_srvs`, `pcl_ros`, `pcl_conversions`

---

## 📁 Repository Structure

```text
.
├── README.md
├── cat.pcd                  # Sample input point cloud
├── horse.pcd                # Sample input point cloud
├── lioness.pcd              # Sample input point cloud
├── wolf.pcd                 # Sample input point cloud
└── pcd_publisher_pkg/
    ├── CMakeLists.txt
    ├── package.xml
    ├── srv/
    │   └── SavePCD.srv
    ├── src/
    │   └── pcd_publisher_node.cpp
    ├── test/
    │   └── test_pcd_processor.cpp
    └── output/
        └── cat_rotate_90.pcd    # Sample exported result (Y rotation, 90°)
```

---

## 📡 ROS Interfaces

| Type        | Name               | Message / Service Type        | Description                                  |
|-------------|--------------------|-------------------------------|----------------------------------------------|
| Parameter   | `~pcd_path`        | `string` (private parameter)  | Path to the input `.pcd` file                |
| Publisher   | `/pcl_output`      | `sensor_msgs/PointCloud2`     | Rotated point cloud stream (frame: `map`)    |
| Subscriber  | `/rotation_angle`  | `std_msgs/Float32`            | Target rotation angle around Y, in degrees   |
| Service     | `/save_pcd`        | `pcd_publisher_pkg/SavePCD`   | Saves the rotated cloud to a `.pcd` file     |

`SavePCD.srv` definition:

```text
string data          # request: absolute path of the output .pcd file
---
bool success         # response: save result flag
string message       # response: human-readable result message
```

---

## 🚀 Build Guide

1. **Clone the repository into your ROS workspace**:

```bash
cd ~/catkin_ws/src
git clone <YOUR_REPOSITORY_URL> pcd_publisher_pkg
```

2. **Build the package**:

```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```

---

## 📖 Usage Guide (Execution)

### Step 1: Start the ROS Master

```bash
roscore
```

### Step 2: Run the Main Node

Run the node with the path of the `.pcd` file you want to open:

```bash
source ~/catkin_ws/devel/setup.bash
rosrun pcd_publisher_pkg pcd_publisher_node _pcd_path:=/path/to/input.pcd
```

### Step 3: Visualize in RViz

Open RViz in a new terminal:

```bash
rviz
```

* Set **Fixed Frame** to `map`.
* Click **Add** (bottom left) → choose **PointCloud2**, set the Topic to `/pcl_output`.
* Set **Color Transformer** to `AxisColor` and **Size (m)** to `0.5`.

### Step 4: Rotate the Object (Real-Time)

Send a rotation angle value (in degrees) to the `/rotation_angle` topic:

```bash
rostopic pub -1 /rotation_angle std_msgs/Float32 "data: 90.0"
```

### Step 5: Save the Rotated Result via ROS Service

Call the `/save_pcd` service to export the rotated object to a new `.pcd` file:

```bash
rosservice call /save_pcd "data: '$HOME/catkin_ws/src/pcd_publisher_pkg/output/cat_rotate_90.pcd'"
```

> **Note**: If an empty path is provided, the file is saved as `output_rotated.pcd` in the node's current working directory.

---

## 🧪 Running the Unit Tests (Bonus)

To run the automated `gtest` tests on the point cloud transformation math:

```bash
cd ~/catkin_ws
catkin_make run_tests_pcd_publisher_pkg_gtest_pcd_processor_test
```

Or using `ctest`:

```bash
cd ~/catkin_ws
ctest -V -R pcd_processor_test
```

Test cases covered:

* **TestYAxisRotation90Deg** — a point at `(1, 0, 0)` rotated 90° around Y must land at `(0, 0, -1)`.
* **TestFullRotation360Deg** — a full 360° rotation must return every point to its original position.

---

## 📂 Output `.pcd` File

The `.pcd` file produced by rotating the point cloud 90° around the Y axis is stored in:

```text
pcd_publisher_pkg/output/cat_rotate_90.pcd
```
