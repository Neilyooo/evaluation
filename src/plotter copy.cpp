#include "plotter.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

void Plotter::plot(const DataPack& refPack, const std::vector<ResDataPack>& resPacks, const OptParam& optParam) {
    plt::figure_size(optParam.fig_width, optParam.fig_height);

    // 绘制参考数据
    plt::named_plot("Reference", refPack.time, refPack.x, "b-");

    // 绘制结果数据
    for (const auto& resPack : resPacks) {
        plt::named_plot(resPack.label, resPack.time, resPack.x, "r-");
    }

    // 添加标题和标签
    plt::title(optParam.title);
    plt::xlabel(optParam.xlabel);
    plt::ylabel(optParam.ylabel);

    // 显示图形
    plt::legend();
    plt::show();
}

void Plotter::save(const DataPack& refPack, const std::vector<ResDataPack>& resPacks, const OptParam& optParam) {
    plt::figure_size(optParam.fig_width, optParam.fig_height);

    // 绘制参考数据
    plt::named_plot("Reference", refPack.time, refPack.x, "b-");

    // 绘制结果数据
    for (const auto& resPack : resPacks) {
        plt::named_plot(resPack.label, resPack.time, resPack.x, "r-");
    }

    // 添加标题和标签
    plt::title(optParam.title);
    plt::xlabel(optParam.xlabel);
    plt::ylabel(optParam.ylabel);

    // 保存图形
    plt::legend();
    plt::save(optParam.output_directory + "/" + optParam.filename);
}
