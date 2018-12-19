#include <iostream>
#include <opencv2/opencv.hpp>

#define CAM_WIDTH  400
#define CAM_HEIGHT 400

//Global variables
cv::CascadeClassifier hand_cascade;
int *x_offset = new int;
int *y_offset = new int;
int *w_offset = new int;
int *h_offset = new int;

//Functions
void find_hands(cv::Mat source,std::vector<cv::Rect> &hands);
void draw_hands(cv::Mat &source,std::vector<cv::Rect> hands);

//Main
int main(int argc,char **argv){
	//Variable declaration	
	cv::Mat fliped,source;
	std::vector<cv::Rect> hands;
	cv::VideoCapture vid;
	
	//Loading classifier
	hand_cascade.load("files/Hand_haar_cascade.xml");
	
	vid.open(0);
	if(!vid.isOpened()){
		std::cerr << "Error: camera couldn't be opened." << std::endl;
		return -1;
	}
	vid.set(cv::CAP_PROP_FRAME_WIDTH,CAM_WIDTH);
	vid.set(cv::CAP_PROP_FRAME_HEIGHT,CAM_HEIGHT);
	
	// Creating Windows
	std::string winTitle = "Ventana de calibración";
	std::string originalTitle = "Original (press \"q\" to exit)";
	std::string x_offsetT = "Eje x";
	std::string y_offsetT = "Eje y";
	std::string w_offsetT = "Ancho";
	std::string h_offsetT = "Alto";
	*x_offset = 0;
	*y_offset = 0;
	*w_offset = 0;
	*h_offset = 0;
	cv::namedWindow(winTitle);
	cv::namedWindow(originalTitle);
	cv::createTrackbar(x_offsetT, winTitle, x_offset, 100);
	cv::createTrackbar(y_offsetT, winTitle, y_offset, 100);
	cv::createTrackbar(w_offsetT, winTitle, w_offset, 100);
	cv::createTrackbar(h_offsetT, winTitle, h_offset, 100);
	cv::moveWindow(winTitle,800,100);
	cv::moveWindow(originalTitle,100,50);
	
	//Main loop
	while(true){
		vid >> fliped;
		cv::flip(fliped,source,1);
		
		//Find hands
		find_hands(source,hands);
		
		//Draw them all
		draw_hands(source,hands);
		
		//Show images
		cv::imshow(originalTitle,source);
		if(cv::waitKey(1) == 'q') break;
	}
	vid.release();
	return 0;
}

void find_hands(cv::Mat source,std::vector<cv::Rect> &hands){
	cv::Mat blur,thresh;
	std::vector<cv::Mat> splited;
	
	cv::medianBlur(source,blur,5);
	cv::cvtColor(blur,blur,cv::COLOR_BGR2HSV);
	cv::split(blur,splited);
	cv::threshold(splited[1],thresh,50,200,cv::THRESH_TRIANGLE);
		
	hand_cascade.detectMultiScale(thresh,hands);
}

void draw_hands(cv::Mat &source,std::vector<cv::Rect> hands){
	for(cv::Rect r : hands){//r = {x,y,width,height}
		r.x += *x_offset;
		r.y += *y_offset;
		r.width  += 14+*w_offset;
		r.height += 40+*h_offset;
		cv::rectangle(source,r,cv::Scalar(255,0,0));
	}
}
