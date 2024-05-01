# trajcSimilar

## introduction

## run the demo


You can get a result by going to the trajcSimilar folder and running the following command.
```shell
./run_demo.sh
```




## EDR/DTW non-continue with length limit

file : /data/wangfei/trajc/trajcSimilar/EDRTrajSim.cpp

### 数据处理格式
这里处理的是T_Drive的数据
处理函数入口： file2paths_t_drive
涉及到的函数 : file2paths_save->file2paths_into_vector


处理后最终的数据格式：
```
1709
0 39.94727 116.36650
1 39.94727 116.36650
2 39.94413 116.35599
3 39.93452 116.34951
4 39.92101 116.35014
5 39.89943 116.35058
6 39.89535 116.32338
7 39.89593 116.31605
8 39.89892 116.31560
9 39.89178 116.31465
```
第一个数字n是轨迹长度，然后接下来n行对应每个轨迹点的坐标。

### useage
main function : 
EDR：
void FastEDRConstraintLen(path data, path query, double eps, int min_len);

DTW：
void FastDTWConstraintLen(path data, path query, vector<int> min_len, int idx);


#### 执行逻辑
在main函数中调用
T-Drive的数据已经处理好放在data目录下： /data/wangfei/trajc/data/release/trajctory/tra_2024_4_5
// t_drive : all file number is : 10357
取数据的时候直接调用 vector<path> paths = readfileWithLimit(datasource, 2);

取出来数据后就可以直接调用EDR或者DTW的执行函数了。

#### 输出索引保存为文件
路径： /data/wangfei/trajc/trajcSimilar/constant.h filepath
filepath + "/idx/trajc_idx_" + to_string(idx) + ".txt";

格式：
len
idx1, idx2, idx3 ...
```
0.004
0,396,1133,1396,1475,1708,
0.002
0,1133,1708,
0.006
0,172,371,445,619,1133,1355,1410,1475,1708,
```

#### data 和 query 计算距离 DTW
void calc_trajc_dtw(vector<path> paths, int query_size);
并保存
query size代表前多少个path是query。

#### 计算Frechet
double Discret_Frechet(path path1, path path2, int start, int end)；

#### Frechet 的限长非连续子序列。
void FastFREConstraintLen(path data, path query, vector<double> min_len, int idx);


#### 注意的点
1. EDR需要设置eps
2. 正式执行前把所有日志输出都注释掉，特别是print_element
