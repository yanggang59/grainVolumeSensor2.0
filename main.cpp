#include <opencv2/opencv.hpp>
#include "cal_3d_coor.h"
#include "skeleton.h"
#include "cal_area.h"

using namespace std;
using namespace cv;


float a, b, c,A,B,C; //abc��ax+by+cz+1=0����ʽ�Ĳ�����ABC��Z=Ax+By+C��ʽ�Ĳ���,��Ҫת��һ��

/*�ڲ�������֮ǰ�Ѿ��궨��*/
Mat K=(Mat_<float>(3,3)<<518.8906422065566, 0, 294.5896218285499,
											0, 520.0230989700873, 226.11902054293,
											0, 0, 1);
int main()
{
	/*�����ṹ�������֮ǰ�궨�õ�*/
	//FileStorage fs("C://test_pic//test.yml",FileStorage::READ);
	//fs["A"]>>A;
	//fs["B"]>>B;
	//fs["C"]>>C;
	//fs.release();

	a = 0;
	b = -0.0153846;
	c = -0.0068497;

	A = -a / c;
	B = -b / c;
	C = -1 / c;

	Vec3f coeff(A,B,C);
	//cout<<coeff[0]<<endl;
	
	Mat src=imread("C://test_pic//500_light.jpg");
	int height=src.rows;
	int width=src.cols;

	Mat skeleton(Size(width,height),CV_8UC1,Scalar(0));
	Mat r_thresholded;
	vector<Mat> channels_bgr;
	split(src,channels_bgr);
	threshold(channels_bgr[2],r_thresholded,220,255,3);	

	Mat element = getStructuringElement(MORPH_RECT, Size(3,3));
	morphologyEx(r_thresholded,r_thresholded,MORPH_OPEN,element);

	/*���Ͳ������޲��׶�*/
	//dilate(z_thresholded,z_thresholded,element);

	//thinning(r_thresholded,skeleton);

	vector<int> center_line;

	//��ʼ���洢�������
	for(int i=0;i<width;i++)
	{
		center_line.push_back(0);
	}

	int count=0;
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
			if(r_thresholded.at<uchar>(i,j)!=0 && center_line[j]==0)
			{
				center_line[j]=1;
				skeleton.at<uchar>(i,j)=255;

				count++;
			}
	}

	cout<<"count="<<count<<endl;
	/*imshow("r_channel",channels_bgr[2]);
	imshow("src",src);
	imshow("r_thresholded",r_thresholded);*/
	imshow("skeleton",skeleton);
	/*imwrite("C://skeleton.jpg",skeleton);
*/

//�����ĸ�������50������Ϊ�ǿհ�֡,continue��������һ��ѭ��

	if(count<50)
	{
		cout<<"��֡�ǿհ�֡"<<endl;
		//continue;
	}



	/*����ֱ�������е���������������*/
	vector<Vec3f> allPoints;

	int z_count=0;  //��¼����z_threshold�ĵ�ĸ���
	int z_threshold=20;
	cal_3d_coor(skeleton,allPoints,K,coeff,z_count,z_threshold);
	cout<<"����z_threshold�ĵ�ĸ�����"<<z_count<<endl;
	cout<<"��ĸ���Ϊ��"<<allPoints.size()<<endl;

	if(z_count<50)
	{
		cout<<"�ǹΰ�֡"<<endl;
	}
	
		waitKey(0);
}