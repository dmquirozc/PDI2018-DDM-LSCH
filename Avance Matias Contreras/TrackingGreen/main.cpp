#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>

using namespace std;

int main(){
    cv::VideoCapture vid;   // Captura de vídeo
    vid.open(0);

    if(!vid.isOpened()) {
        cerr << "Cannot open the web cam." << endl;
        return 1;
    }

    cv::namedWindow("Control HSV", CV_WINDOW_AUTOSIZE);

    int lowH = 0, highH = 255;      // Valores mínimos/máximos de matiz
    int lowS = 0, highS = 255;      // Valores mínimos/máximos de saturación
    int lowV = 0, highV = 255;      // Valores mínimos/máximos de valor

    cvCreateTrackbar("LowH", "Control HSV", &lowH, 255);   // Hue(0 - 255)
    cvCreateTrackbar("HighH", "Control HSV", &highH, 255);

    cvCreateTrackbar("LowS", "Control HSV", &lowS, 255);   // Saturation(0 - 255)
    cvCreateTrackbar("HighS", "Control HSV", &highS, 255);

    cvCreateTrackbar("LowV", "Control HSV", &lowV, 255);   // Value(0 - 255)
    cvCreateTrackbar("HighV", "Control HSV", &highV, 255);

    cv::Mat img, HSV, threshold;

    /* Mezcla de Gaussianas
    cv::Mat fg, gaussian;
    int historyMOG = 255;
    double varThreshold = 16;
    double learningRate = 0.005;
    bool shadowDetection = true;
    cv::Ptr<cv::BackgroundSubtractorMOG2> mog;
    mog = cv::createBackgroundSubtractorMOG2(historyMOG, varThreshold, shadowDetection); */

    while(1) {
        vid >> img;
        //cv::cvtColor(img, gaussian, cv::COLOR_BGR2GRAY);
        //mog->apply(gaussian, fg, learningRate);

        cv::cvtColor(img, HSV, cv::COLOR_BGR2HSV);
        cv::inRange(HSV, cv::Scalar(lowH, lowS, lowV), cv::Scalar(highH, highS, highV), threshold);
        //cv::bitwise_and(fg, HSV, threshold);

        // morphological opening (remove small objects from the foreground)
        cv::erode(threshold, threshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)) );
        cv::dilate(threshold, threshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)) );

        // morphological closing (fill small holes in the foreground)
        cv::dilate(threshold, threshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)) );
        cv::erode(threshold, threshold, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)) );

        cv::imshow("Thresholded Image", threshold);     // Muestra imagen del umbral
        cv::imshow("Original", img);                    // Muestra imagen original

        if(cv::waitKey(10) != -1) break;
    }

    vid.release();
    return 0;
}
