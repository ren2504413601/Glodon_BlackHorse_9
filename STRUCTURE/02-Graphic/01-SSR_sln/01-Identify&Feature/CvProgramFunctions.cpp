#include"CvProgramFunctions.h"

ErrorStatus CvProgramFunctions::showCracks(vector<vector<Point>>& vecCracks, vector<CrackInfo>& crackInfos)
{
	Mat srcOriginPic = imread(GLOBAL_ORIGIN_JPG_DIR);
	Mat srcOriginPicBGR = srcOriginPic;
	Mat resImg;
	cout << "ԭ��ͼƬ��"<< srcOriginPic.channels() << "ͨ��ͼƬ" << endl;
	//imshow("test", srcOriginPic);
	//waitKey();
	if (srcOriginPic.empty())
	{
		return ERROR_OPENFILE;
	}

	cvtColor(srcOriginPic, resImg, COLOR_BGR2GRAY, 1); // ��Ϊ�Ҷ�ͼ 
	//imshow("test", resImg);
	//waitKey();
	CvGeFunctions::addContrast(srcOriginPic); // ��ǿ�Աȶ� 
	//imshow("test", resImg);
	//waitKey();
	CvGeFunctions::swapMat(srcOriginPic, resImg);

	int avgGray = CvGeFunctions::getCannyThresholdUp(srcOriginPic);

	int cannyMax = (avgGray) > 255 ? 255 : (avgGray);
	int cannyMin = cannyMax / 5; // ������ֵ֮��Ϊ5:1 [ssr 07-09]
	Canny(srcOriginPic, resImg, 240, 255, 3); // ��Ե��� 
	//Canny(srcOriginPic, resImg, cannyMin, cannyMax, 3); // ��Ե��� 
	imwrite(GLOBAL_OUTPUT_DIR + "canneyRes.png", resImg);

	//imshow("test", resImg);
	//waitKey();

	//��̬ѧ�任
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	dilate(resImg, resImg, kernel);//����
	morphologyEx(resImg, resImg, MORPH_CLOSE, kernel, Point(-1, -1), 3);
	morphologyEx(resImg, resImg, MORPH_CLOSE, kernel);

	CvGeFunctions::setEdgeBlack(resImg);

	imwrite(GLOBAL_OUTPUT_DIR + "afterKernelRes.png", resImg);

	//Ѱ����ͨ��
	vector<vector<Point>> connectedDomains;
	CvGeFunctions::findConnectedDomain(resImg, connectedDomains, 0, 0); //area��WHRatio����Ϊ0������ʶ���ʣ�ȱ�㣺��ͨ�����
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

	cout << "��ʼ����" << endl;
	cout << "��ͨ��������" << connectedDomains.size() << endl;
	Mat lookUpTable(1, 256, CV_8U, Scalar(0));
	/*vector<CrackInfo> crackInfos;*/
	
	for (auto domain_it = connectedDomains.begin(); domain_it != connectedDomains.end(); ++domain_it)
	{
		LUT(resImg, lookUpTable, resImg);
		for (auto point_it = domain_it->cbegin(); point_it != domain_it->cend(); ++point_it)
		{
			resImg.ptr<uchar>(point_it->y)[point_it->x] = 255;
		}
		//imshow("connect pic", resImg);
		//waitKey();
		double area = (double)domain_it->size();
		CvGeFunctions::thinImage(resImg);
		auto domain_copy = *domain_it;
		CvGeFunctions::getWhitePoints(resImg, domain_copy);
		double length = (double)domain_copy.size();
		Point position = CvGeFunctions::calInfoPosition(resImg.rows, resImg.cols, 50, domain_copy);
		crackInfos.push_back(CrackInfo(position, area, length, area / length));
		//imshow("thin pic", resImg);
		//waitKey();
	}

	cout << "��ʼ������Ϣ" << endl;
	cout << "��Ϣ������" << crackInfos.size() << endl;

	LUT(resImg, lookUpTable, resImg);
	for (auto domain_it = connectedDomains.cbegin(); domain_it != connectedDomains.cend(); ++domain_it)
	{
		for (auto point_it = domain_it->cbegin(); point_it != domain_it->cend(); ++point_it)
		{
			resImg.ptr<uchar>(point_it->y)[point_it->x] = 255;
		}
	}

	//resImg = ~resImg; //��ɫȡ������ȡ�׵׺ڷ��ͼ��[ssr 07-09]
	imwrite(GLOBAL_OUTPUT_DIR + "result1.png", resImg);

	Mat redRes = ~imread(GLOBAL_OUTPUT_DIR + "result1.png");
	CvGeFunctions::turnBlackCrackToRed(redRes);//�ѷ���ɫȡ��ɫ[ssr 07-09]
	imwrite(GLOBAL_OUTPUT_DIR + "redRes.png", redRes);


	Mat mix;
	addWeighted(redRes, 0.5, srcOriginPicBGR, 0.5, 0.0, mix);//�ѷ�ͼ��ԭͼ��1:1Ȩ�ػ�� [ssr 07-09]
	imwrite(GLOBAL_OUTPUT_DIR + "mixRes.png", mix);

	cout << "����ͼ�����" << endl;
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
