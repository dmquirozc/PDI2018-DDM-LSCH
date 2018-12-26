#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>
using namespace cv::ml;
using namespace std;
using namespace cv;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* Chat */
    ui->lineEdit->setFocusPolicy(Qt::StrongFocus);
    ui->textEdit->setFocusPolicy(Qt::NoFocus);
    ui->textEdit->setReadOnly(true);

    hog = HOGDescriptor(
                Size(200, 200),
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
    svm = Algorithm::load<SVM>("SVM/classifierModel.yml");
    tracker = TrackerCSRT::create();
    Roi=NULL;
    previousLetter =" ";
    roiBox = Rect2d(100,50,200,200);
    timer=new QTimer();
    timer->start(40);
    for(int i =0;i<3;i++){
        mat_imagen[i]=new Mat(480,640, CV_8UC3, Scalar(255,255,255));;
        imagen[i] = new QImage();
    }
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(videoListo()));
    QObject::connect(this,SIGNAL(nadaPressed()),this,SLOT(returnPressed()));
    setText();

}

void MainWindow::setText(){
    if (QFontDatabase::addApplicationFont(":/FontAwsomeSolid") < 0)
        qWarning() << "FontAwesome cannot be loaded !";
    font.setFamily("FontAwsome");
    font.setPixelSize(32);
    ui->pushButton->setFont(font);

    ui->pushButton->setText("");
}
MainWindow::~MainWindow()
{
    delete ui;
}

Mat MainWindow::deskew(Mat& img) {
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

void MainWindow::initVideo(){
    //setText();
    if(isVideoOpened==false){
         video.open(0);
         /*La primera imagen es el BackGround*/
         video >> backGround;
         cv::resize(backGround,backGround,Size(640,480),0,0,INTER_LINEAR);
         cvtColor(backGround, backGround, CV_BGR2GRAY);
         isVideoOpened=true;
         ui->actionIniciarCamara->setText("Apagar cámara");
         emit videoListo();
    }else{
        video.release();
        //delete(mat_imagen[0]);
        mat_imagen[0]=new Mat(480,640, CV_8UC3, Scalar(255,255,255));
        ui->actionIniciarCamara->setText("Iniciar Cámara");
        isVideoOpened=false;
        emit videoListo();

    }

}

void MainWindow::appendMessage(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty())
        return;

    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 2, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(message);
    QScrollBar *bar = ui->textEdit->verticalScrollBar();

    bar->setValue(bar->maximum());

}

void MainWindow::returnPressed()
{
    QString text = ui->lineEdit->text();
    if (text.isEmpty())
        return;

    if (text.startsWith(QChar('/'))) {
        QColor color = ui->textEdit->textColor();
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->append(tr("! Unknown command: %1")
                         .arg(text.left(text.indexOf(' '))));
        ui->textEdit->setTextColor(color);
    } else {

        appendMessage("Cliente", text);
    }

    ui->lineEdit->clear();
}



