#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/core/ocl.hpp>
#include <opencv2/ml.hpp>
#define SZ 200

#define affineFlags WARP_INVERSE_MAP | INTER_LINEAR
using namespace cv::ml;
using namespace std;
using namespace cv;

#include <QtFontDatabaseSupport/QtFontDatabaseSupport>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool isVideoOpened=false;
    bool isCaptureTaked=false;
    bool handDetection=false;
    VideoCapture video;
    QImage* imagen[3];
    Mat* mat_imagen[3];
    QImage MatToQImage(const Mat& mat);
    QTimer* timer;
    void setText();
public slots:
    void videoListo();
    void textoListo();
    void initVideo();
    void handDetect();
    //void deteccionManos();

private:
    Ui::MainWindow *ui;
    Mat deskew(Mat& img);
    HOGDescriptor hog;
    char letra;
    Ptr<SVM> svm;
    Ptr<Tracker> tracker;
    Mat Roi;
    Rect2d roiBox;
    Mat backGround, foreGround, foreThresh, backMask,grayScale;
    QFont font;

signals:
    void imagenLista(int);
    void detectaManos();

};

#endif // MAINWINDOW_H
