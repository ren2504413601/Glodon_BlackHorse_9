//一些全局变量
#pragma once
#include <string>
using namespace std;

typedef int ErrorStatus;
enum { SUCCEED, ERROR, ERROR_OPENFILE, ERROR_WRITEFILE };
//const string GLOBAL_OUTPUT_DIR = "F:\\任磊旧电脑\\study\\deep learning\\比赛\\广联达黑马赛\\414522_792851_bundle_archive\\Results\\";
//const string GLOBAL_ORIGIN_JPG_DIR = "F:\\任磊旧电脑\\study\\deep learning\\比赛\\广联达黑马赛\\414522_792851_bundle_archive\\001.jpg";
//const string GLOBAL_GROUDTRUTH_MAT_FILE = "F:\\任磊旧电脑\\study\\deep learning\\比赛\\广联达黑马赛\\414522_792851_bundle_archive\\001.mat";
const string GLOBAL_OUTPUT_DIR = "../../../../../Results/";
const string GLOBAL_ORIGIN_JPG_DIR = "../../../../../SrcFigures/001.jpg";
const string GLOBAL_GROUDTRUTH_MAT_FILE = "../../../../../SrcFigures/001.mat";
const double GLOBAL_CHANGE_RATE = 0.2;