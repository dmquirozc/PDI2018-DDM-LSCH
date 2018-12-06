#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
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
    //void initDetection();
    //void deteccionManos();

private:
    Ui::MainWindow *ui;
signals:
    void imagenLista(int);
    void detectaManos();

};

#endif // MAINWINDOW_H
