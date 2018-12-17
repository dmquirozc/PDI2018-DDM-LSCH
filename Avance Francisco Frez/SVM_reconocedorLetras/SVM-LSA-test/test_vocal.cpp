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
	Size(128, 128), //winSize
	Size(16, 16), //blocksize
	Size(4, 4), //blockStride,
	Size(8, 8), //cellSize,
	9, //nbins,
	1, //derivAper,
	-1, //winSigma,
	0, //histogramNormType,
	0.2, //L2HysThresh,
	0,//gammal correction,
	64,//nlevels=64
	0);


int main()
{	
	Ptr<SVM> svm = Algorithm::load<SVM>("results/classifierModel.yml");

	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat video;
	
	namedWindow("Video", 1);

	Rect Rec(100, 70, 200, 200);

	Mat back, fore, mask;
	cap >> back;
	cvtColor(back, back, CV_BGR2GRAY);
	//imshow("Back", back);

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
		resize(Roi, Roi, Size(128, 128));

		Mat deskewedTestCells = deskew(Roi);
		imshow("Roi", Roi);
		vector<float> test_descriptor;
		hog.compute(Roi, test_descriptor);

		int descriptor_size = test_descriptor.size();

		/// Convert descriptor to matrix
		Mat testMat(Size(descriptor_size, 1), CV_32FC1);

		for (int i = 0; i < descriptor_size; i++)
			testMat.at<float>(0, i) = test_descriptor[i];
		Mat testResponse;
		svm->predict(test_descriptor, testResponse);

		if (testResponse.at<float>(0, 0) == 1)
		{
			cout << "La letra es: A" << endl;
		}
		if (testResponse.at<float>(0, 0) == 2)
		{
			cout << "La letra es: B" << endl;
		}
		if (testResponse.at<float>(0, 0) == 3)
		{
			cout << "La letra es: C" << endl;
		}
		if (testResponse.at<float>(0, 0) == 4)
		{
			cout << "No hay letra" << endl;
		}
		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
