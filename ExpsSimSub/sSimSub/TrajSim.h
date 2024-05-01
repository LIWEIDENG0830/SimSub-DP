//
// create by wangfei on 2024/5/1.
//

#include "utils.h"
#include <unordered_map>

using namespace std;

// DTW
void calc_dis_matrix(path data, path query, vector<vector<double> > &dis_mat);
void FastDTWConstraintLen(path data, path query, vector<double> min_len, vector<double> &time_save, int idx, vector<vector<double> > &dis_mat);
void get_dtw_trajc_idx(vector<vector<vector<double> > > &Edr_List, vector<int> &Edr_Idx, int min_len);
void FastDTWSub(path data, path query, vector<vector<double> > &dp, vector<vector<double> > &p_dtw, int min_len);

void calc_trajc_dtw(vector<path> paths, int query_size);

void dtw_main_exec(string data_path, 
                string data_path_pkl, 
                string save_path,
                int n_query, 
                int n_data,
                vector<double> &comp_ratios);
void btr_exec_dtw(vector<path> &trajs, 
                unordered_map<double, vector<path>> &comp_trajs,
                vector<double> &comp_ratios);
// EDR
double EDR(path data, path query, int min_len, vector<vector<double>> &p_dtw, double eps);

// Fre
void FastFreSub(path data, path query, vector<vector<double> > &dp, vector<vector<double> > &p_fre, int min_len);
void get_fre_trajc_idx(vector<vector<vector<double> > > &Fre_List, vector<int> &Fre_Idx, int min_len);
void FastFREConstraintLen(path data, path query, vector<double> min_len, int idx);
