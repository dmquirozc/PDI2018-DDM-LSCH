#include <iostream>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;        /* Espacio de nombres */

/*Grupo 3: Damián Quiroz
           Marcelo Cárdenas
           Ignacio Gaete
           Francisco Frez
           Matías Contreras */

int main(int argc, char *argv[]) {
    if(argc != 3) {
        cerr << "Usage: ./programa umbral gamma" << endl;
        return 1;
    }

    cv::VideoCapture vid;       /* Captura de vídeo */
    vid.open(0);

    if(!vid.isOpened()) {
        cerr << "Error opening input." << endl;
        return 1;
    }

    cv::Mat img;        /* Matriz para imagen */
    cv::Mat dst;        /* Matriz para luminosidad */

    float elev;         /* Variables auxiliares y parámetros */
    float encoded;
    int umbral = std::atoi(argv[1]);                            /* Parámetro: Umbral */
    float gamma = static_cast<float>(std::atof(argv[2]));       /* Parámetro: Factor gamma */

    while(1) {
        vid >> img;
        cv::cvtColor(img, dst, cv::COLOR_BGR2YCrCb);    /* Conversión de color para ocupar croma */

        uchar *data = dst.data;
        int step = static_cast<int>(dst.step);      /* Obtiene data desde matriz destino */
        int channels = dst.channels();

        for(int i = 0; i < dst.rows; ++i){
            for(int j = 0; j < dst.cols; ++j){
                if(static_cast<int>(data[i*step + j*channels + 0]) <= umbral)       /* Condición: nivel <= umbral */
                    data[i*step + j*channels + 0] = 0;
                else{
                    /* Corrección Gamma: uso de flotantes para ocupar menor espacio en memoria */
                    float umbral_float = static_cast<float>(umbral);
                    float aux = static_cast<float>(data[i*step + j*channels + 0]);
                    elev = static_cast<float>(pow(((aux - umbral_float)/(static_cast<float>(255.0) - umbral_float)), gamma));
                    encoded = static_cast<float>(elev*(static_cast<float>(255.0) - umbral_float));
                    data[i*step + j*channels + 0] = static_cast<uchar>(encoded);
                }
            }
        }

        cv::cvtColor(dst, img, cv::COLOR_YCrCb2BGR);        /* Conversión de color mostrar la imagen en BGR */
        cv::imshow("Gamma Correction - Grupo 3", img);      /* Desplega la imagen */

        if(cv::waitKey(10) != -1)
            break;
    }

    vid.release();
    return 0;
}
