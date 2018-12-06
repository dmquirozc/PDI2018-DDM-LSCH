#include "hand_detector.h"
#include "ui_hand_detector.h"

Hand_Detector::Hand_Detector(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Hand_Detector){

    ui->setupUi(this);
    vid.open(0);
    if(!vid.isOpened())
        qDebug() << "Error opening camera.";

    OK = false;
    for(int i=0; i<2; i++) video[i] = QImage();
    freshTime = new QTimer(this);
    freshTime->start(50);

    mog = cv::createBackgroundSubtractorMOG2(historyMOG, varThreshold, shadowDetection);
    QObject::connect( freshTime, SIGNAL(timeout()), this, SLOT(videoCamera()) );
}


void Hand_Detector::videoCamera(){
    vid >> img;
    video[0] = MatToQImage(img);
    videoCharge();
    if (OK == true) handCharge();
}


void Hand_Detector::videoCharge(){
    QLabel *camera = new QLabel;
    camera->setPixmap(QPixmap::fromImage(video[0].scaled(389, 319), Qt::AutoColor));
    ui->scrollArea->setWidget(camera);
}


void Hand_Detector::handCharge(){
    OK = true;
    vid >> img;
    cv::cvtColor(img, gaussian, cv::COLOR_BGR2GRAY);
    mog->apply(gaussian, fg, learningRate);

    cv::cvtColor(img, HSV, cv::COLOR_BGR2HSV);
    cv::inRange(HSV, cv::Scalar(0,48,80), cv::Scalar(50,255,255), HSV);
    cv::bitwise_and(fg, HSV, handAnal);

    video[1] = MatToQImage(handAnal);

    QLabel *handCam = new QLabel;
    handCam->setPixmap(QPixmap::fromImage(video[1].scaled(379, 319), Qt::AutoColor));
    ui->scrollArea_2->setWidget(handCam);
}


QImage Hand_Detector::MatToQImage(const cv::Mat& mat){
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type() == CV_8UC1){
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*) mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type() == CV_8UC3){
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*) mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    } else{
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
} // MatToQImage()


Hand_Detector::~Hand_Detector(){
    delete ui;
}
