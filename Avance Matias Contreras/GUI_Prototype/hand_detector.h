#ifndef HAND_DETECTOR_H
#define HAND_DETECTOR_H

#include <QLabel>
#include <QImage>
#include <QDebug>
#include <QTimer>
#include <QPixmap>
#include <QMainWindow>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>

namespace Ui {
class Hand_Detector;
}

class Hand_Detector : public QMainWindow
{
    Q_OBJECT

public:
    explicit Hand_Detector(QWidget *parent = 0);
    ~Hand_Detector();

    QImage MatToQImage(const cv::Mat& mat);

public slots:
    void videoCharge();
    void handCharge();

private slots:
    void videoCamera();

private:
    Ui::Hand_Detector *ui;
    QImage video[2];
    QTimer *freshTime;

    bool OK;
    cv::Mat img, HSV;
    cv::Mat handAnal;
    cv::VideoCapture vid;

    /* Mezcla de Gaussianas */
    cv::Mat fg, gaussian;
    int historyMOG = 255;
    double varThreshold = 16;
    double learningRate = 0.005;
    bool shadowDetection = true;
    cv::Ptr<cv::BackgroundSubtractorMOG2> mog;

};

#endif // HAND_DETECTOR_H
