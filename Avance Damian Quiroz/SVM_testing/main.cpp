#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/ml.hpp>

#define CV_LOAD_IMAGE_GRAYSCALE 0
using namespace cv::ml;
using namespace cv;
using namespace std;

string pathName = "SVM_data/";
string test_pathName = "SVM_data/";
int SZ = 200;
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

void loadTrainTestLabel(string &pathName, vector<Mat> &trainCells, vector<Mat> &testCells, vector<int> &trainLabels, vector<int> &testLabels)
{
    /// Read all images
    Mat vocalImg;
    Mat test_vocalImg;
    string	imgName = pathName;
    string	test_imgName = test_pathName;
    int ImgCount = 0;
    vector<string> ftrain; // std::string in opencv2.4, but cv::String in 3.0
    vector<string>::iterator it;
    string path =pathName+"A/A*";
    cv::glob(path,ftrain,false);
    int i=0;
    for (it = ftrain.begin(); it != ftrain.end(); it++,i++)
    {
        /// Train A
        imgName = *it;
        vocalImg = imread(imgName, CV_LOAD_IMAGE_GRAYSCALE);
        if(i<ftrain.size()/2){
            trainCells.push_back(vocalImg);
            trainLabels.push_back(1);
        }else{
            testCells.push_back(vocalImg);
            testLabels.push_back(1);
        }

    }
    path =pathName+"I/I*";
    cv::glob(path,ftrain,false);
    i=0;
    for(it = ftrain.begin(); it != ftrain.end(); it++,i++)
    {
        /// Train B
        imgName =*it;
        vocalImg = imread(imgName, CV_LOAD_IMAGE_GRAYSCALE);
        if(i<ftrain.size()/2){
            trainCells.push_back(vocalImg);
            trainLabels.push_back(2);
        }else{
            testCells.push_back(vocalImg);
            testLabels.push_back(2);
        }

    }
    /*
    for (int i = 1; i <= 250; i++)
    {
        /// Train C
        imgName += "C/C";
        imgName += to_string(i);
        imgName += ".png";
        vocalImg = imread(imgName, CV_LOAD_IMAGE_GRAYSCALE);
        resize(vocalImg, vocalImg, Size(128, 128));
        trainCells.push_back(vocalImg);
        trainLabels.push_back(3);
        ImgCount++;
        imgName = pathName;
    }
    for (int i = 1; i <= 113; i++)
    {
        /// Train Nada
        imgName += "Nada/Nada";
        imgName += to_string(i);
        imgName += ".png";
        vocalImg = imread(imgName, CV_LOAD_IMAGE_GRAYSCALE);
        resize(vocalImg, vocalImg, Size(128, 128));
        trainCells.push_back(vocalImg);
        trainLabels.push_back(4);
        ImgCount++;
        imgName = pathName;
    }

    for (int i = 1; i <= 20; i++)
    {
        /// Test A
        test_imgName += "A/A";
        test_imgName += to_string(i);
        test_imgName += ".png";
        test_vocalImg = imread(test_imgName, CV_LOAD_IMAGE_GRAYSCALE);
        resize(test_vocalImg, test_vocalImg, Size(128, 128));
        testCells.push_back(test_vocalImg);
        testLabels.push_back(1);
        test_imgName = test_pathName;

        /// Test B
        test_imgName += "B/B";
        test_imgName += to_string(i);
        test_imgName += ".png";
        test_vocalImg = imread(test_imgName, CV_LOAD_IMAGE_GRAYSCALE);
        resize(test_vocalImg, test_vocalImg, Size(128, 128));
        testCells.push_back(test_vocalImg);
        testLabels.push_back(2);
        test_imgName = test_pathName;

        /// Test C
        test_imgName += "C/C";
        test_imgName += to_string(i);
        test_imgName += ".png";
        test_vocalImg = imread(test_imgName, CV_LOAD_IMAGE_GRAYSCALE);
        resize(test_vocalImg, test_vocalImg, Size(128, 128));
        testCells.push_back(test_vocalImg);
        testLabels.push_back(3);
        test_imgName = test_pathName;

        /// Test Nada
        test_imgName += "Nada/Nada";
        test_imgName += to_string(i);
        test_imgName += ".png";
        test_vocalImg = imread(test_imgName, CV_LOAD_IMAGE_GRAYSCALE);
        resize(test_vocalImg, test_vocalImg, Size(128, 128));
        testCells.push_back(test_vocalImg);
        testLabels.push_back(4);
        test_imgName = test_pathName;
    }
    */

    //cout << "Image Count : " << ImgCount << endl;
}

void CreateDeskewedTrainTest(vector<Mat> &deskewedTrainCells, vector<Mat> &deskewedTestCells, vector<Mat> &trainCells, vector<Mat> &testCells) {


    for (int i = 0; i < trainCells.size(); i++) {

        Mat deskewedImg = deskew(trainCells[i]);
        deskewedTrainCells.push_back(deskewedImg);
    }

    for (int i = 0; i < testCells.size(); i++) {

        Mat deskewedImg = deskew(testCells[i]);
        deskewedTestCells.push_back(deskewedImg);
    }
}

