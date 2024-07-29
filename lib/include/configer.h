#ifndef CONFIGER_H
#define CONFIGER_H

#include <string>
#include <vector>
#include <tuple>
#include "yaml-cpp/yaml.h"

struct DataParam {
    std::string label;
    int type;
    std::string path;
    double tf_time;
    double tf_x, tf_y, tf_z;
    double tf_roll, tf_pitch, tf_yaw;
    bool inv_roll, inv_pitch, inv_yaw;
    bool display_ellipse;

    bool separate_time_stamp;
    int stamp_column, secs_stamp_column, nsecs_stamp_column;
    int x_column, y_column, z_column;
    bool use_quaternion;
    int ori_x_column, ori_y_column, ori_z_column, ori_w_column;
    bool use_radian;
    int roll_column, pitch_column, yaw_column;
    int covariance_xx_column, covariance_xy_column, covariance_yx_column, covariance_yy_column;

    std::string topic_name;
    std::string storage_id;
    std::string serialization_format;

    void loadFromNode(const YAML::Node& node);
};

struct OptParam {
    std::string title;
    std::string xlabel;
    std::string ylabel;
    std::string output_directory;
    std::string filename;
    int fig_width;
    int fig_height;
    int axis_type;
    int degree_type;
    int dilution_step;
    int progress_info;
    int interval;
    float title_font_size;
    float label_font_size;
    float ticks_font_size;
    bool save_figures;
    bool save_dataframe;
    std::string save_extension_type;
    bool use_lerp;
};

class Configer {
public:
    static std::tuple<DataParam, std::vector<DataParam>, OptParam> yaml2params(const YAML::Node& config);
};

#endif // CONFIGER_H
