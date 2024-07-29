#include "packer.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <regex>
#include <Eigen/Geometry>
// 定义用于 CSV 解析的字符串拆分函数
std::vector<std::string> DataPack::split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

bool isValidNumber(const std::string& str) {
    std::regex number_regex("^-?\\d*(\\.\\d+)?$");
    return std::regex_match(str, number_regex);
}

double parseTimestamp(const std::string& timestamp) {
    std::regex timestamp_regex(R"((\d{4})/(\d{2})/(\d{2})/(\d{2}):(\d{2}):(\d{2})\.(\d{6}))");
    std::smatch match;
    if (std::regex_match(timestamp, match, timestamp_regex)) {
        int year = std::stoi(match[1]);
        int month = std::stoi(match[2]);
        int day = std::stoi(match[3]);
        int hour = std::stoi(match[4]);
        int minute = std::stoi(match[5]);
        int second = std::stoi(match[6]);
        int microsecond = std::stoi(match[7]);

        std::tm tm = {};
        tm.tm_year = year - 1900;
        tm.tm_mon = month - 1;
        tm.tm_mday = day;
        tm.tm_hour = hour;
        tm.tm_min = minute;
        tm.tm_sec = second;

        auto time_since_epoch = std::mktime(&tm) + microsecond / 1e6;
        return static_cast<double>(time_since_epoch);
    } else {
        throw std::invalid_argument("Invalid timestamp format");
    }
}

DataPack::DataPack(const DataParam& param) {
    readData(param);
    adjustTimeStamps(param);
}
void DataPack::readData(const DataParam& param) {
    std::ifstream file(param.path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open CSV file: " + param.path);
    }
    std::string line;

    if (param.type == 0) { // reference.csv type
        while (std::getline(file, line)) {
            auto tokens = split(line, ','); // Assuming comma-separated values
            // Skip header row
            if (tokens[0] == "input->header.seq") {
                continue;
            }
            std::cout<<"tokens: "<<tokens[0]<<std::endl;
            // Parse tokens into respective fields
            if (tokens.size() > 5) { // Adjust indices as needed
                try {
                    double time_value = std::stod(tokens[1]) + param.tf_time;
                    std::cout << "Parsing header: " << tokens[1] << std::endl;
                    time.push_back(time_value);
                    x.push_back(std::stod(tokens[3]) + param.tf_x);
                    std::cout << "Parsing ref x: " << tokens[3] << std::endl;
                    y.push_back(std::stod(tokens[4]) + param.tf_y);
                    std::cout << "Parsing ref y: " << tokens[4] << std::endl;
                    z.push_back(std::stod(tokens[5]) + param.tf_z);
                    std::cout << "Parsing ref z: " << tokens[5] << std::endl;

                    roll.push_back(std::stod(tokens[6]) + param.tf_roll);
                    std::cout << "Parsing ref roll: " << tokens[6] << std::endl;
                    pitch.push_back(std::stod(tokens[7]) + param.tf_pitch);
                    std::cout << "Parsing ref pitch: " << tokens[7] << std::endl;
                    yaw.push_back(std::stod(tokens[8]) + param.tf_yaw);
                    std::cout << "Parsing ref yaw: " << tokens[8] << std::endl;
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid argument: " << e.what() << " for line: " << line << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Out of range: " << e.what() << " for line: " << line << std::endl;
                }
            }
        }
    } else if (param.type == 1) { // result_trj_ekf.csv type
        while (std::getline(file, line)) {
            auto tokens = split(line, ','); // Assuming comma-separated values
            // Skip header row
            if (tokens[0] == "time") {
                continue;
            }
            // Parse tokens into respective fields
            try {
                // Parse time
                double time_value = std::stod(tokens[2]) + std::stod(tokens[3]) / 1e9 + param.tf_time;
                std::cout << "Parsing time: " << time_value << std::endl;
                time.push_back(time_value);

                // Parse position
                x.push_back(std::stod(tokens[5]) + param.tf_x);
                std::cout << "Parsing res x: " << tokens[5] << std::endl;
                y.push_back(std::stod(tokens[6]) + param.tf_y);
                std::cout << "Parsing res y: " << tokens[6] << std::endl;
                z.push_back(std::stod(tokens[7]) + param.tf_z);
                std::cout << "Parsing res z: " << tokens[7] << std::endl;

                // Convert quaternion to euler angles (roll, pitch, yaw)
                Eigen::Quaterniond q(
                    std::stod(tokens[11]), // w
                    std::stod(tokens[8]),  // x
                    std::stod(tokens[9]),  // y
                    std::stod(tokens[10])  // z
                );
                Eigen::Vector3d euler = q.toRotationMatrix().eulerAngles(2, 1, 0);
                roll.push_back(euler[2] + param.tf_roll);
                std::cout << "Parsing res roll: " << euler[2] << std::endl;
                pitch.push_back(euler[1] + param.tf_pitch);
                std::cout << "Parsing res pitch: " << euler[1] << std::endl;
                yaw.push_back(euler[0] + param.tf_yaw);
                std::cout << "Parsing res yaw: " << euler[0] << std::endl;

                // Parse covariance if display_ellipse is enabled
                // if (param.display_ellipse) {
                //     std::vector<double> cov_values;
                //     std::string cov_str = tokens[12].substr(1, tokens[12].size() - 2); // Remove parentheses
                //     auto cov_tokens = split(cov_str, ',');
                //     for (const auto& cov_token : cov_tokens) {
                //         cov_values.push_back(std::stod(cov_token));
                //     }
                //     // Assuming cov_xx, cov_xy, cov_yx, cov_yy are stored in specific indices
                //     covariance_xx.push_back(cov_values[0]);
                //     covariance_xy.push_back(cov_values[1]);
                //     covariance_yx.push_back(cov_values[6]);
                //     covariance_yy.push_back(cov_values[7]);
                // }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: " << e.what() << " for line: " << line << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Out of range: " << e.what() << " for line: " << line << std::endl;
            }
        }
    } else {
        throw std::invalid_argument("Unsupported data type");
    }

    file.close();
}
// void DataPack::readData(const DataParam& param) {
//     std::ifstream file(param.path);
//     if (!file.is_open()) {
//         throw std::runtime_error("Failed to open CSV file: " + param.path);
//     }
//     std::string line;

