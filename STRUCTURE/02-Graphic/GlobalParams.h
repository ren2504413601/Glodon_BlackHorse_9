//一些全局变量
#pragma once
#include <string>
using namespace std;

typedef int ErrorStatus;
enum { SUCCEED, ERROR, ERROR_OPENFILE, ERROR_WRITEFILE };
const string GLOBAL_OUTPUT_DIR = "../../Results/";
const string GLOBAL_ORIGIN_JPG_DIR = "../../SrcFigures/001.jpg";
const string GLOBAL_GROUDTRUTH_MAT_FILE = "../../SrcFigures/001.mat";
const double GLOBAL_CHANGE_RATE = 0.2;