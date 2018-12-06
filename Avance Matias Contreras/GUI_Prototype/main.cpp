#include "hand_detector.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Hand_Detector w;
    w.show();

    return a.exec();
}
