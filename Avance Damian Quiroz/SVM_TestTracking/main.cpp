#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/core/ocl.hpp>
#include <opencv2/ml.hpp>
#include <ctime>

#define SZ 200

using namespace cv::ml;
using namespace cv;
using namespace std;


#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

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

int main(int argc, char **argv)
{
    Ptr<SVM> svm = Algorithm::load<SVM>("SVM/classifierModel.yml");
    string trackerTypes[8] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"};



    string trackerType = trackerTypes[7];

    Ptr<Tracker> tracker;

        if (trackerType == "MIL")
            tracker = TrackerMIL::create();
        if (trackerType == "KCF")
            tracker = TrackerKCF::create();
       if (trackerType == "TLD")
           tracker = TrackerTLD::create();
        if (trackerType == "MEDIANFLOW")
            tracker = TrackerMedianFlow::create();
        if (trackerType == "GOTURN")
            tracker = TrackerGOTURN::create();
        if (trackerType == "MOSSE")
            tracker = TrackerMOSSE::create();
        if (trackerType == "CSRT")
            tracker = TrackerCSRT::create();
//    }

    VideoCapture video;
    video.open(0);//  video("videos/chaplin.mp4");

    // Exit if video is not opened
    if(!video.isOpened())
    {
        cout << "Could not read video file" << endl;
        return 1;
    }
    /* Variable para indicar la calibracion*/


    /*Variables para guardar fotos de prueba*/

    string imageName;
    Mat Roi;
    Roi = NULL;

    /*Variables para describir el funcionamiento
    * base del video */
    Mat frame;
    bool trackOk=false;
    video.read(frame);
    resize(frame,frame,Size(720,480),0,0,INTER_LINEAR);
    Rect2d bbox(100,50,200,200);

    Mat back, fore, fore_thresh, mask,grayscale;

    imshow("Tracking", frame);

    video >> back;
    resize(back,back,Size(720,480),0,0,INTER_LINEAR);
    cvtColor(back, back, CV_BGR2GRAY);
    //imshow("Back", back);
    while(video.read(frame))
    {
        resize(frame,frame,Size(720,480),0,0,INTER_LINEAR);
        cvtColor(frame, grayscale, CV_BGR2GRAY);
        absdiff(grayscale, back, fore);
        threshold(fore, mask, 10, 255, THRESH_BINARY);
        Mat structuringElement = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
        morphologyEx(mask, mask, cv::MORPH_CLOSE, structuringElement);
        //imshow("Thresh", mask);

        bitwise_and(grayscale, mask, fore);
        //imshow("Fore", fore);

        double timer = (double)getTickCount();
        float fps = getTickFrequency() / ((double)getTickCount() - timer);

        if (trackOk)
        {
            int limit_x = frame.cols;
            int limit_y = frame.rows;
            double boxlimit =bbox.x;
            double boylimit =bbox.y;
            tracker->update(fore, bbox);

            if(bbox.x+bbox.width>=limit_x){
                bbox=Rect2d(bbox.x,bbox.y,limit_x-boxlimit-10,bbox.height);
            }
            if(bbox.x<10){
                  bbox=Rect2d(10,bbox.y,bbox.width,bbox.height);
            }

            if(bbox.y+bbox.height>limit_y){
                bbox=Rect2d(bbox.x,bbox.y,bbox.width,limit_y-boylimit-10);
            }

            if(bbox.y<10){
               bbox=Rect2d(bbox.x,10,bbox.width,bbox.height);
            }
            Roi = fore(bbox);
            resize(Roi,Roi,Size(200,200));
            Mat deskewedTestCells = deskew(Roi);
            vector<float> test_descriptor;

            hog.compute(Roi, test_descriptor);

            int descriptor_size = test_descriptor.size();

            /// Convert descriptor to matrix
            Mat testMat(Size(descriptor_size, 1), CV_32FC1);

            for (int i = 0; i < descriptor_size; i++)
            testMat.at<float>(0, i) = test_descriptor[i];
            Mat testResponse;
            svm->predict(test_descriptor, testResponse);
            rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );

            if (testResponse.at<float>(0, 0) == 1){
                    cout << "La letra es: A" << endl;
            }
            if (testResponse.at<float>(0, 0) == 2){
                cout << "La letra es: B" << endl;
            }
            if (testResponse.at<float>(0, 0) == 3){
                cout << "La letra es: C" << endl;
            }
            if (testResponse.at<float>(0, 0) == 4){
                cout << "No hay letra" << endl;
            }

            imshow("Tracking", frame);
        }else{
            //Rect2d cajaVerdeFija(128, 128,100,50);
            //putText(frame, trackerType + " Tracker", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);
            //putText(frame, "FPS : " + SSTR(int(fps)), Point(100,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);
            rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
            imshow("Tracking", frame);
        }


        //time in seconds
        time_t t = std::time(0);
        asctime(std::localtime(&t));

        // Exit if ESC pressed.
        char WaitKeyStatus = waitKey(1);
        if(WaitKeyStatus=='s'||WaitKeyStatus=='S'){
          trackOk = true;
          rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
          tracker->init(fore, bbox);
          Roi = fore(bbox);
          resize(Roi,Roi,Size(200,200));
        }

        if ((WaitKeyStatus == 'a' || WaitKeyStatus == 'A')&&trackOk) //Save picture
            {

                imageName = "A/A_" +to_string(t);//+ to_string(a);
                imwrite(imageName + ".png", Roi);

            }

            if ((WaitKeyStatus == 'e' || WaitKeyStatus == 'E')&&trackOk) //Save picture
            {
                imageName = "E/E" + to_string(t);
                imwrite(imageName + ".png", Roi);

            }

            if ((WaitKeyStatus == 'i' || WaitKeyStatus == 'I')&&trackOk) //Save picture
            {
                imageName = "I/I" + to_string(t);
                imwrite(imageName + ".png", Roi);

            }

            if ((WaitKeyStatus == 'o' || WaitKeyStatus == 'O')&&trackOk) //Save picture
            {
                imageName = "O/O" + to_string(t);
                imwrite(imageName + ".png", Roi);

            }

            if ((WaitKeyStatus == 'u' || WaitKeyStatus == 'U')&&trackOk) //Save picture
            {
                imageName = "U/U" + to_string(t);
                imwrite(imageName + ".png", Roi);

            }

            if ((WaitKeyStatus == 'n' || WaitKeyStatus == 'N')) //Save picture
            {
                imageName = "Nada/Nada" + to_string(t);
                imwrite(imageName + ".png", Roi);

            }

            if (WaitKeyStatus == 'q' || WaitKeyStatus == 'Q' || WaitKeyStatus == 27) //Quit
            {
                destroyAllWindows();
                break;
            }

    }
}
