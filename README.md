# localization_evaluation_toolkit (C++version)

An evaluator for localization results, which compares with a reliable pose trajectory. Specifically, comparisons between different start times, end times, and periods are supported, since evaluation will be automatically aligned with the one with fewer data.

## 1. Installation & Preparation

Clone this repo to the local environment.

```sh
$ git clone https://gitlab.unity-drive.net/Titankiller/evaluation.git
```

Evaluatable data types are limited to the format of CSV or ros2 bag, and the start time, end time, and period can be different for each data.
Specifically, a valid **CSV file** requires at least the following entries.

- Timestamp
- Position (x,y,z)
- Rotation (Quaternion or Euler[degree or radian])

## 2. Main-evaluation

### 2.1 Pose evaluation

A standard configuration file for pose evaluation consists of the following parts.

- Reference
- Result1, Result2, ...
- Optional settings

It is required that the reference block should have the name of ***Reference*** and the names of result blocks should begin with ***Result***.
When specifying the paths, it is recommended to use the absolute path.
There is a sample YAML file at `config/evaluation_pose.yaml`.
To create your own configuration file, just use the following templates for CSV or ros2 bag,

## Dependencies

```
git clone git clone https://github.com/lava/matplotlib-cpp.git
sudo apt-get install python-matplotlib python-numpy
```

I have already generated the .a static library in */lib*, so you can ignore it.

and run with

```
cd ${YOUR_WORK_SPACE}
mkdir build&&cd build
cmake ..
make
$ ./evalution ${YOUR_CONFIG_PATH}/evaluation_pose.yaml
```

The res will be generated in the *res* floder at the root director.

## DATA

### Reference

| input->header.seq | input->header.stamp | input->header.frame_id | predict_pose_x | predict_pose_y | predict_pose_z | predict_pose_roll | predict_pose_pitch | predict_pose_yaw | exe_time_sum |
| :------------------ | :-------------------- | :----------------------- | :--------------- | :--------------- | :--------------- | :------------------ | :------------------- | :----------------- | :------------- |
|                 6 |    163645070.103866 | velodyne               |   -25869.68916 |    -93280.4161 |        1.61705 |                 0 |            0.00122 |          1.75112 |     1815.736 |

### Res

| time                       | .header.seq | .header.stamp.secs | .header.stamp.nsecs | .header.frame_id | .pose.pose.position.x | .pose.pose.position.y | .pose.pose.position.z | .pose.pose.orientation.x | .pose.pose.orientation.y | .pose.pose.orientation.z | .pose.pose.orientation.w |
| :--------------------------- | :------------ | :------------------- | :-------------------- | :----------------- | :---------------------- | :---------------------- | :---------------------- | :------------------------- | :------------------------- | :------------------------- | :------------------------- |
| 1975/03/10/09:57:49.993503 |         270 |          163645069 |           993503132 | map              |        -25869.6261582 |        -93280.6712351 |         1.55207324028 |        -0.00114102813738 |         0.00135154030913 |           0.767611759692 |           0.640912675625 |

If you have a custom data format, you will need to modify the `DataPack::readData` function in 'src/packer.cpp`.


<details>
<summary>Template of CSV (click to expand)</summary>

```yaml
Reference/Result:
  ## Auxiliary info
  label: any
  type: 0 # [0]: version only for csv, 
  path: /path/to/csv

  ## Time
  separate_time_stamp: false # [true]:Set secs_stamp_column and nsecs_stamp_column / [false]:Set stamp_column
  #--------true--------#
  secs_stamp_column: 2
  nsecs_stamp_column: 3
  #--------false-------#
  stamp_column: 0

  ## Position
  x_column: 1
  y_column: 2
  z_column: 3

  ## Rotation
  use_quaternion: false # [true]:Set Quaternion / [false]:Set Euler
  #--------true--------#
  # Quaternion
  ori_x_column: 8
  ori_y_column: 9
  ori_z_column: 10
  ori_w_column: 11
  #--------false-------#
  # Euler
  use_radian: true # [true]:radian / [false]:degree
  roll_column: 4
  pitch_column: 5
  yaw_column: 6

  ## TF
  tf_time: 0   # [s]
  tf_x: 0      # [m]
  tf_y: 0      # [m]
  tf_z: 0      # [m]
  tf_roll: 0   # [rad]
  tf_pitch: 0  # [rad]
  tf_yaw: 0    # [rad]
  inv_roll: 1  # 1 or -1
  inv_pitch: 1 # 1 or -1
  inv_yaw: 1   # 1 or -1

  # Display ellipse (put 2D covariance in result file)
  display_ellipse: false #unsupport
  covariance_xx_column: 10
  covariance_xy_column: 11
  covariance_yx_column: 12
  covariance_yy_column: 13
```

