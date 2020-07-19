// TestOpenCv.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#pragma once
#include <math.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "CvGeFunctions.h"
#include <vector>
#include <stack>
#include "CvProgramFunctions.h"
#include "EXIF.h"
#include "GlobalParams.h"
using namespace std;
using namespace cv;

//#define test



#include <mat.h>

Mat padding_bounderies(mxLogical *Bounderies, int& M, int& N)
{
	Mat resImg = Mat::zeros(M, N, CV_8U); // 长度是N，宽度是M
	for (int i = 0; i < N; ++i)
	{
		vector<int> tmpCrackBoundery;
		for (int j = 0; j < M; ++j)
		{
			if (Bounderies[i*M + j])
				tmpCrackBoundery.push_back(j);
		}


		if (!tmpCrackBoundery.empty())
		{
			for (int k = tmpCrackBoundery.front(); k < tmpCrackBoundery.back(); ++k)
			{
				resImg.at<uchar>(k, i) = 255;
			}
		}

	}
	return resImg;
}



Mat getGroundTruthMat()
{
	MATFile *pmatFile = NULL;
	mxArray *pMxArray = NULL;
	mxArray *pStruct = NULL;
	mxLogical *Bounderies = NULL;
	pmatFile = matOpen(GLOBAL_GROUDTRUTH_MAT_FILE.c_str(), "r");

	//const char ** var_names;// variable names in .mat file
	//int nvars;// the number of variables in .mat file
	//var_names = (const char **)matGetDir(pmatFile, &nvars);
	//if (var_names == NULL)
	//{
	//	cout << "The .mat file contarin 0 variable, please perform checking!" << endl;
	//	exit(-1);
	//}

	//// output the number of variable in .mat file
	//cout << "The numboer of variable in.mat file is : " << nvars << endl;
	//cout << " The name for all variables:";
	//for (int i = 0; i < nvars; i++) cout << string(var_names[i]) << endl;

	// because we only have one variable in .mat file , so the index of var_names is set as 0
	pMxArray = matGetVariable(pmatFile, "groundTruth"); // 读取名字为 groundTruth 的变量 var_names[0] ->groundTruth

	//// output the number of variable in groundTruth struct.
	//nvars = mxGetNumberOfFields(pMxArray);
	//cout << "The numboer of variable in groundTruth struct is : " << nvars << endl;
	//cout << " The name for all variables:";
	//for (int i = 0; i < nvars; i++) cout << mxGetFieldNameByNumber(pMxArray, i) << endl;

	/*Get the ObjectsNumber */
	pStruct = mxGetField_700(pMxArray, 0, "Boundaries"); // mxGetFieldNameByNumber(pMxArray, 1) -> Boundaries
	Bounderies = (mxLogical*)mxGetData(pStruct); // mxLogical -> bool
	int M = mxGetM(pStruct); //获取矩阵行数
	int N = mxGetN(pStruct); //获取矩阵列数
	
	Mat resImg = padding_bounderies(Bounderies, M, N);
	//cout << "row number:" << M << "col number:" << N << endl;
	//cout << resImg;
	//imshow("test", resImg);
	//waitKey();
	//imwrite("black.tiff", resImg);
	
	return resImg;
}

double computeAbsError(Mat& groundTrthImg, Mat& segmentionPic)
{
	Mat gray;
	cvtColor(segmentionPic, gray, COLOR_BGR2GRAY, 1); // 改为灰度图
	CvGeFunctions::binaryzation(gray);
	int M = groundTrthImg.rows;
	int N = groundTrthImg.cols;
	double absErr = 0.0, divdided = double(M*N*255);
	//cout << gray;
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			absErr += abs(groundTrthImg.at<uchar>(i, j) - gray.at<uchar>(i, j))/divdided;
		}
	}
	return	absErr;
}

double computeIoULoss(Mat& groundTrthImg, Mat& segmentionPic)
{
	Mat gray;
	cvtColor(segmentionPic, gray, COLOR_BGR2GRAY, 1); // 改为灰度图
	CvGeFunctions::binaryzation(gray);
	int M = groundTrthImg.rows;
	int N = groundTrthImg.cols;
	int interSum = 0, unionSum = 0;
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			interSum += groundTrthImg.at<uchar>(i, j) / 255 * gray.at<uchar>(i, j) / 255;
			unionSum += groundTrthImg.at<uchar>(i, j) / 255 + gray.at<uchar>(i, j) / 255;
		}
	}
	return	1-double(2*interSum) / unionSum;
}


int main()
{
	vector<vector<Point>> vecCracks;
	vector<CrackInfo> crackInfos;
	int errorStatue = CvProgramFunctions::showCracks(vecCracks, crackInfos);
	if (errorStatue == ERROR_OPENFILE)
	{
		printf("打开图片失败！\n");
	}
	//double area = CvProgramFunctions::getCrackArea(crackInfos, GLOBAL_CHANGE_RATE);
	//double maxLen = CvProgramFunctions::getCrackMaxLength(crackInfos, GLOBAL_CHANGE_RATE);
	//double meanWid = CvProgramFunctions::getCrackMeanWidth(crackInfos, GLOBAL_CHANGE_RATE);
	vector<double> Feats = CvProgramFunctions::getGeoFeatures(crackInfos, GLOBAL_CHANGE_RATE);

	cout << "面积大小：" << Feats[0] << endl;
	cout << "最大长度：" << Feats[1] << endl;
	cout << "平均宽度：" << Feats[2] << endl;
	Mat groudTruthPic = getGroundTruthMat();
	Mat resPic = imread(GLOBAL_OUTPUT_DIR + "result1.png");
	cout << "最终结果和标注数据的相对误差大小：" << computeAbsError(groudTruthPic, resPic) << endl;
	cout << "最终结果和标注数据的IoU（交并补）大小：" << computeIoULoss(groudTruthPic, resPic) << endl;
	
	imwrite(GLOBAL_OUTPUT_DIR + "true.png", groudTruthPic);
	imshow("seg results", resPic);
	imshow("groudTruth", groudTruthPic);
	waitKey();
	return 0;
}