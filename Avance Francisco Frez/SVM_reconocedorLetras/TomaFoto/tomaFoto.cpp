#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/ml.hpp>


using namespace cv::ml;
using namespace cv;
using namespace std;

int SZ = 200;
float affineFlags = WARP_INVERSE_MAP | INTER_LINEAR;

Mat deskew(Mat& img) {
	Moments m = moments(img);
	if (abs(m.mu02) < 1e-2) {
		return img.clone();
	}
	float skew = m.mu11 / m.mu02;
	Mat warpMat = (Mat_<float>(2, 3) << 1, skew, -0.5*SZ*skew, 0, 1, 0);
	Mat imgOut = Mat::zeros(img.rows, img.cols, img.type());
	warpAffine(img, imgOut, warpMat, imgOut.size(), affineFlags);

	return imgOut;
}

HOGDescriptor hog(
	Size(200, 200), //winSize
	Size(8, 8), //blocksize
	Size(4, 4), //blockStride,
	Size(8, 8), //cellSize,
	9, //nbins,
	1, //derivAper,
	-1, //winSigma,
	0, //histogramNormType,
	0.2, //L2HysThresh,
	0,//gammal correction,
	64,//nlevels=64
	1);


int main()
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat video;
	namedWindow("Video", 1);

	Rect Rec(100, 70, 200, 200);

	char WaitKeyStatus;
	string imageName;
	int a = 0;
	int b = 0;
	int c = 0;
	int nada = 0;

	Mat back, fore, fore_thresh, mask;
	cap >> back;
	cvtColor(back, back, CV_BGR2GRAY);
	imshow("Back", back);

	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		cvtColor(frame, video, CV_BGR2GRAY);
		rectangle(video, Rec, Scalar(255), 1, 8, 0);
		imshow("Video", video);

		absdiff(video, back, fore);
		threshold(fore, mask, 10, 255, THRESH_BINARY);

		Mat structuringElement = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
		morphologyEx(mask, mask, cv::MORPH_CLOSE, structuringElement);
		imshow("Thresh", mask);

		bitwise_and(video, mask, fore);
		imshow("Fore", fore);

		Mat Roi = fore(Rec);
		//resize(Roi, Roi, Size(128, 128));
		
		//waits for the Key input
		WaitKeyStatus = waitKey(1);
		if (WaitKeyStatus == 'a' || WaitKeyStatus == 'A') //Save picture
		{
			imageName = "A/A" + to_string(a);
			imwrite(imageName + ".png", Roi);
			a++;
		}

		if (WaitKeyStatus == 'b' || WaitKeyStatus == 'B') //Save picture
		{
			imageName = "B/B" + to_string(b);
			imwrite(imageName + ".png", Roi);
			b++;
		}

		if (WaitKeyStatus == 'c' || WaitKeyStatus == 'C') //Save picture
		{
			imageName = "C/C" + to_string(c);
			imwrite(imageName + ".png", Roi);
			c++;
		}

		if (WaitKeyStatus == 'n' || WaitKeyStatus == 'N') //Save picture
		{
			imageName = "Nada/Nada" + to_string(nada);
			imwrite(imageName + ".png", Roi);
			nada++;
		}

		if (WaitKeyStatus == 'q' || WaitKeyStatus == 'Q' || WaitKeyStatus == 27) //Quit
		{
			destroyAllWindows();
			break;
		}
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
