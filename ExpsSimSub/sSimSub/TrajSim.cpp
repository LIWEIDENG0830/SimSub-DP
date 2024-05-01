//
// create by wangfei on 2024/5/1.
//

#include "TrajSim.h"

// DTW
void FastDTWSub(path data, path query, vector<vector<double> > &dp, vector<vector<double> > &p_dtw, int min_len, vector<vector<double> > &dis_mat) 
{
    int m = data.size(), n = query.size();
    dp.resize(n, vector<double>(m, 0));
    // initDp(dp, n, m);
    int choices = 0;
    double subCost = 0;
    if (min_len == 1) {
        for (int j = 0; j < n; j++) {
            subCost = dis_mat[j][0]; //pointDistance(query[j], data[0]);
            if (j == 0) {
                dp[j][0] = subCost;
            } else {
                dp[j][0] = dp[j-1][0] + subCost;
            }
        }
    } else if (min_len == 2) {
        for (int j = min_len - 1; j < m; j++) {
            dp[0][j] = p_dtw[0][0] + dis_mat[0][j]; //pointDistance(query[0], data[j]);
        }
        double min_val = INT64_MAX;
        for (int i = 1; i < n; i++) {
            min_val = min(p_dtw[i-1][0], p_dtw[i][0]);
            for (int j = min_len - 1; j < m; j++) {
                subCost = dis_mat[i][j]; //pointDistance(query[i], data[j]);
                if (min_val > dp[i-1][j]) {
                    dp[i][j] = dp[i-1][j] + subCost;
                } else {
                    dp[i][j] = min_val + subCost;
                }
            }
        }
    } else {
        double min_val = INT64_MAX;
        // first
        for (int i = min_len - 1; i < m; i++) {
            if (min_val > p_dtw[0][i - 1]) {
                min_val = p_dtw[0][i - 1];
            }
            dp[0][i] = min_val + dis_mat[0][i];// pointDistance(query[0], data[i]);
        }
        // other row : 2 - n
        min_val = INT64_MAX;
        for (int i = 1; i < n; i++) {
            min_val = INT64_MAX;
            for (int j = min_len - 1; j < m; j++) {
                if (min_val > p_dtw[i-1][j-1]) {
                    min_val = p_dtw[i-1][j-1];
                }
                if (min_val > p_dtw[i][j-1]) {
                    min_val = p_dtw[i][j-1];
                }
                //
                if (min_val > dp[i-1][j]) {
                    dp[i][j] = dp[i-1][j] + dis_mat[i][j];//pointDistance(query[i], data[j]);
                } else {
                    dp[i][j] = min_val + dis_mat[i][j];// pointDistance(query[i], data[j]);
                }
            }
        }
    }
}

void get_dtw_trajc_idx(vector<vector<vector<double> > > &Dtw_List, vector<int> &Dtw_idx, int min_len)
{
    int pos_n = Dtw_List[0].size() - 1, pos_m = Dtw_List[0][0].size() - 1;
    int pos = -1, flag = -1;
    double min_val = INT64_MAX;
    for (int i = min_len - 1; i >= 0; i--) {
        Dtw_idx.push_back(pos_m);
        if (i == 0) {
        } else if (i == 1) {
            pos_m = 0;
        } else {
            min_val = INT64_MAX;
            flag = -1;
            for (int j = i - 1; j < pos_m; j++) {
                if (pos_n > 0 && min_val > Dtw_List[i-1][pos_n - 1][j]) {
                    min_val = Dtw_List[i-1][pos_n - 1][j];
                    pos = j;
                    flag = 1;
                }
                if (min_val > Dtw_List[i-1][pos_n][j]) {
                    min_val = Dtw_List[i-1][pos_n][j];
                    pos = j;
                    flag = 0;
                }
            }
            if (pos_n > 0 && min_val > Dtw_List[i][pos_n - 1][pos_m]) {
                min_val = Dtw_List[i][pos_n - 1][pos_m];
                pos = pos_m;
                flag = 1;
            }
            while (pos_m == pos) 
            {
                pos_n --;
                min_val = INT64_MAX;
                for (int j = i - 1; j < pos_m; j++) {
                    if (pos_n > 0 && min_val > Dtw_List[i-1][pos_n - 1][j]) {
                        min_val = Dtw_List[i-1][pos_n - 1][j];
                        pos = j;
                        flag = 1;
                    }
                    if (min_val > Dtw_List[i-1][pos_n][j]) {
                        min_val = Dtw_List[i-1][pos_n][j];
                        pos = j;
                        flag = 0;
                    }
                }
                if (pos_n > 0 && min_val > Dtw_List[i][pos_n - 1][pos_m]) {
                    min_val = Dtw_List[i][pos_n - 1][pos_m];
                    pos = pos_m;
                    flag = 1;
                }
            }
            pos_m = pos;
            if (flag == 1) {
                pos_n --;
            }
            
        }
    }
    reverse(Dtw_idx.begin(), Dtw_idx.end());
}

