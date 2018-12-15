#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc,char **argv){
	//Variable declaration
	cv::CascadeClassifier hand_cascade;
	cv::CascadeClassifier fist_cascade;
	
	std::vector<cv::Rect> hand,fist;
	
	cv::Point pt1,pt2;
	std::vector<cv::Vec4i> defects;
	
	std::vector<std::vector<cv::Point>> contours,hand_cnt,convex_hull_array;
	std::vector<cv::Point> cnt,convex_hull,hull2;
	std::vector<cv::Vec4i> hierarchy;
	
	cv::Mat fliped,source,blur,hsv,gray,bin0,bin1,eroded,element;
	cv::Mat binary,canvas,masked_hand,last_step,hand_trace;
	
	std::vector<cv::Mat> hsv_splited;
	
	cv::VideoCapture vid;
	
	int *low_thresh = new int,*high_thresh = new int;
	int *x_offset = new int,*y_offset = new int,*w_offset = new int,*h_offset = new int;
	int area,max_area,min_area,max_ind;
	
	// Loading Hand Cascade
	hand_cascade.load("files/Hand_haar_cascade.xml");
	fist_cascade.load("files/aGest.xml");
	element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3),cv::Point(-1,-1));
	vid.open(0);
	if(!vid.isOpened()){
		std::cerr << "Error: camera couldn't be opened." << std::endl;
		return -1;
	}
	
	// Creating Windows
	std::string winTitle = "Ventana de calibración";
	std::string tbTitle_L = "Umbral inferior";
	std::string tbTitle_H = "Umbral superior";
	std::string x_offsetT = "Eje x";
	std::string y_offsetT = "Eje y";
	std::string w_offsetT = "Ancho";
	std::string h_offsetT = "Alto";
	*low_thresh = 35;
	*high_thresh = 255;
	*x_offset = 35;
	*y_offset = 31;
	*w_offset = 26;
	*h_offset = 35;
	cv::namedWindow(winTitle);
	cv::createTrackbar(tbTitle_L, winTitle , low_thresh, 255);
	cv::createTrackbar(tbTitle_H, winTitle, high_thresh, 255);
	cv::createTrackbar(x_offsetT, winTitle, x_offset, 100);
	cv::createTrackbar(y_offsetT, winTitle, y_offset, 100);
	cv::createTrackbar(w_offsetT, winTitle, w_offset, 100);
	cv::createTrackbar(h_offsetT, winTitle, h_offset, 100);
	
	while(true){
		vid >> fliped;
		cv::flip(fliped,source,1);
		
		cv::cvtColor(source,hsv,cv::COLOR_BGR2HSV);
		cv::cvtColor(source,gray,cv::COLOR_BGR2GRAY);
		cv::GaussianBlur(source,blur,cv::Size(5,5),2,2);
		cv::split(hsv,hsv_splited);
		cv::threshold(hsv_splited[0],bin0,*low_thresh,*high_thresh,cv::THRESH_OTSU);
		cv::threshold(hsv_splited[1],bin1,*low_thresh,*high_thresh,cv::THRESH_OTSU);
		cv::erode(bin0,eroded,element);
		hand_cascade.detectMultiScale(bin1,hand);
		fist_cascade.detectMultiScale(gray,fist);
		eroded.copyTo(binary);
		
		eroded.copyTo(canvas);
		eroded.copyTo(masked_hand);
		source.copyTo(hand_trace);
		canvas.setTo(0);
		masked_hand.setTo(0);
		hand_trace.setTo(0);
		
		for(cv::Rect r : hand){//r = {x,y,w,h}
			pt1 = cv::Point(r.x+*x_offset-50,r.y+*y_offset-50);
			pt2 = cv::Point(r.x+r.width+*w_offset,r.y+r.height+*h_offset);
			cv::rectangle(canvas,pt1,pt2,255,-1);
			cv::rectangle(binary,pt1,pt2,255,1);
			cv::rectangle(source,pt1,pt2,255,1);
		}
		
		for(cv::Rect r : fist){//r = {x,y,w,h}
			pt1 = cv::Point(r.x+*x_offset-50,r.y+*y_offset-50);
			pt2 = cv::Point(r.x+r.width+*w_offset,r.y+r.height+*h_offset);
			cv::rectangle(canvas,pt1,pt2,255,-1);
			cv::rectangle(binary,pt1,pt2,255,1);
			cv::rectangle(source,pt1,pt2,255,1);
		}
		
		cv::bitwise_and(canvas,eroded,masked_hand);
		cv::GaussianBlur(masked_hand,last_step,cv::Size(7,7),2,2);
		cv::findContours(last_step,contours,hierarchy,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
		max_area = 0;
		min_area = 0;
		max_ind = 0;
		
		for(int i = 0;i < contours.size();i++){
			cnt = contours[i];
			area = cv::contourArea(cnt);
			if(area > max_area){
				max_area = area;
				max_ind = i;
			}
		}
		
		if(contours.size() > 0){
			hand_cnt.clear();
			convex_hull.clear();
			hull2.clear();
			hand_cnt.push_back(contours.at(max_ind));
			cv::drawContours(hand_trace,hand_cnt,-1,cv::Scalar(255,255,255),3);
			cv::convexHull(hand_cnt.at(0),convex_hull);
			convex_hull_array.clear();
			convex_hull_array.push_back(convex_hull);
			cv::drawContours(hand_trace, convex_hull_array,-1,cv::Scalar(255, 255, 0),3);

			cv::convexHull(hand_cnt.at(0),hull2,false,false);
			if(hull2.size() == hand_cnt.at(0).size()){
				std::cout << "yes!" << std::endl;
				//cv::convexityDefects(hand_cnt.at(0),hull2,defects);
			}

		}
		/*
		for(cv::Vec4i i : defects){
			s,e,f,d = defects[i,0]
			start = tuple(hand_cnt[s][0])
			end = tuple(hand_cnt[e][0])
			far = tuple(hand_cnt[f][0])
			FarDefect.append(far)
			cv2.line(hand_trace,start,end,[0,255,0],1)
			cv2.circle(hand_trace,far,10,[100,255,255],3)
		}
		*/
		//Show images
		cv::imshow("Original (press \"q\" to exit)",source);
		cv::imshow("Binaria (press \"q\" to exit)",binary);
		cv::imshow("Contorno + Convexo (press \"q\" to exit)",hand_trace);
		
		if(cv::waitKey(1) == 'q') break;
	}
	vid.release();
	return 0;
}
