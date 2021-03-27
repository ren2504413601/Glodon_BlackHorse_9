#include"CvProgramFunctions.h"
// #define picsShow

ErrorStatus CvProgramFunctions::showCracks(vector<vector<Point>>& vecCracks, vector<CrackInfo>& crackInfos)
{
	Mat srcOriginPic = imread(GLOBAL_ORIGIN_JPG_DIR);
	Mat srcOriginPicBGR = srcOriginPic;
	Mat resImg;
	cout << "原来图片是"<< srcOriginPic.channels() << "通道图片" << endl;
#ifdef picsShow	
	imshow("test", srcOriginPic);
	waitKey();
#endif
	if (srcOriginPic.empty())
	{
		return ERROR_OPENFILE;
	}

	cvtColor(srcOriginPic, resImg, COLOR_BGR2GRAY, 1); // 改为灰度图 
#ifdef picsShow	
	imshow("test", resImg);
	waitKey();
#endif
	CvGeFunctions::addContrast(srcOriginPic); // 加强对比度 
#ifdef picsShow	
	imshow("test", resImg);
	waitKey();
#endif
	CvGeFunctions::swapMat(srcOriginPic, resImg);

	int avgGray = CvGeFunctions::getCannyThresholdUp(srcOriginPic);

	int cannyMax = (avgGray) > 255 ? 255 : (avgGray);
	int cannyMin = cannyMax / 5; // 上下阈值之比为5:1 [ssr 07-09]
	Canny(srcOriginPic, resImg, 240, 255, 3); // 边缘检测 
	//Canny(srcOriginPic, resImg, cannyMin, cannyMax, 3); // 边缘检测 
	imwrite(GLOBAL_OUTPUT_DIR + "canneyRes.png", resImg);

#ifdef picsShow	
	imshow("test", resImg);
	waitKey();
#endif

	//形态学变换
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	dilate(resImg, resImg, kernel);//膨胀
	morphologyEx(resImg, resImg, MORPH_CLOSE, kernel, Point(-1, -1), 3);
	morphologyEx(resImg, resImg, MORPH_CLOSE, kernel);

	CvGeFunctions::setEdgeBlack(resImg);

	imwrite(GLOBAL_OUTPUT_DIR + "afterKernelRes.png", resImg);

	//寻找连通域
	vector<vector<Point>> connectedDomains;
	CvGeFunctions::findConnectedDomain(resImg, connectedDomains, 0, 0); //area和WHRatio调整为0，增加识别几率，缺点：连通域过宽
	kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
	morphologyEx(resImg, resImg, MORPH_CLOSE, kernel, Point(-1, -1), 5);
	for (int m = 0; m < connectedDomains.size(); ++m)
	{
		for (int n = 0; n < connectedDomains[m].size(); ++n)
		{
			int cPointR, cPointG, cPointB, cPoint;//currentPoint;
			int i = connectedDomains[m][n].y;
			int j = connectedDomains[m][n].x;

			srcOriginPicBGR.at<Vec3b>(i, j)[0] = 0;
			srcOriginPicBGR.at<Vec3b>(i, j)[1] = 0;
			srcOriginPicBGR.at<Vec3b>(i, j)[2] = 255;
		}
	}
	imwrite(GLOBAL_OUTPUT_DIR + "orgConnRes.png", srcOriginPicBGR);
	connectedDomains.clear();
	CvGeFunctions::findConnectedDomain(resImg, connectedDomains, 0, 0);
	kernel = getStructuringElement(MORPH_CROSS, Size(3, 3));
	morphologyEx(resImg, resImg, MORPH_CLOSE, kernel);

	kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	erode(resImg, resImg, kernel);

	connectedDomains.clear();
	CvGeFunctions::findConnectedDomain(resImg, connectedDomains, 0, 0);
	vecCracks = connectedDomains;

	cout << "开始测量" << endl;
	cout << "连通域数量：" << connectedDomains.size() << endl;
	Mat lookUpTable(1, 256, CV_8U, Scalar(0));
	/*vector<CrackInfo> crackInfos;*/
	
	for (auto domain_it = connectedDomains.begin(); domain_it != connectedDomains.end(); ++domain_it)
	{
		LUT(resImg, lookUpTable, resImg);
		for (auto point_it = domain_it->cbegin(); point_it != domain_it->cend(); ++point_it)
		{
			resImg.ptr<uchar>(point_it->y)[point_it->x] = 255;
		}
	#ifdef picsShow	
		imshow("connect pic", resImg);
		waitKey();
	#endif
		double area = (double)domain_it->size();
		CvGeFunctions::thinImage(resImg);
		auto domain_copy = *domain_it;
		CvGeFunctions::getWhitePoints(resImg, domain_copy);
		double length = (double)domain_copy.size();
		Point position = CvGeFunctions::calInfoPosition(resImg.rows, resImg.cols, 50, domain_copy);
		crackInfos.push_back(CrackInfo(position, area, length, area / length));
	#ifdef picsShow	
		imshow("thin pic", resImg);
		waitKey();
	#endif
	}

	cout << "开始绘制信息" << endl;
	cout << "信息数量：" << crackInfos.size() << endl;

	LUT(resImg, lookUpTable, resImg);
	for (auto domain_it = connectedDomains.cbegin(); domain_it != connectedDomains.cend(); ++domain_it)
	{
		for (auto point_it = domain_it->cbegin(); point_it != domain_it->cend(); ++point_it)
		{
			resImg.ptr<uchar>(point_it->y)[point_it->x] = 255;
		}
	}

	//resImg = ~resImg; //颜色取反，获取白底黑缝的图案[ssr 07-09]
	imwrite(GLOBAL_OUTPUT_DIR + "result1.png", resImg);

	Mat redRes = ~imread(GLOBAL_OUTPUT_DIR + "result1.png");
	CvGeFunctions::turnBlackCrackToRed(redRes);//裂缝颜色取红色[ssr 07-09]
	imwrite(GLOBAL_OUTPUT_DIR + "redRes.png", redRes);


	Mat mix;
	addWeighted(redRes, 0.5, srcOriginPicBGR, 0.5, 0.0, mix);//裂缝图和原图按1:1权重混合 [ssr 07-09]
	imwrite(GLOBAL_OUTPUT_DIR + "mixRes.png", mix);

	cout << "保存图像完成" << endl;
	return SUCCEED;
}
bool CvProgramFunctions::comparePoint(Point pt1, Point pt2)
{
	if (pt1.x < pt2.x)
	{
		return true;
	}
	else if (pt1.x > pt2.x)
	{
		return false;
	}
	else if (pt1.y < pt2.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}
ErrorStatus CvProgramFunctions::getCrackLengthByRatio(vector<vector<Point>>& vecCracks, multimap<double, Point>& mapLength_Pos, double changeRatio)
{
	for (int i = 0; i < vecCracks.size(); ++i)
	{
		int piexLength = vecCracks[i].size();
		Point crPos = vecCracks[i][piexLength / 2];
		double trueLength = piexLength * changeRatio;
		mapLength_Pos.insert(std::make_pair( trueLength, crPos));
	}
	return SUCCEED;
}

double CvProgramFunctions::getCrackArea(vector<CrackInfo>& crackInfos, double changeRatio)
{
	double area = 0.;
	for (int m = 0; m < crackInfos.size(); m++)
	{
		CrackInfo tmpCrack = crackInfos[m];
		area += tmpCrack.length*changeRatio*tmpCrack.width*changeRatio;
	}
	return area;
}
vector<double> CvProgramFunctions::getCrackLegth(vector<CrackInfo>& crackInfos, double changeRatio)
{
	vector<double> length;
	for (int m = 0; m < crackInfos.size(); m++)
	{
		CrackInfo tmpCrack = crackInfos[m];
		length.push_back(tmpCrack.length*changeRatio);
	}
	return length;
}
double CvProgramFunctions::getCrackMaxLength(vector<CrackInfo>& crackInfos, double changeRatio)
{
	vector<double> lengths = getCrackLegth(crackInfos, changeRatio);
	double maxlen = 0.;
	for (double& val : lengths)
	{
		maxlen = max(val, maxlen);
	}
	return maxlen;
}
vector<double> CvProgramFunctions::getCrackWidth(vector<CrackInfo>& crackInfos, double changeRatio)
{
	vector<double> width;
	for (int m = 0; m < crackInfos.size(); m++)
	{
		CrackInfo tmpCrack = crackInfos[m];
		width.push_back(tmpCrack.width*changeRatio);
	}
	return width;
}
double CvProgramFunctions::getCrackMeanWidth(vector<CrackInfo>& crackInfos, double changeRatio)
{
	vector<double> widths = getCrackWidth(crackInfos, changeRatio);
	double meanwid = 0.;
	for (double& val : widths)
	{
		meanwid += val / widths.size();
	}
	return meanwid;
}

vector<double> CvProgramFunctions::getGeoFeatures(vector<CrackInfo>& crackInfos, double changeRatio)
{
	double area = getCrackArea(crackInfos, changeRatio);
	double maxLen = getCrackMaxLength(crackInfos, changeRatio);
	double meanWid = getCrackMeanWidth(crackInfos, changeRatio);
	vector<double> Features  = {area, maxLen, meanWid};
	return Features;
}
