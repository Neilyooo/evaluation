
#include <iostream>
#include "configer.h"
#include <stdexcept>

void DataParam::loadFromNode(const YAML::Node& node) {
    try {
        //std::cout << "Loading label..." << std::endl;
        label = node["label"].as<std::string>();
        //std::cout << "label: " << label << std::endl;
        
        //std::cout << "Loading type..." << std::endl;
        type = node["type"].as<int>();
        //std::cout << "type: " << type << std::endl;
        
        //std::cout << "Loading path..." << std::endl;
        path = node["path"].as<std::string>();
        //std::cout << "path: " << path << std::endl;
        
        //std::cout << "Loading tf_time..." << std::endl;
        tf_time = node["tf_time"].as<double>();
        //std::cout << "tf_time: " << tf_time << std::endl;
        
        //std::cout << "Loading tf_x..." << std::endl;
        tf_x = node["tf_x"].as<double>();
        //std::cout << "tf_x: " << tf_x << std::endl;
        
        //std::cout << "Loading tf_y..." << std::endl;
        tf_y = node["tf_y"].as<double>();
        //std::cout << "tf_y: " << tf_y << std::endl;
        
        //std::cout << "Loading tf_z..." << std::endl;
        tf_z = node["tf_z"].as<double>();
        //std::cout << "tf_z: " << tf_z << std::endl;
        
        //std::cout << "Loading tf_roll..." << std::endl;
        tf_roll = node["tf_roll"].as<double>();
        //std::cout << "tf_roll: " << tf_roll << std::endl;
        
        //std::cout << "Loading tf_pitch..." << std::endl;
        tf_pitch = node["tf_pitch"].as<double>();
        //std::cout << "tf_pitch: " << tf_pitch << std::endl;
        
        //std::cout << "Loading tf_yaw..." << std::endl;
        tf_yaw = node["tf_yaw"].as<double>();
        //std::cout << "tf_yaw: " << tf_yaw << std::endl;

        //std::cout << "Loading inv_roll..." << std::endl;
        inv_roll = node["inv_roll"].as<bool>();
        //std::cout << "inv_roll: " << inv_roll << std::endl;
        
        //std::cout << "Loading inv_pitch..." << std::endl;
        inv_pitch = node["inv_pitch"].as<bool>();
        //std::cout << "inv_pitch: " << inv_pitch << std::endl;
        
        //std::cout << "Loading inv_yaw..." << std::endl;
        inv_yaw = node["inv_yaw"].as<bool>();
        //std::cout << "inv_yaw: " << inv_yaw << std::endl;
        
        //std::cout << "Loading display_ellipse..." << std::endl;
        // display_ellipse = node["display_ellipse"].as<bool>();
        if (node["display_ellipse"].IsDefined()) {
            display_ellipse = node["display_ellipse"].as<bool>();
            //std::cout << "display_ellipse: " << display_ellipse << std::endl;
        } else {
            std::cerr << "display_ellipse not defined in YAML file." << std::endl;
        }
        
        if (type == 0) {  // CSV type
            //std::cout << "Loading CSV specific fields..." << std::endl;
            separate_time_stamp = node["separate_time_stamp"].as<bool>();
            //std::cout << "separate_time_stamp: " << separate_time_stamp << std::endl;
            
            stamp_column = node["stamp_column"].as<int>();
            //std::cout << "stamp_column: " << stamp_column << std::endl;
            
            secs_stamp_column = node["secs_stamp_column"].as<int>();
            //std::cout << "secs_stamp_column: " << secs_stamp_column << std::endl;
            
            nsecs_stamp_column = node["nsecs_stamp_column"].as<int>();
            //std::cout << "nsecs_stamp_column: " << nsecs_stamp_column << std::endl;
            
            x_column = node["x_column"].as<int>();
            //std::cout << "x_column: " << x_column << std::endl;
            
            y_column = node["y_column"].as<int>();
            //std::cout << "y_column: " << y_column << std::endl;
            
            z_column = node["z_column"].as<int>();
            //std::cout << "z_column: " << z_column << std::endl;
            
            use_quaternion = node["use_quaternion"].as<bool>();
            //std::cout << "use_quaternion: " << use_quaternion << std::endl;
            
            ori_x_column = node["ori_x_column"].as<int>();
            //std::cout << "ori_x_column: " << ori_x_column << std::endl;
            
            ori_y_column = node["ori_y_column"].as<int>();
            //std::cout << "ori_y_column: " << ori_y_column << std::endl;
            
            ori_z_column = node["ori_z_column"].as<int>();
            //std::cout << "ori_z_column: " << ori_z_column << std::endl;
            
            ori_w_column = node["ori_w_column"].as<int>();
            //std::cout << "ori_w_column: " << ori_w_column << std::endl;
            
            use_radian = node["use_radian"].as<bool>();
            //std::cout << "use_radian: " << use_radian << std::endl;
            
            roll_column = node["roll_column"].as<int>();
            //std::cout << "roll_column: " << roll_column << std::endl;
            
            pitch_column = node["pitch_column"].as<int>();
            //std::cout << "pitch_column: " << pitch_column << std::endl;
            
            yaw_column = node["yaw_column"].as<int>();
            //std::cout << "yaw_column: " << yaw_column << std::endl;
            
            // covariance_xx_column = node["covariance_xx_column"].as<int>();
            // std::cout << "covariance_xx_column: " << covariance_xx_column << std::endl;
            if (node["covariance_xx_column"].IsDefined()) {
            display_ellipse = node["covariance_xx_column"].as<int>();
            //std::cout << "covariance_xx_column: " << display_ellipse << std::endl;
            } else {
            std::cerr << "covariance_xx_column not defined in YAML file." << std::endl;
            }
            // covariance_xy_column = node["covariance_xy_column"].as<int>();
            // //std::cout << "covariance_xy_column: " << covariance_xy_column << std::endl;
            if (node["covariance_xy_column"].IsDefined()) {
            display_ellipse = node["covariance_xy_column"].as<int>();
            //std::cout << "covariance_xy_column: " << display_ellipse << std::endl;
            } else {
            std::cerr << "covariance_xy_column not defined in YAML file." << std::endl;
            }
            // covariance_yx_column = node["covariance_yx_column"].as<int>();
            // //std::cout << "covariance_yx_column: " << covariance_yx_column << std::endl;
            if (node["covariance_yx_column"].IsDefined()) {
            display_ellipse = node["covariance_yx_column"].as<int>();
            //std::cout << "covariance_yx_column: " << display_ellipse << std::endl;
            } else {
            std::cerr << "covariance_yx_column not defined in YAML file." << std::endl;
            }
            // covariance_yy_column = node["covariance_yy_column"].as<int>();
            // //std::cout << "covariance_yy_column: " << covariance_yy_column << std::endl;
            if (node["covariance_yy_column"].IsDefined()) {
            display_ellipse = node["covariance_yy_column"].as<int>();
            //std::cout << "covariance_yy_column: " << display_ellipse << std::endl;
            } else {
            std::cerr << "covariance_yy_column not defined in YAML file." << std::endl;
            }
        } else if (type == 1) { // ROS2 bag type
            //std::cout << "Loading ROS2 bag specific fields..." << std::endl;
            topic_name = node["topic_name"].as<std::string>();
            //std::cout << "topic_name: " << topic_name << std::endl;
            
            storage_id = node["storage_id"].as<std::string>();
            //std::cout << "storage_id: " << storage_id << std::endl;
            
            serialization_format = node["serialization_format"].as<std::string>();
            //std::cout << "serialization_format: " << serialization_format << std::endl;
        }
    } catch (const YAML::BadConversion& e) {
        std::cerr << "BadConversion error: " << e.what() << std::endl;
        throw;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        throw;
    }
}