//     if (param.type == 0) { // reference.csv type
//         int ind=0;
//         while (std::getline(file, line)) {

//             auto tokens = split(line, ','); // Assuming comma-separated values
//             if(ind++<5)
//             {
//                 std::cout<<"references tokens[0]: "<<tokens[0]<<std::endl;
//             }

//             // Skip header row
//             if (tokens[0] == "input->header.seq") {
//                 continue;
//             }
//             // Parse tokens into respective fields
//             if (tokens.size() > std::max({1, 2, 3, 4})) { // Adjust indices as needed
//                 try {
//                     double time_value = std::stod(tokens[1]) / 1e9 + param.tf_time;
//                     // std::cout << "Parsing time: " << time_value << std::endl;
//                     time.push_back(time_value);
//                     x.push_back(std::stod(tokens[2]) + param.tf_x);
//                     // std::cout << "Parsing x: " << tokens[2] << std::endl;
//                     y.push_back(std::stod(tokens[3]) + param.tf_y);
//                     // std::cout << "Parsing y: " << tokens[3] << std::endl;
//                     z.push_back(std::stod(tokens[4]) + param.tf_z);
//                     // std::cout << "Parsing z: " << tokens[4] << std::endl;

//                     if (param.use_quaternion) {
//                         // Handle quaternion conversion to Euler angles
//                     } else {
//                         roll.push_back(std::stod(tokens[5]) + param.tf_roll);
//                         // std::cout << "Parsing roll: " << tokens[5] << std::endl;
//                         pitch.push_back(std::stod(tokens[6]) + param.tf_pitch);
//                         // std::cout << "Parsing pitch: " << tokens[6] << std::endl;
//                         yaw.push_back(std::stod(tokens[7]) + param.tf_yaw);
//                         // std::cout << "Parsing yaw: " << tokens[7] << std::endl;
//                     }
//                 } catch (const std::invalid_argument& e) {
//                     std::cerr << "Invalid argument: " << e.what() << " for line: " << line << std::endl;
//                 } catch (const std::out_of_range& e) {
//                     std::cerr << "Out of range: " << e.what() << " for line: " << line << std::endl;
//                 }
//             }
//         }
//     } else if (param.type == 1) { // result_trj_ekf.csv type
//         int id=0;
//         while (std::getline(file, line)) {

//             auto tokens = split(line, ','); // Assuming comma-separated values
//             // Skip header row
//             if(id++<5)
//             {
//                 std::cout<<"Results tokens[0]: "<<tokens[0]<<std::endl;
//             }
//             if (tokens[0] == "time") {
//                 continue;
//             }
//             // Parse tokens into respective fields
//             try {
//                 double time_value = std::stod(tokens[2]) / 1e9 + param.tf_time;
//                 // std::cout << "Parsing time: " << time_value << std::endl;
//                 time.push_back(time_value);
//                 x.push_back(std::stod(tokens[4]) + param.tf_x);
//                 // std::cout << "Parsing x: " << tokens[4] << std::endl;
//                 y.push_back(std::stod(tokens[5]) + param.tf_y);
//                 // std::cout << "Parsing y: " << tokens[5] << std::endl;
//                 z.push_back(std::stod(tokens[6]) + param.tf_z);
//                 // std::cout << "Parsing z: " << tokens[6] << std::endl;

