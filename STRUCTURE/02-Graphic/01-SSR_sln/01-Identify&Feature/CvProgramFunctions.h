#pragma once
#include "GlobalParams.h"
#include <math.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "CvGeFunctions.h"
#include <vector>
#include <stack>
#include <map>

class CvProgramFunctions //程序运行所需的各类程序
{
private:
	bool comparePoint(Point pt1, Point pt2);//两个Point的自定义排序比较函数，map用
public:

	static ErrorStatus showCracks(vector<vector<Point>>& vecCracks, vector<CrackInfo>& crackInfos); //输出裂缝图像[07-09]
	static ErrorStatus getCrackLengthByRatio(vector<vector<Point>>& vecCracks, multimap<double, Point>& mapLength_Pos, double changeRatio);//输出裂缝长度[07-10]
	static double getCrackArea(vector<CrackInfo>& crackInfos, double changeRatio);
	static vector<double> getCrackLegth(vector<CrackInfo>& crackInfos, double changeRatio);
	static double getCrackMaxLength(vector<CrackInfo>& crackInfos, double changeRatio);
	static vector<double> getCrackWidth(vector<CrackInfo>& crackInfos, double changeRatio);
	static double getCrackMeanWidth(vector<CrackInfo>& crackInfos, double changeRatio);
	static vector<double> getGeoFeatures(vector<CrackInfo>& crackInfos, double changeRatio);
};