#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/ml.hpp>

using namespace cv::ml;
using namespace cv;
using namespace std;

int SZ = 20;
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
	Size(20, 20), //winSize
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
	Ptr<SVM> svm = Algorithm::load<SVM>("results/eyeGlassClassifierModel.yml");
	
	Mat img = imread("ocho.png", CV_LOAD_IMAGE_GRAYSCALE);
	resize(img, img, Size(20, 20));
	Mat deskewedTestCells = deskew(img);

	vector<float> test_descriptor;
	hog.compute(deskewedTestCells, test_descriptor);

	int descriptor_size = test_descriptor.size();
	cout << "Descriptor Size : " << descriptor_size << endl;

	/// Convert descriptor to matrix
	Mat testMat(Size(descriptor_size,1), CV_32FC1);

	for (int i = 0; i < descriptor_size; i++) 
		testMat.at<float>(0, i) = test_descriptor[i];

	Mat testResponse;
	svm->predict(test_descriptor, testResponse);
	
	cout << "El numero es: " << testResponse << endl;

	system("PAUSE");
	return 0;
}