void MainWindow::videoListo(){
    if(isVideoOpened){
        video >> *mat_imagen[0];
        cv::resize(*mat_imagen[0],*mat_imagen[0],Size(640,480),0,0,INTER_LINEAR);
        if(!handDetection){
           //
            putText(*mat_imagen[0], "Ponga su mano en el cuadro Rojo", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
            rectangle(*mat_imagen[0], roiBox, Scalar( 0, 0, 255 ), 2, 1 );
            cvtColor(*mat_imagen[0], grayScale, CV_BGR2GRAY);
            absdiff(grayScale, backGround, foreGround);
            threshold(foreGround, backMask, 10, 255, THRESH_BINARY);
            Mat structuringElement = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
            morphologyEx(backMask, backMask, cv::MORPH_CLOSE, structuringElement);
            bitwise_and(grayScale, backMask, foreGround);
             Roi = (foreGround)(roiBox);
            *imagen[0] = MatToQImage(*mat_imagen[0]);
            QImage *out = new QImage(*imagen[0]);
            QLabel *label = new QLabel;
            label->setPixmap(QPixmap::fromImage(*out, Qt::AutoColor));
            ui->video->setWidget(label);
        }else{
            /*Algoritmo para segmentacion de fondo */
            cvtColor(*mat_imagen[0], grayScale, CV_BGR2GRAY);
            absdiff(grayScale, backGround, foreGround);
            threshold(foreGround, backMask, 10, 255, THRESH_BINARY);
            Mat structuringElement = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
            morphologyEx(backMask, backMask, cv::MORPH_CLOSE, structuringElement);
            bitwise_and(grayScale, backMask, foreGround);
            /*Fondo segmentado*/
            imshow("foreGround",foreGround);
            int limit_x = (*mat_imagen[0]).cols;
            int limit_y = (*mat_imagen[0]).rows;
            double boxlimit =roiBox.x;
            double boylimit =roiBox.y;
            tracker->update((*mat_imagen[0]), roiBox);

            if(roiBox.x+roiBox.width>=limit_x){
                roiBox=Rect2d(roiBox.x,roiBox.y,limit_x-boxlimit-10,roiBox.height);
            }
            if(roiBox.x<10){
                  roiBox=Rect2d(10,roiBox.y,roiBox.width,roiBox.height);
            }

            if(roiBox.y+roiBox.height>limit_y){
                roiBox=Rect2d(roiBox.x,roiBox.y,roiBox.width,limit_y-boylimit-10);
            }

            if(roiBox.y<10){
               roiBox=Rect2d(roiBox.x,10,roiBox.width,roiBox.height);
            }
            Roi = (foreGround)(roiBox);
            cv::resize(Roi,Roi,Size(200,200));
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
            rectangle((*mat_imagen[0]), roiBox, Scalar( 255, 0, 0 ), 2, 1 );
            QString letra;
            if (testResponse.at<float>(0, 0) == 1){
                 letra="A";
            }
            if (testResponse.at<float>(0, 0) == 2){
                 letra="E";
            }
            if (testResponse.at<float>(0, 0) == 3){
                letra="I";
            }
            if (testResponse.at<float>(0, 0) == 4){
                letra ="\n";
            }
            if(letra!=previousLetter){
                if(letra==QString("\n")){
                    emit nadaPressed();
                }else{
                    ui->lineEdit->setText(ui->lineEdit->text()+QString(letra));
                    previousLetter=letra;
                }

            }
            imshow("Roi",Roi);
            //imshow("Tracking", (*mat_imagen[0]));
            *imagen[0] = MatToQImage(*mat_imagen[0]);
            QImage *out = new QImage(*imagen[0]);
            QLabel *label = new QLabel;
            label->setPixmap(QPixmap::fromImage(*out, Qt::AutoColor));
            ui->video->setWidget(label);
        }
    }else{
        cv::resize(*mat_imagen[0],*mat_imagen[0],Size(640,480),0,0,INTER_LINEAR);
        *imagen[0] = MatToQImage(*mat_imagen[0]);
        QImage *out = new QImage(*imagen[0]);
        QLabel *label = new QLabel;
        label->setPixmap(QPixmap::fromImage(*out, Qt::AutoColor));
        ui->video->setWidget(label);
    }


}

void MainWindow::textoListo(){

}
void MainWindow::handDetect(){
    if(!handDetection && isVideoOpened){
        tracker->init(*mat_imagen[0], roiBox);
        Roi = (foreGround)(roiBox);
        cv::resize(Roi,Roi,Size(200,200));
        handDetection = true;
    }else{
        handDetection=false;
        Roi=NULL;
        roiBox = Rect2d(100,50,200,200);
        if(isVideoOpened){
            tracker->clear();
        }
    }

}

QImage MainWindow::MatToQImage(const Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
} // MatToQImage()