HOGDescriptor hog(
    Size(200, 200), //winSize
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
void CreateTrainTestHOG(vector<vector<float> > &trainHOG, vector<vector<float> > &testHOG, vector<Mat> &deskewedtrainCells, vector<Mat> &deskewedtestCells) {

    for (int y = 0; y < deskewedtrainCells.size(); y++) {
        vector<float> descriptors;
        hog.compute(deskewedtrainCells[y], descriptors);
        trainHOG.push_back(descriptors);
    }

    for (int y = 0; y < deskewedtestCells.size(); y++) {

        vector<float> descriptors;
        hog.compute(deskewedtestCells[y], descriptors);
        testHOG.push_back(descriptors);
    }
}
void ConvertVectortoMatrix(vector<vector<float> > &trainHOG, vector<vector<float> > &testHOG, Mat &trainMat, Mat &testMat)
{

    int descriptor_size = trainHOG[0].size();

    for (int i = 0; i < trainHOG.size(); i++) {
        for (int j = 0; j < descriptor_size; j++) {
            trainMat.at<float>(i, j) = trainHOG[i][j];
        }
    }
    for (int i = 0; i < testHOG.size(); i++) {
        for (int j = 0; j < descriptor_size; j++) {
            testMat.at<float>(i, j) = testHOG[i][j];
        }
    }
}

void getSVMParams(SVM *svm)
{
    cout << "Kernel type     : " << svm->getKernelType() << endl;
    cout << "Type            : " << svm->getType() << endl;
    cout << "C               : " << svm->getC() << endl;
    cout << "Degree          : " << svm->getDegree() << endl;
    cout << "Nu              : " << svm->getNu() << endl;
    cout << "Gamma           : " << svm->getGamma() << endl;
}

Ptr<SVM> svmInit(float C, float gamma)
{
    Ptr<SVM> svm = SVM::create();
    svm->setGamma(gamma);
    svm->setC(C);
    svm->setKernel(SVM::RBF);
    svm->setType(SVM::C_SVC);

    return svm;
}

void svmTrain(Ptr<SVM> svm, Mat &trainMat, vector<int> &trainLabels)
{
    Ptr<TrainData> td = TrainData::create(trainMat, ROW_SAMPLE, trainLabels);
    svm->train(td);
    svm->save("SVM_machine/classifierModel.yml");
}

void svmPredict(Ptr<SVM> svm, Mat &testResponse, Mat &testMat)
{
    svm->predict(testMat, testResponse);
}

void SVMevaluate(Mat &testResponse, float &count, float &accuracy, vector<int> &testLabels)
{
    for (int i = 0; i < testResponse.rows; i++)
    {
        cout << testResponse.at<float>(i,0) << " " << testLabels[i] << endl;
        if (testResponse.at<float>(i, 0) == testLabels[i])
            count = count + 1;
    }
    accuracy = (count / testResponse.rows) * 100;
}


int main()
{
    vector<Mat> trainCells;
    vector<Mat> testCells;
    vector<int> trainLabels;
    vector<int> testLabels;
    loadTrainTestLabel(pathName, trainCells, testCells, trainLabels, testLabels);

    vector<Mat> deskewedTrainCells;
    vector<Mat> deskewedTestCells;
    CreateDeskewedTrainTest(deskewedTrainCells, deskewedTestCells, trainCells, testCells);

    std::vector<std::vector<float> > trainHOG;
    std::vector<std::vector<float> > testHOG;
    CreateTrainTestHOG(trainHOG, testHOG, deskewedTrainCells, deskewedTestCells);

    int descriptor_size = trainHOG[0].size();
    cout << "Descriptor Size : " << descriptor_size << endl;

    Mat trainMat(trainHOG.size(), descriptor_size, CV_32FC1);
    Mat testMat(testHOG.size(), descriptor_size, CV_32FC1);

    ConvertVectortoMatrix(trainHOG, testHOG, trainMat, testMat);

    //C = 20, gamma = 0.0007 -> 93.75%
    float C = 20, gamma = 0.0007;

    Mat testResponse;
    Ptr<SVM> model = svmInit(C, gamma);

    ///////////  SVM Training  ////////////////
    svmTrain(model, trainMat, trainLabels);

    ///////////  SVM Testing  ////////////////
    svmPredict(model, testResponse, testMat);

    ////////////// Find Accuracy   ///////////
    float count = 0;
    float accuracy = 0;
    getSVMParams(model);
    SVMevaluate(testResponse, count, accuracy, testLabels);

    cout << "the accuracy is :" << accuracy << endl;
    system("PAUSE");
    return 0;
}
