#include "plotter.h"
#include "matplotlibcpp.h"
#include <fstream>

namespace plt = matplotlibcpp;
std::map<std::string, std::string> color_map = {
    {"EKF", "orange"},
    {"NDT", "blue"},
    {"reference", "black"}
};
void Plotter::plot(const DataPack& refPack, const std::vector<ResDataPack>& resPacks, const OptParam& optParam) {
    plot_2d_traj(refPack, resPacks,optParam);
    std::cout<<"saving 2d error png"<<std::endl;
    plot_longitudinal_error(refPack, resPacks,optParam);
    std::cout<<"saving longitudinal error png"<<std::endl;
    plot_lateral_error(refPack, resPacks,optParam);
    std::cout<<"saving lateral error png"<<std::endl;
}
void Plotter::plot_2d_traj(const DataPack& refPack, const std::vector<ResDataPack>& resPacks, const OptParam& optParam) {
    plt::figure();
    
    // 设置标题及其字体大小
    plt::title("2D Trajectory", {{"fontsize", std::to_string(optParam.title_font_size)}});

    // 设置x轴标签及其字体大小
    plt::xlabel("x[m]", {{"fontsize", std::to_string(optParam.label_font_size)}});

    // 设置y轴标签及其字体大小
    plt::ylabel("y[m]", {{"fontsize", std::to_string(optParam.label_font_size)}});

    // 绘制参考数据
    if (!refPack.x.empty() && !refPack.y.empty()) {
        plt::named_plot("Reference", refPack.x, refPack.y, "k:");
        plt::scatter(refPack.x, refPack.y, 1.0, {{"c", "black"}, {"marker", "o"}});
    } else {
        std::cerr << "Reference data is empty!" << std::endl;
    }

    // 绘制结果数据
    for(const auto& resPack : resPacks)
    {
        std::cout<<"label: "<<resPack.label<<" 2d traj timesize: "<<resPack.time.size()<<", x size:"<<resPack.x.size()<<", y size:"<<resPack.y.size()<<std::endl;
        if (!resPack.x.empty() && !resPack.y.empty()) {
            plt::named_plot(resPack.label, resPack.x, resPack.y, "y-");
            plt::scatter(resPack.x, resPack.y, 1.0, {{"c", "y"}, {"marker", "o"}});
        } else {
            std::cerr << "Result data for " << resPack.label << " is empty!" << std::endl;
        }
    }
    

    plt::legend();
    plt::grid(true);
    plt::save(optParam.output_directory + "/2d_trajectory." + optParam.save_extension_type);
}
void Plotter::save_df(const DataPack& refPack, const std::vector<ResDataPack>& resPacks, const OptParam& optParam) {
    // Save reference dataframe
    std::ofstream ref_outfile(optParam.output_directory + "/sync_" + "Reference" + "_df.csv");
    ref_outfile << "time,x,y,z,yaw\n";
    ref_outfile << std::fixed << std::setprecision(15);
    for (size_t i = 0; i < refPack.time.size(); ++i) {
        ref_outfile << refPack.time[i]<<"," 
                    << refPack.x[i] << ","
                    << refPack.y[i] << ","
                    << refPack.z[i] << ","
                    << refPack.yaw[i] << "\n";
    }
    ref_outfile.close();
    // Save result dataframes
    for (const auto& resPack : resPacks) {
        std::ofstream res_outfile(optParam.output_directory + "/sync_" + resPack.label + "_df.csv");
        res_outfile << "time,x,y,z,yaw,error_x,error_y,error_z,error_longitudinal,error_lateral\n";
        res_outfile << std::fixed << std::setprecision(15);
        for (size_t i = 0; i < resPack.time.size(); ++i) {
            res_outfile << resPack.time[i] <<"," 
                        << resPack.x[i] << ","
                        << resPack.y[i] << ","
                        << resPack.z[i] << ","
                        << resPack.data.at("error_x")[i] << "," 
                        << resPack.data.at("error_y")[i] << "," 
                        << resPack.data.at("error_z")[i] << "," 
                        << resPack.data.at("error_longitudinal")[i] << "," 
                        << resPack.data.at("error_lateral")[i] << "\n";
        }
        res_outfile.close();
    }
}