void save_node_into_file(ofstream &ofs, vector<int>& idx_res) {
    for (int idx : idx_res) {
        ofs << idx << ",";
    }
    ofs << endl;
}

void print_element1(vector<vector<double> > &List)
{
    int n = List.size(), m = List[0].size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout<<i<<"-"<<j<<" : "<<List[i][j]<<"; ";
        }
        cout<<endl;
    }
}

void dtw_main_exec(string data_path, 
                string data_path_pkl, 
                string save_path,
                int n_query, 
                int n_data,
                vector<double> &comp_ratios)
{
    clock_t start_time = clock();
    // read data
    vector<path> trajs = readfileWithLimit(data_path, (n_query + n_data));
    clock_t end_time = clock();
    cout<<"load data time cost is : "<<end_time - start_time<<" ms "<<endl;

    unordered_map<double, vector<path>> comp_trajs;
    btr_exec_dtw(trajs, comp_trajs, comp_ratios);

    // save path
}

void btr_exec_dtw(vector<path> &trajs, 
                unordered_map<double, vector<path>> &comp_trajs,
                vector<double> &comp_ratios)
{
    vector<double> min_len;
    vector<double> time_ratio(comp_ratios.size(), 0);
    vector<vector<double> > dis_mat;
    for (path traj : trajs) {
        int n = traj.size();
        // cout<<n<<endl;
        dis_mat.clear();
        dis_mat.resize(n, vector<double>(n, 0));
        calc_dis_matrix(traj, traj, dis_mat);
        min_len.clear();
        for (int i = 0; i < comp_ratios.size(); i++) {
            int budget = int(traj.size() * comp_ratios[i]);
            if (budget < 2)
                budget = 2;
            min_len.push_back(budget);
        }
        FastDTWConstraintLen(traj, traj, min_len, time_ratio, 0, dis_mat);
    }
    for (int i = 0;i < time_ratio.size(); i++) {
        cout <<"Comp  Ratio : "<<comp_ratios[i]<<" time is : "<< time_ratio[i]<<endl;
    }
}

void calc_dis_matrix(path data, path query, vector<vector<double> > &dis_mat)
{
    int n = query.size();
    int m = data.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
        {
            dis_mat[i][j] = pointDistance(query[i], data[j]);
        }
    }
}

void FastDTWConstraintLen(path data, path query, vector<double> min_len, vector<double> &time_save, int idx, vector<vector<double> > &dis_mat)
{
    vector<vector<vector<double> > > DTW_List;
    double len = 0;
    struct timeval start_t;
    struct timeval end_t;

    for (double l : min_len) {
        len = max(l, len);
    }
    for (int i = 1; i <= len; i++) {
        vector<vector<double> > Dtw_Dp;
        if (i == 1) {
            FastDTWSub(data, query, Dtw_Dp, Dtw_Dp, i, dis_mat);
        } else {
            FastDTWSub(data, query, Dtw_Dp, DTW_List[DTW_List.size() - 1], i, dis_mat);
        }
        DTW_List.push_back(Dtw_Dp);
        }
    string file_name;
    file_name = filepath + "/idx/trajc_idx_" + to_string(idx) + ".txt";
    ofstream ofs(file_name, ios::app);
    for (int k = 0; k < min_len.size(); k++) {
        vector<int> t_v;
        get_dtw_trajc_idx(DTW_List, t_v, min_len[k]);
        ofs<<min_len[k]<<endl;
        save_node_into_file(ofs, t_v);
    }
    ofs.close();
}

// calc query and data dtw
void calc_trajc_dtw(vector<path> paths, int query_size)
{
    int len = paths.size();
    string file_name = filepath + "/res/trajc_dtw_dis_"+to_string(query_size)+".txt";
    ofstream ofs(file_name);
    vector<vector<double> > res(query_size, vector<double>(len - query_size));
    for (int i = 0; i < query_size; i++) {
        for (int j = query_size; j < len; j++) {
            double res = dtwDistance(paths[i], paths[j], 0, paths[j].size());
            ofs<<res<<",";
        }
        ofs<<endl;
    }
    ofs.close();
}

