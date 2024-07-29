#include "packer.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <regex>
#include <iomanip>
#include <math.h>
// Define function for string splitting for CSV parsing
std::vector<std::string> DataPack::split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

DataPack::DataPack(const DataParam& param) {
    readData(param);
    adjustTimeStamps(param);
}
void ResDataPack::calc_error(const DataPack& refPack, ResDataPack& resPack) {
    size_t size = refPack.x.size();
    std::cout<<"------------Calc----------- "<<std::endl;
    std::cout<<"resPack time size: "<<resPack.time.size()<<", resPack x size: "<<resPack.x.size()<<std::endl;
    std::cout<<"refPack time size: "<<refPack.time.size()<<", refPack x size: "<<refPack.x.size()<<std::endl;
    std::vector<double> error_x(size), error_y(size), error_z(size), error_longitudinal(size), error_lateral(size);
    int ind=0;
    for (size_t i = 0; i < size; ++i) {
        error_x[i] = resPack.x[i] - refPack.x[i];
        error_y[i] = resPack.y[i] - refPack.y[i];
        error_z[i] = resPack.z[i] - refPack.z[i];
        std::cout<<"calc index: "<<ind++<<std::endl;
        std::cout<<"calc resPack: time: "<<resPack.time[i]<<"("<<resPack.x[i]<<","<<resPack.y[i]<<","<<resPack.z[i]<<")"<<std::endl;
        std::cout<<"calc refPack: time: "<<refPack.time[i]<<"("<<refPack.x[i]<<","<<refPack.y[i]<<","<<refPack.z[i]<<")"<<std::endl;       
        error_longitudinal[i] = error_x[i] * std::cos(refPack.yaw[i]) + error_y[i] * std::sin(refPack.yaw[i]);
        error_lateral[i] = -error_x[i] * std::sin(refPack.yaw[i]) + error_y[i] * std::cos(refPack.yaw[i]);
    }

    resPack.data["error_x"] = error_x;
    resPack.data["error_y"] = error_y;
    resPack.data["error_z"] = error_z;
    resPack.data["error_longitudinal"] = error_longitudinal;
    resPack.data["error_lateral"] = error_lateral;

    std::cout << "calc_error for " << resPack.label << ": size = " << size << "\n" << std::endl;
}
void DataPack::readData(const DataParam& param) {
    if (param.label == "reference") { // reference.csv type
        std::ifstream file(param.path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + param.path);
        }
        std::string line;
        std::getline(file, line); // skip header
        size_t ind=0;
        std::cout << std::fixed << std::setprecision(15);
        while (std::getline(file, line)) {
            auto tokens = split(line, ',');
            //std::cout << "Reading line: " << line << std::endl;
            //std::cout << "Tokens size: " << tokens.size() << std::endl;

            try {
                auto time_value = std::stod(tokens[1]);
                time_value += param.tf_time;

                time.push_back(time_value);
                x.push_back(std::stod(tokens[3]) + param.tf_x);
                y.push_back(std::stod(tokens[4]) + param.tf_y);
                z.push_back(std::stod(tokens[5]) + param.tf_z);
                // std::cout<<"extract time "<<ind++<<"\n"<<std::endl;
                // std::cout<<"refdata timestamp: "<<time_value<<", ("<<tokens[3]<<","<<tokens[4]<<","<<tokens[5]<<")"<<std::endl;
                if (param.use_quaternion) {
                    double qx = std::stod(tokens[param.ori_x_column]);
                    double qy = std::stod(tokens[param.ori_y_column]);
                    double qz = std::stod(tokens[param.ori_z_column]);
                    double qw = std::stod(tokens[param.ori_w_column]);
                    Eigen::Quaterniond quaternion(qw, qx, qy, qz);
                    Eigen::Vector3d euler = quaternion.toRotationMatrix().eulerAngles(2, 1, 0);
                    roll.push_back((euler[2] + param.tf_roll) * param.inv_roll);
                    pitch.push_back((euler[1] + param.tf_pitch) * param.inv_pitch);
                    yaw.push_back((euler[0] + param.tf_yaw) * param.inv_yaw);
                } else {
                    roll.push_back(std::stod(tokens[6]) + param.tf_roll);
                    pitch.push_back(std::stod(tokens[7]) + param.tf_pitch);
                    yaw.push_back(std::stod(tokens[8]) + param.tf_yaw);
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: " << e.what() << " for line: " << line << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Out of range: " << e.what() << " for line: " << line << std::endl;
            }
        }
        std::cout<<"ref extract finish!!!"<<std::endl;
    } else if (param.label != "reference")  { // result_trj_ekf.csv type 
        std::ifstream file(param.path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + param.path);
        }
        std::string line;
        std::getline(file, line); // skip header
        size_t ind=0;
        std::cout << std::fixed << std::setprecision(15);
        while (std::getline(file, line)) {
            auto tokens = split(line, ',');
            try {
                auto time_value = std::stod(tokens[2]) + std::stod(tokens[3]) / 1e9 + param.tf_time;

                time.push_back(time_value);
                x.push_back(std::stod(tokens[5]) + param.tf_x);
                y.push_back(std::stod(tokens[6]) + param.tf_y);
                z.push_back(std::stod(tokens[7]) + param.tf_z);
                // std::cout<<"extract time "<<ind++<<"\n"<<std::endl;
                // std::cout<<"resdata timestamp: "<<time_value<<", ("<<tokens[5]<<","<<tokens[6]<<","<<tokens[7]<<")"<<std::endl;
                double qx = std::stod(tokens[8]);
                double qy = std::stod(tokens[9]);
                double qz = std::stod(tokens[10]);
                double qw = std::stod(tokens[11]);
                Eigen::Quaterniond quaternion(qw, qx, qy, qz);
                Eigen::Vector3d euler = quaternion.toRotationMatrix().eulerAngles(2, 1, 0);
                roll.push_back((euler[2] + param.tf_roll) * param.inv_roll);
                pitch.push_back((euler[1] + param.tf_pitch) * param.inv_pitch);
                yaw.push_back((euler[0] + param.tf_yaw) * param.inv_yaw);

                if (param.display_ellipse) {
                    // Parse covariance values
                    std::vector<double> cov_values;
                    std::istringstream cov_stream(tokens[11].substr(1, tokens[11].size() - 2)); // Remove parentheses
                    std::string cov_value;
                    while (std::getline(cov_stream, cov_value, ' ')) {
                        cov_values.push_back(std::stod(cov_value));
                    }
                    // Assuming cov_xx, cov_xy, cov_yx, cov_yy are stored in specific indices
                    // Adjust these indices based on your actual data structure
                    covariance_xx.push_back(cov_values[0]);
                    covariance_xy.push_back(cov_values[1]);
                    covariance_yx.push_back(cov_values[6]);
                    covariance_yy.push_back(cov_values[7]);
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: " << e.what() << " for line: " << line << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Out of range: " << e.what() << " for line: " << line << std::endl;
            }
        }
    } else {
        throw std::invalid_argument("Unsupported data type");
    }
}