void Plotter::plot_longitudinal_error(const DataPack& refPack, const std::vector<ResDataPack>& resPacks, const OptParam& optParam) {
    plt::figure_size(20, 12);
    plt::figure();
    plt::title("Longitudinal Error", {{"fontsize", std::to_string(optParam.title_font_size)}});
    plt::xlabel("time[s]", {{"fontsize", std::to_string(optParam.label_font_size)}});
    plt::ylabel("error[m]", {{"fontsize", std::to_string(optParam.label_font_size)}});

    for (const auto& resPack : resPacks) {
        std::string color = "gray"; // 默认颜色
        if (color_map.find(resPack.label) != color_map.end()) {
            color = color_map[resPack.label];
        }
        if (!resPack.time.empty() && resPack.data.find("error_longitudinal") != resPack.data.end() && !resPack.data.at("error_longitudinal").empty()) {//
            std::cout << "Plotting longitudinal error for " << resPack.label << std::endl;
            std::cout << "Time size: " << resPack.time.size() << ", Error size: " << resPack.data.at("error_longitudinal").size() << "\n"<<std::endl;


            std::vector<double> error_longitudinal = resPack.data.at("error_longitudinal");
            std::vector<double> common_time(error_longitudinal.size());
            std::iota(common_time.begin(), common_time.end(), 0); // 生成从0开始的索引

            plt::named_plot(resPack.label, common_time, error_longitudinal);
            plt::scatter(common_time, error_longitudinal, 2.0, {{"c", color}, {"marker", "o"}});
        } else {
            std::cerr << "Result data for " << resPack.label << " is empty or does not contain longitudinal error!" << std::endl;
        }
    }

    plt::legend();
    plt::grid(true);
    plt::save(optParam.output_directory + "/longitudinal_error." + optParam.save_extension_type);
}

void Plotter::plot_lateral_error(const DataPack& refPack, const std::vector<ResDataPack>& resPacks, const OptParam& optParam) {
    plt::figure_size(20, 12);
    plt::figure();
    plt::title("Lateral Error", {{"fontsize", std::to_string(optParam.title_font_size)}});
    plt::xlabel("time[s]", {{"fontsize", std::to_string(optParam.label_font_size)}});
    plt::ylabel("error[m]", {{"fontsize", std::to_string(optParam.label_font_size)}});

    for (const auto& resPack : resPacks) {
        std::string color = "gray"; // 默认颜色
        if (color_map.find(resPack.label) != color_map.end()) {
            color = color_map[resPack.label];
        }
        if (!resPack.time.empty() && resPack.data.find("error_lateral") != resPack.data.end() && !resPack.data.at("error_lateral").empty()) {//
            std::cout << "Plotting lateral error for " << resPack.label << std::endl;
            std::cout << "Time size: " << resPack.time.size() << ", Error size: " << resPack.data.at("error_lateral").size() <<"\n"<<std::endl;

            std::vector<double> error_lateral = resPack.data.at("error_lateral");
            std::vector<double> common_time(error_lateral.size());
            std::iota(common_time.begin(), common_time.end(), 0); // 生成从0开始的索引

            plt::named_plot(resPack.label, common_time, error_lateral);
            plt::scatter(common_time, error_lateral, 2.0, {{"c", color}, {"marker", "o"}});
        } else {
            std::cerr << "Result data for " << resPack.label << " is empty or does not contain lateral error!" << std::endl;
        }
    }

    plt::legend();
    plt::grid(true);
    plt::save(optParam.output_directory + "/lateral_error." + optParam.save_extension_type);
}