std::tuple<DataParam, std::vector<DataParam>, OptParam> Configer::yaml2params(const YAML::Node& config) {
    DataParam refParam;
    std::cout << "Loading Reference..." << std::endl;
    refParam.loadFromNode(config["Reference"]);
    std::cout << "Reference loaded successfully." << std::endl;

    std::vector<DataParam> resParams;
    for (const auto& entry : config) {
        if (entry.first.as<std::string>().find("Result") != std::string::npos) {
            std::cout << "Loading " << entry.first.as<std::string>() << "..." << std::endl;
            DataParam resParam;
            resParam.loadFromNode(entry.second);
            resParams.push_back(resParam);
            std::cout << entry.first.as<std::string>() << " loaded successfully." << std::endl;
        }
    }

    OptParam optParam;
    std::cout << "Loading Option..." << std::endl;
    if(config["Option"])
    {
        std::cout<<"Option"<<std::endl;
    }else{
        std::cout<<"Option not defined in yaml file"<<std::endl;
    }
        if (config["Option"]["axis_type"].IsDefined()) {
            optParam.axis_type = config["Option"]["axis_type"].as<int>();
            //std::cout << "axis_type: " << optParam.axis_type << std::endl;
        } else {
            std::cerr << "axis_type not defined in YAML file." << std::endl;
        }
        
        if (config["Option"]["degree_type"].IsDefined()) {
            optParam.degree_type = config["Option"]["degree_type"].as<int>();
            //std::cout << "degree_type: " << optParam.degree_type << std::endl;
        } else {
            std::cerr << "degree_type not defined in YAML file." << std::endl;
        }
        
        if (config["Option"]["dilution_step"].IsDefined()) {
            optParam.dilution_step = config["Option"]["dilution_step"].as<int>();
            //std::cout << "dilution_step: " << optParam.dilution_step << std::endl;
        } else {
            std::cerr << "dilution_step not defined in YAML file." << std::endl;
        }
        
        if (config["Option"]["progress_info"].IsDefined()) {
            optParam.progress_info = config["Option"]["progress_info"].as<int>();
            //std::cout << "progress_info: " << optParam.progress_info << std::endl;
        } else {
            std::cerr << "progress_info not defined in YAML file." << std::endl;
        }
        
        if (config["Option"]["interval"].IsDefined()) {
            optParam.interval = config["Option"]["interval"].as<int>();
            //std::cout << "interval: " << optParam.interval << std::endl;
        } else {
            std::cerr << "interval not defined in YAML file." << std::endl;
        }
        
        if (config["Option"]["title_font_size"].IsDefined()) {
            optParam.title_font_size = config["Option"]["title_font_size"].as<float>();
            //std::cout << "title_font_size: " << optParam.title_font_size << std::endl;
        } else {
            std::cerr << "title_font_size not defined in YAML file." << std::endl;
        }
        
        if (config["Option"]["label_font_size"].IsDefined()) {
            optParam.label_font_size = config["Option"]["label_font_size"].as<float>();
            //std::cout << "label_font_size: " << optParam.label_font_size << std::endl;
        } else {
            std::cerr << "label_font_size not defined in YAML file." << std::endl;
        }
        
        if (config["Option"]["ticks_font_size"].IsDefined()) {
            optParam.ticks_font_size = config["Option"]["ticks_font_size"].as<float>();
            //std::cout << "ticks_font_size: " << optParam.ticks_font_size << std::endl;
        } else {
            std::cerr << "ticks_font_size not defined in YAML file." << std::endl;
        }
        
        if (config["Option"]["save_figures"].IsDefined()) {
            optParam.save_figures = config["Option"]["save_figures"].as<bool>();
            //std::cout << "save_figures: " << optParam.save_figures << std::endl;
        } else {
            std::cerr << "save_figures not defined in YAML file." << std::endl;
        }
        
        if (config["Option"]["save_extension_type"].IsDefined()) {
            optParam.save_extension_type = config["Option"]["save_extension_type"].as<std::string>();
            //std::cout << "save_extension_type: " << optParam.save_extension_type << std::endl;
        } else {
            std::cerr << "save_extension_type not defined in YAML file." << std::endl;
        }
        
        if (config["Option"]["save_dataframe"].IsDefined()) {
            optParam.save_dataframe = config["Option"]["save_dataframe"].as<bool>();
            //std::cout << "save_dataframe: " << optParam.save_dataframe << std::endl;
        } else {
            std::cerr << "save_dataframe not defined in YAML file." << std::endl;
        }
        
        if (config["Option"]["output_directory"].IsDefined()) {
            optParam.output_directory = config["Option"]["output_directory"].as<std::string>();
            //std::cout << "output_directory: " << optParam.output_directory << std::endl;
        } else {
            std::cerr << "output_directory not defined in YAML file." << std::endl;
        }
        
        if (config["Option"]["use_lerp"].IsDefined()) {
            optParam.use_lerp = config["Option"]["use_lerp"].as<bool>();
            //std::cout << "use_lerp: " << optParam.use_lerp << std::endl;
        } else {
            std::cerr << "use_lerp not defined in YAML file." << std::endl;
        }
        //std::cout << "Option loaded successfully." << std::endl;

    return std::make_tuple(refParam, resParams, optParam);
}

