#include<iostream>
#include<opencv.hpp>

using namespace cv;
using namespace std;

vector<float> Match(const Mat& src, const Mat& obj, vector<Rect>& rect, int type, int num = 1);

void main()
{
	Mat src = imread("D:\\img.png");
	Mat obj = imread("D:\\template.png");
	namedWindow("Match");   imshow("Match", obj);

	Mat src0 = src.clone(), dst;
	//cvtColor(src, src, CV_BGR2GRAY);
	//cvtColor(obj, obj, CV_BGR2GRAY);
	//normalize(src, src, 0, 255, NORM_MINMAX);
	//normalize(obj, obj, 0, 255, NORM_MINMAX);

	namedWindow("Result");  int type = 0, num = 1;
	createTrackbar("Type", "Result", &type, 5);
	createTrackbar("Number", "Result", &num, 5);

	vector<Rect> rect;  vector<float> pro;
	while ((waitKey(500) & 255) != 13)
	{
		dst = src0.clone();
		pro = Match(src, obj, rect, type, num);
		for (int i = 0; i < num; ++i)
		{
			rectangle(dst, rect[i], Scalar(0, 255, 0), 2);
			cout << i << "\t" << rect[i] << "\t" << pro[i] << endl;
			Point center(rect[i].x + rect[i].width / 2, rect[i].y + rect[i].height / 2);
			circle(dst, center, 4, Scalar(0, 255, 0), -1);
		}//end for
		imshow("Result", dst);  cout << endl;
	}//end while
}//end main

vector<float> Match(const Mat& src, const Mat& obj, vector<Rect>& rect, int type, int num)
{
	Mat dst;
	matchTemplate(src, obj, dst, type);
	normalize(dst, dst, 0, 255, NORM_MINMAX);

	vector<float> pro;  rect.clear();
	double mx = 0;  Point loc, pre(-1, -1);
	for (int i = 0; i < num;)
	{
		minMaxLoc(dst, NULL, &mx, NULL, &loc);
		if (abs(loc.x - pre.x) > 10 && abs(loc.y - pre.y) > 10)
		{
			rect.push_back(Rect(loc.x, loc.y, obj.cols, obj.rows));
			pro.push_back(mx / 255);  pre = loc;  ++i;
		}//end if
		dst.at<float>(loc.y, loc.x) = 0;    mx = 0;
	}//end for
	return pro;
}//end Match