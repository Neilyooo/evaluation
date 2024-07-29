#ifndef PACKER_H
#define PACKER_H

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include "configer.h"


class DataPack {
public:
    // std::vector<long double> time;
    std::vector<double> x, y, z, roll, pitch, yaw,time;
    std::string label;
    bool display_ellipse;
    std::unordered_map<std::string, std::vector<double>> data;

    DataPack(const DataParam& param);
    void readData(const DataParam& param);
    void adjustTimeStamps(const DataParam& param);

    static std::vector<std::string> split(const std::string& s, char delimiter);
    std::vector<double> covariance_xx, covariance_xy, covariance_yx, covariance_yy;
};

class ResDataPack : public DataPack {
public:
    ResDataPack(const DataParam& param, const DataPack& refPack) ;

    void syncWithReference(const DataPack& refPack);
    void calc_error(const DataPack& refPack, ResDataPack& resPack);
    // void adjustStartTime(DataPack& refPack);
    // void adjustEndTime(DataPack& refPack);
};

class Packer {
public:
    static std::pair<DataPack, std::vector<ResDataPack>> param2pack(const DataParam& refParam, const std::vector<DataParam>& resParams);

};
#endif // PACKER_H
