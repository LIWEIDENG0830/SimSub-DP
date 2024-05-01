#include <iostream>
#include "TrajSim.h"
using namespace std;

void dtw_csimsub_exec_example()
{
   // dtw ours
   // geofile
   cout<<"dtw ours"<<endl;
   static const char *geofile = "/data1/dlw/trajectory/SimSub-general/ExpComp/data/geolife/traj_all_1000_2500";
   static const char *geofile_save = "./traj_all_1000_2500_geofile_dtw_save";
   vector<double> comp_ratios_1 = {0.005};
   cout<<"geofile"<<endl;
   dtw_main_exec(geofile, "", geofile_save, 10, 20, comp_ratios_1);

   // porto
   static const char *portofile = "/data1/dlw/trajectory/SimSub-general/ExpComp/data/porto/traj_all_20_400";
   static const char *portofile_save = "./traj_all_20_400_porto_save";
   vector<double> comp_ratios_2 = {0.3};
   cout<<"porto"<<endl;
   dtw_main_exec(portofile, "", portofile_save, 10, 20, comp_ratios_2);

   // chengdu
   static const char *chengdufile = "/data1/dlw/trajectory/SimSub-general/ExpComp/data/chengdu/traj_all_20_400";
   static const char *chengdufile_save = "./traj_all_20_400_chengdu_save";
   cout<<"chengdu"<<endl;
   dtw_main_exec(chengdufile, "", chengdufile_save, 10, 20, comp_ratios_2);
}

void FastFREConstraintLen_exec_example()
{
   string str_path = "/data1/dlw/trajectory/SimSub-general/ExpComp/data/geolife/traj_all_1000_2500";
   vector<path> paths = readfileWithLimit(str_path, 1);
   vector<double> len;
   len.push_back(0.004);
   FastFREConstraintLen(paths[0], paths[0], len, 0);
}

int main() {
   // 数据处理
   // file2paths_t_drive("", "", -1);

   FastFREConstraintLen_exec_example();

   // dtw_csimsub_exec_example();

   return 0;
}
