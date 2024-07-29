#ifndef PLOTTER_H
#define PLOTTER_H

#include <vector>
#include <string>
#include "packer.h"  // 包含 DataPack 和 DataParam 的定义
#include <iomanip>
class Plotter {
public:
    static void plot(const DataPack& refPack, const std::vector<ResDataPack>& resPacks, const OptParam& optParam);
    static void save(const DataPack& refPack, const std::vector<ResDataPack>& resPacks, const OptParam& optParam);
    static void save_dataframe(const DataPack& refPack, const std::vector<ResDataPack>& resPacks, const OptParam& optParam);
    static void plot_2d_traj(const DataPack& refPack, const std::vector<ResDataPack>& resPacks, const OptParam& optParam);
    static void plot_lateral_error(const DataPack& refPack, const std::vector<ResDataPack>& resPacks, const OptParam& optParam);
    static void plot_longitudinal_error(const DataPack& refPack, const std::vector<ResDataPack>& resPacks, const OptParam& optParam);
    static void save_df(const DataPack& refPack, const std::vector<ResDataPack>& resPacks, const OptParam& optParam);

};

#endif // PLOTTER_H