//                 if (param.use_quaternion) {
//                     // Handle quaternion conversion to Euler angles
//                                     // Convert quaternion to euler angles (roll, pitch, yaw)
//                     Eigen::Quaterniond q(
//                         std::stod(tokens[10]), // w
//                         std::stod(tokens[7]),  // x
//                         std::stod(tokens[8]), // y
//                         std::stod(tokens[9])  // z
//                     );
//                 Eigen::Vector3d euler = q.toRotationMatrix().eulerAngles(2, 1, 0);
//                 roll.push_back(euler[2] + param.tf_roll);
//                 std::cout << "Parsing roll: " << euler[2] << std::endl;
//                 pitch.push_back(euler[1] + param.tf_pitch);
//                 std::cout << "Parsing pitch: " << euler[1] << std::endl;
//                 yaw.push_back(euler[0] + param.tf_yaw);
//                 std::cout << "Parsing yaw: " << euler[0] << std::endl;
//                 } else {
//                     // roll.push_back(std::stod(tokens[7]) + param.tf_roll);
//                     // std::cout << "Parsing roll: " << tokens[7] << std::endl;
//                     // pitch.push_back(std::stod(tokens[8]) + param.tf_pitch);
//                     // std::cout << "Parsing pitch: " << tokens[8] << std::endl;
//                     // yaw.push_back(std::stod(tokens[9]) + param.tf_yaw);
//                     // std::cout << "Parsing yaw: " << tokens[9] << std::endl;
//                                         Eigen::Quaterniond q(
//                         std::stod(tokens[10]), // w
//                         std::stod(tokens[7]),  // x
//                         std::stod(tokens[8]), // y
//                         std::stod(tokens[9])  // z
//                     );
//                     Eigen::Vector3d euler = q.toRotationMatrix().eulerAngles(2, 1, 0);
//                     roll.push_back(euler[2] + param.tf_roll);
//                     std::cout << "Parsing roll: " << euler[2] << std::endl;
//                     pitch.push_back(euler[1] + param.tf_pitch);
//                     std::cout << "Parsing pitch: " << euler[1] << std::endl;
//                     yaw.push_back(euler[0] + param.tf_yaw);
//                     std::cout << "Parsing yaw: " << euler[0] << std::endl;
//                 }

//                 // Parse covariance if display_ellipse is enabled
//                 if (param.display_ellipse) {
//                     std::vector<double> cov_values;
//                     std::istringstream cov_stream(tokens[12].substr(1, tokens[12].size() - 2)); // Remove parentheses
//                     std::string cov_value;
//                     while (std::getline(cov_stream, cov_value, ' ')) {
//                         cov_values.push_back(std::stod(cov_value));
//                     }
//                     // Assuming cov_xx, cov_xy, cov_yx, cov_yy are stored in specific indices
//                     // Adjust these indices based on your actual data structure
//                     // covariance_xx.push_back(cov_values[0]);
//                     // covariance_xy.push_back(cov_values[1]);
//                     // covariance_yx.push_back(cov_values[6]);
//                     // covariance_yy.push_back(cov_values[7]);
//                 }
//             } catch (const std::invalid_argument& e) {
//                 std::cerr << "Invalid argument: " << e.what() << " for line: " << line << std::endl;
//             } catch (const std::out_of_range& e) {
//                 std::cerr << "Out of range: " << e.what() << " for line: " << line << std::endl;
//             }
//         }
//     } else {
//         throw std::invalid_argument("Unsupported data type");
//     }

//     file.close();
// }

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
}

void ResDataPack::syncWithReference(const DataPack& refPack) {
    if (refPack.time.empty() || time.empty()) {
        throw std::runtime_error("Reference data or result data is empty.");
    }

    // Sync time with reference
    double ref_start_time = refPack.time.front();
    double res_start_time = time.front();
    double time_offset = ref_start_time - res_start_time;

    for (auto& t : time) {
        t += time_offset;
    }

    // Optionally sync spatial data based on timestamps
    // This part assumes that the data points are temporally aligned after time sync

    size_t ref_index = 0;
    for (size_t i = 0; i < time.size(); ++i) {
        while (ref_index < refPack.time.size() - 1 && refPack.time[ref_index + 1] < time[i]) {
            ++ref_index;
        }

        if (ref_index >= refPack.time.size() - 1) {
            break;
        }

        // Linear interpolation for position and orientation based on the nearest timestamps
        double t0 = refPack.time[ref_index];
        double t1 = refPack.time[ref_index + 1];
        double ratio = (time[i] - t0) / (t1 - t0);

        x[i] = refPack.x[ref_index] + ratio * (refPack.x[ref_index + 1] - refPack.x[ref_index]);
        y[i] = refPack.y[ref_index] + ratio * (refPack.y[ref_index + 1] - refPack.y[ref_index]);
        z[i] = refPack.z[ref_index] + ratio * (refPack.z[ref_index + 1] - refPack.z[ref_index]);

        roll[i] = refPack.roll[ref_index] + ratio * (refPack.roll[ref_index + 1] - refPack.roll[ref_index]);
        pitch[i] = refPack.pitch[ref_index] + ratio * (refPack.pitch[ref_index + 1] - refPack.pitch[ref_index]);
        yaw[i] = refPack.yaw[ref_index] + ratio * (refPack.yaw[ref_index + 1] - refPack.yaw[ref_index]);
    }
}

std::pair<DataPack, std::vector<ResDataPack>> Packer::param2pack(const DataParam& refParam, const std::vector<DataParam>& resParams) {
    DataPack refPack(refParam);
    std::vector<ResDataPack> resPacks;
    for (const auto& resParam : resParams) {
        resPacks.emplace_back(resParam, refPack);
    }
    return {refPack, resPacks};
}