// discret frechet
void FastFreSub(path data, path query, vector<vector<double> > &dp, vector<vector<double> > &p_fre, int min_len) 
{
    int m = data.size(), n = query.size();
    dp.resize(n, vector<double>(m, 0));
    // initDp(dp, n, m);
    int choices = 0;
    double subCost = 0;
    if (min_len == 1) {
        for (int j = 0; j < n; j++) {
            subCost = pointDistance(query[j], data[0]);
            if (j == 0) {
                dp[j][0] = subCost;
            } else {
                dp[j][0] = max(dp[j-1][0], subCost);
            }
        }
    } else if (min_len == 2) {
        for (int j = min_len - 1; j < m; j++) {
            dp[0][j] = max(p_fre[0][0], pointDistance(query[0], data[j]));
        }
        double min_val = INT64_MAX;
        for (int i = 1; i < n; i++) {
            min_val = min(p_fre[i-1][0], p_fre[i][0]);
            for (int j = min_len - 1; j < m; j++) {
                subCost = pointDistance(query[i], data[j]);
                if (min_val > dp[i-1][j]) {
                    dp[i][j] = max(dp[i-1][j], subCost);
                } else {
                    dp[i][j] = max(min_val, subCost);
                }
            }
        }
    } else {
        double min_val = INT64_MAX;
        // first
        for (int i = min_len - 1; i < m; i++) {
            if (min_val > p_fre[0][i - 1]) {
                min_val = p_fre[0][i - 1];
            }
            dp[0][i] = min_val + pointDistance(query[0], data[i]);
        }
        // other row : 2 - n
        min_val = INT64_MAX;
        for (int i = 1; i < n; i++) {
            min_val = INT64_MAX;
            for (int j = min_len - 1; j < m; j++) {
                if (min_val > p_fre[i-1][j-1]) {
                    min_val = p_fre[i-1][j-1];
                }
                if (min_val > p_fre[i][j-1]) {
                    min_val = p_fre[i][j-1];
                }
                //
                if (min_val > dp[i-1][j]) {
                    dp[i][j] = max(dp[i-1][j], pointDistance(query[i], data[j]));
                } else {
                    dp[i][j] = max(min_val, pointDistance(query[i], data[j]));
                }
            }
        }
    }
}

void get_fre_trajc_idx(vector<vector<vector<double> > > &Fre_List, vector<int> &Fre_Idx, int min_len)
{
    int pos_n = Fre_List[0].size() - 1, pos_m = Fre_List[0][0].size() - 1;
    int pos = -1, flag = -1;
    double min_val = INT64_MAX;
    for (int i = min_len - 1; i >= 0; i--) {
        Fre_Idx.push_back(pos_m);
        if (i == 0) {
        } else if (i == 1) {
            pos_m = 0;
        } else {
            min_val = INT64_MAX;
            flag = -1;
            for (int j = i - 1; j < pos_m; j++) {
                if (pos_n > 0 && min_val > Fre_List[i-1][pos_n - 1][j]) {
                    min_val = Fre_List[i-1][pos_n - 1][j];
                    pos = j;
                    flag = 1;
                }
                if (min_val > Fre_List[i-1][pos_n][j]) {
                    min_val = Fre_List[i-1][pos_n][j];
                    pos = j;
                    flag = 0;
                }
            }
            if (pos_n > 0 && min_val > Fre_List[i][pos_n - 1][pos_m]) {
                min_val = Fre_List[i][pos_n - 1][pos_m];
                pos = pos_m;
                flag = 1;
            }
            while (pos_m == pos) 
            {
                pos_n --;
                min_val = INT64_MAX;
                for (int j = i - 1; j < pos_m; j++) {
                    if (pos_n > 0 && min_val > Fre_List[i-1][pos_n - 1][j]) {
                        min_val = Fre_List[i-1][pos_n - 1][j];
                        pos = j;
                        flag = 1;
                    }
                    if (min_val > Fre_List[i-1][pos_n][j]) {
                        min_val = Fre_List[i-1][pos_n][j];
                        pos = j;
                        flag = 0;
                    }
                }
                if (pos_n > 0 && min_val > Fre_List[i][pos_n - 1][pos_m]) {
                    min_val = Fre_List[i][pos_n - 1][pos_m];
                    pos = pos_m;
                    flag = 1;
                }
            }
            pos_m = pos;
            if (flag == 1) {
                pos_n --;
            }
            
        }
    }
    reverse(Fre_Idx.begin(), Fre_Idx.end());
}

void FastFREConstraintLen(path data, path query, vector<double> min_len, int idx)
{
    vector<vector<vector<double> > > Fre_List;
    double len = 0;
    for (double l : min_len) {
        len = max(l, len);
    }
    int i_len = len*data.size();
    for (int i = 1; i <= i_len; i++) {
        vector<vector<double> > Fre_Dp;
        if (i == 1) {
            FastFreSub(data, query, Fre_Dp, Fre_Dp, i);
        } else {
            FastFreSub(data, query, Fre_Dp, Fre_List[Fre_List.size() - 1], i);
        }
        cout<<"len : "<<i<<endl;
        cout<<Fre_Dp[Fre_Dp.size() - 1][Fre_Dp[0].size() - 1]<<endl;
        Fre_List.push_back(Fre_Dp);
    }
    // get idx and save file
    string file_name;
    file_name = filepath + "/idx/trajc_fre_idx_" + to_string(idx) + ".txt";
    ofstream ofs(file_name);
    for (double k : min_len) {
        vector<int> t_v;
        get_fre_trajc_idx(Fre_List, t_v, k*data.size());
        ofs<<k<<endl;
        save_node_into_file(ofs, t_v);
    }
    ofs.close();
}

