#pragma once
#include "GlobalParams.h"
#include <math.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "CvGeFunctions.h"
#include <vector>
#include <stack>
#include <map>

class CvProgramFunctions //������������ĸ������
{
private:
	bool comparePoint(Point pt1, Point pt2);//����Point���Զ�������ȽϺ�����map��
public:

	static ErrorStatus showCracks(vector<vector<Point>>& vecCracks, vector<CrackInfo>& crackInfos); //����ѷ�ͼ��[07-09]
	static ErrorStatus getCrackLengthByRatio(vector<vector<Point>>& vecCracks, multimap<double, Point>& mapLength_Pos, double changeRatio);//����ѷ쳤��[07-10]
	static double getCrackArea(vector<CrackInfo>& crackInfos, double changeRatio);
	static vector<double> getCrackLegth(vector<CrackInfo>& crackInfos, double changeRatio);
	static double getCrackMaxLength(vector<CrackInfo>& crackInfos, double changeRatio);
	static vector<double> getCrackWidth(vector<CrackInfo>& crackInfos, double changeRatio);
	static double getCrackMeanWidth(vector<CrackInfo>& crackInfos, double changeRatio);
	static vector<double> getGeoFeatures(vector<CrackInfo>& crackInfos, double changeRatio);
};