void DataPack::adjustTimeStamps(const DataParam& param) {
    if (param.separate_time_stamp) {
        for (auto& t : time) {
            t += param.tf_time;
        }
    } else {
        for (auto& t : time) {
            if (t > 1e10) {
                t /= 1e9;
            }
            t += param.tf_time;
        }
    }
}

ResDataPack::ResDataPack(const DataParam& param, const DataPack& refPack) : DataPack(param) {
    label = param.label;
    syncWithReference(refPack);
    //calc_error(refPack, *this);
}
void ResDataPack::syncWithReference(const DataPack& refPack) {
    if (refPack.time.empty() || time.empty()) {
        throw std::runtime_error("Reference data or result data is empty.");
    }

    // Debugging: Print initial time ranges
    std::cout << std::fixed << std::setprecision(15);
    std::cout << "Reference time range: " << refPack.time.front() << " to " << refPack.time.back() << std::endl;
    std::cout << "Result time range: " << time.front() << " to " << time.back() << std::endl;
    std::cout <<"before sync res timesize: "<<time.size()<<", ref timesize: "<<refPack.time.size()<<std::endl;

    // Optionally sync spatial data based on timestamps
    std::vector<double> synced_time, synced_x, synced_y, synced_z, synced_roll, synced_pitch, synced_yaw;
    std::vector<double> error_x, error_y, error_z, error_longitudinal, error_lateral;
    size_t ref_index = 0;
    double before_min_time = 0;
    size_t before_sync = -1;
    size_t ind = 0;

    for (size_t i = 0; i < time.size(); ++i) {
        double min_ref_time = std::numeric_limits<double>::max();
        size_t sync_ref_id = 0;

        // 查找最接近的参考时间点
        for (size_t j = ref_index; j < refPack.time.size(); ++j) {
            double current_ref_time = std::abs(refPack.time[j] - time[i]);
            if (current_ref_time < min_ref_time) {
                min_ref_time = current_ref_time;
                sync_ref_id = j;
            } else {
                break;
            }
        }

        // 更新参考索引
        ref_index = sync_ref_id;

        // 处理重复时间点
        if (sync_ref_id == before_sync) {
            if (min_ref_time < before_min_time) {
                synced_time.pop_back();
                synced_x.pop_back();
                synced_y.pop_back();
                synced_z.pop_back();
                synced_roll.pop_back();
                synced_pitch.pop_back();
                synced_yaw.pop_back();
                error_x.pop_back();
                error_y.pop_back();
                error_z.pop_back();
                error_longitudinal.pop_back();
                error_lateral.pop_back();
            } else {
                continue;
            }
        }

        // 直接同步到最接近的参考时间点
        synced_time.push_back(time[i]);
        synced_x.push_back(x[i]);
        synced_y.push_back(y[i]);
        synced_z.push_back(z[i]);
        synced_roll.push_back(roll[i]);
        synced_pitch.push_back(pitch[i]);
        synced_yaw.push_back(yaw[i]);

        // 计算误差
        double ex = x[i] - refPack.x[ref_index];
        double ey = y[i] - refPack.y[ref_index];
        double ez = z[i] - refPack.z[ref_index];
        double elon = ex * std::cos(-refPack.yaw[ref_index]) - ey * std::sin(-refPack.yaw[ref_index]);
        double elat = ex * std::sin(-refPack.yaw[ref_index]) + ey * std::cos(-refPack.yaw[ref_index]);
        error_x.push_back(ex);
        error_y.push_back(ey);
        error_z.push_back(ez);
        error_longitudinal.push_back(elon);
        error_lateral.push_back(elat);

        std::cout << "sync times: " << ind++ << std::endl;
        std::cout << "synced res time: " << time[i] << ", " << "(" << x[i] << "," << y[i] << "," << z[i] << ")" << std::endl;
        std::cout << "synced ref time: " << refPack.time[ref_index] << ", " << "(" << refPack.x[ref_index] << "," << refPack.y[ref_index] << "," << refPack.z[ref_index] << ")" << std::endl;
        std::cout << "sync res-ref error: \n"<<
        "error_x:"<<ex <<"\n"
        "error_y:"<<ey <<"\n"
        "error_z:"<<ez <<"\n"
        "error_longitudinal:"<<elon << "\n"
        "error_lateral:"<<elat << std::endl;      
        before_sync = sync_ref_id;
        before_min_time = min_ref_time;
    }

    // 更新类成员变量
    time = synced_time;
    x = synced_x;
    y = synced_y;
    z = synced_z;
    roll = synced_roll;
    pitch = synced_pitch;
    yaw = synced_yaw;

    // 保存误差数据
    data["error_x"] = error_x;
    data["error_y"] = error_y;
    data["error_z"] = error_z;
    data["error_longitudinal"] = error_longitudinal;
    data["error_lateral"] = error_lateral;

    // Debugging: Print final time ranges
    std::cout << "res Label: " << label << std::endl;
    std::cout << "ref Label: " << refPack.label << std::endl;
    std::cout << "Final result time range: " << time.front() << " to " << time.back() << std::endl;
    std::cout << "after sync res time size: " << time.size() << ", synced time size: " << synced_time.size() << "\n" << std::endl;
}
std::pair<DataPack, std::vector<ResDataPack>> Packer::param2pack(const DataParam& refParam, const std::vector<DataParam>& resParams) {
    DataPack refPack(refParam);
    std::vector<ResDataPack> resPacks;
    for (const auto& resParam : resParams) {
        resPacks.emplace_back(resParam, refPack);
    }
    return {refPack, resPacks};
}


