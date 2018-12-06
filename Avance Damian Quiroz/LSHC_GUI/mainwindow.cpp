#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer=new QTimer();
    timer->start(20);
    for(int i =0;i<3;i++){
        mat_imagen[i]=new Mat();
        imagen[i] = new QImage();
    }
     QObject::connect(timer,SIGNAL(timeout()),this,SLOT(videoListo()));
    setText();

}

void MainWindow::setText(){
    if (QFontDatabase::addApplicationFont(":/FontAwsomeSolid") < 0)
        qWarning() << "FontAwesome cannot be loaded !";
    QFont font;
    font.setFamily("FontAwesome");
    font.setPixelSize(32);

    ui->pushButton->setFont(font);
    ui->pushButton->setText("\uf083");
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initVideo(){
    if(isVideoOpened==false){
         video.open(0);
         isVideoOpened=true;
         ui->actionIniciarCamara->setText("Apagar cámara");
         emit videoListo();
    }else{
        video.release();
        ui->actionIniciarCamara->setText("Iniciar Cámara");
        isVideoOpened=false;
    }

}

void MainWindow::videoListo(){
    if(isVideoOpened){
        video >> *mat_imagen[0];
        *imagen[0] = MatToQImage(*mat_imagen[0]);
        QImage *out = new QImage(imagen[0]->scaled(460,320));
        QLabel *label = new QLabel;
        label->setPixmap(QPixmap::fromImage(*out, Qt::AutoColor));
        ui->video->setWidget(label);


    }


}

void MainWindow::textoListo(){

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
