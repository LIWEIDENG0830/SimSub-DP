//
// create by wangfei on 2024/5/1.
//

#ifndef TRAJCSIMILAR_CONSTANT_H
#define TRAJCSIMILAR_CONSTANT_H
#include "distance.h"
#include "map"
using namespace std;
typedef pair<double,double> point;
typedef vector<point> path;
typedef pair<pair<int, int>, double> subResult;

static map<string , pair<vector<double>, vector<double>>> range{
    {"xian", {{34.20, 34.29}, {108.91, 109.00}}},
    {"chengdu", {{30.65, 30.73}, {104.04, 104.13}}},
    {"porto", {{41.11, 41.19}, {-8.67, -8.57}}}
};
static bool generateResult = false;

static double MaxSimilar = 100000000000;

static string filepath = "/data/wangfei/trajc/data/result/";
static const char *datasource = "/data/wangfei/trajc/data/release/trajctory/tra_2024_4_5";


static pair<double, double> nullPoint = pair<double, double>{-1000,-1000};
static pair<double, double> centerPoint = pair<double, double>{34.44, 109.95}; // 西安
#endif //TRAJCSIMILAR_CONSTANT_H