</details>

<details>
<summary>Template of optional settings (click to expand)</summary>

```yaml
# Horizontal axis
axis_type: 0   # [0]:time, [1]:distance
degree_type: 0 # [0]:radian, [1]:degree

# Trajectory graph dilution
dilution_step: 10 # at least 1, the larger the sparser for better performance

# Trajectory graph numbering
progress_info: 0 # [0]:off, [1]:number, [2]:time, [3]:ros time, [4]:distance
interval: 0      # progress_info is [2]:second, [3]:second, [4]:meter

# Font
title_font_size: 14 
label_font_size: 10
ticks_font_size: 8

# Save
save_figures: true
save_extension_type: png # without "."
save_dataframe: true
output_directory: /path/to/output/directory

use_lerp: false
```

</details>

<details>
<summary>Template of Reference settings (click to expand)</summary>

```
```yaml
Reference:
  ## Auxiliary info
  label: reference
  path: /path/to/reference/csv

  ## Time
  separate_time_stamp: false # [true]:Set secs_stamp_column and nsecs_stamp_column / [false]:Set stamp_column
  #--------true--------#
  secs_stamp_column: 1
  nsecs_stamp_column: 2
  #--------false-------#
  stamp_column: 2

  ## Position
  x_column: 4
  y_column: 5
  z_column: 6

  ## Rotation
  use_quaternion: true # [true]:Set Quaternion / [false]:Set Euler
  #--------true--------#
  # Quaternion
  ori_x_column: 7
  ori_y_column: 8
  ori_z_column: 9
  ori_w_column: 10
  #--------false-------#
  # Euler
  use_radian: true # [true]:radian / [false]:degree
  roll_column: 7
  pitch_column: 8
  yaw_column: 9

  ## Enu velocity
  use_enu_vel: false # [true]: use enu velocity / [false]: ignored
  vel_x_column: 0
  vel_y_column: 0
  vel_z_column: 0

  ## Angular
  use_angular: false # [true]: use angular / [false]: ignored
  angular_x_column: 0
  angular_y_column: 0
  angular_z_column: 0

  # GNSS quality
  use_gnss_qual: false # [true]: use GNSS quality / [false]: ignored
  gnss_qual: 10
```

</details>

<details>
<summary>Template of optional settings (click to expand)</summary>

```yaml
# Trajectory graph numbering
progress_info: 0 # [0]:off, [1]:number, [2]:time, [3]:ros time, [4]:distance
interval: 0      # progress_info is [2]:second, [3]:second, [4]:meter

# Misc
sync_time_threshold: 0.01  # Time threshold for judgment for time synchronization [s]
leap_time: 0.0             # Offset correction for time synchronizatio [s]
based_heading_angle: false # [true]:The heading angle is based on North / [false]:The heading angle is based on East (ros data)
distance_length: 100       # Distance to calculate relative trajectory [m]
distance_step: 50          # Calculate relative trajectories step [m]
eval_step_max: 3.0   # Maximum value of error to be evaluated default [m]

# Font
title_font_size: 14
label_font_size: 10
ticks_font_size: 8

# Save
save_figures: true
save_extension_type: png # without "."
save_dataframe: true
output_directory: /path/to/output/directory
```



