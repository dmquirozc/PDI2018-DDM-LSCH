#include <iostream>
#include <opencv2/opencv.hpp>

void flipImage(cv::Mat&);
void swap(unsigned char&,unsigned char&);

int main(int argc,char **argv){
	cv::VideoCapture video;
	cv::Mat img;
	int keyNumb, imgNumb = 0;
	std::string windowsName = "Webcam. Press \"q\" to exit or \"SPACE BAR\" to take a photo";
	std::string imagesName = "";
	video.open(0);
	if(!video.isOpened()){
		std::cerr << "Camera error: Video couldn't be opened." << std::endl;
		return -1;
	}
	cv::namedWindow(windowsName,cv::WINDOW_AUTOSIZE);//create window
	while(1){
		video >> img;
		if(img.empty()){
			std::cout << "Error: Empty image." << std::endl;
			return -1;
		}
		flipImage(img);
		keyNumb = cv::waitKey(1);//detect key pressed by the user
		if(keyNumb != -1)
			std::cout << "Key pressed (ASCII): " << keyNumb << std::endl;
		if(keyNumb == 'q')//exit
			break;
		if(keyNumb == ' '){
			imagesName += "./photos/img";
			imagesName += std::to_string(imgNumb);
			imagesName += ".png";
			std::cout << imagesName << std::endl;
			cv::imwrite(imagesName,img);
			imagesName = "";
			imgNumb++;
		}
		cv::imshow(windowsName,img);
	}
	video.release();
	return 0;
}

void flipImage(cv::Mat &image){
	int cols=image.cols;
	int rows=image.rows;
	int step=image.step;
	int channels=image.channels();
	unsigned char *data=image.data;
	for(int i=0;i<rows;i++)
		for(int j=0;j<cols/2;j++)
			for(int k=0;k<channels;k++)
				swap(data[i*step+j*channels+k],data[i*step+(cols-j)*channels+k]);
	return;
}

void swap(unsigned char &a,unsigned char &b){
	unsigned char temp;
	temp=a;
	a=b;
	b=temp;
	return;
}
