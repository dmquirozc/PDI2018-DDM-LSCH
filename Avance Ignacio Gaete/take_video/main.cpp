#include <opencv2/opencv.hpp>
#include <iostream>
 
using namespace std;
using namespace cv;
 
int main(){
 
  // Create a VideoCapture object and use camera to capture the video
  VideoCapture cap(0); 
 
  // Check if camera opened successfully
  if(!cap.isOpened())
  {
    cout << "Error opening video stream" << endl; 
    return -1; 
  } 
 
  // Default resolution of the frame is obtained.The default resolution is system dependent. 
  int frame_width = cap.get(CAP_PROP_FRAME_WIDTH); 
  int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT); 
   
  // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file. 
  VideoWriter video("video/output_video.avi",VideoWriter::fourcc('M','J','P','G'),25, Size(frame_width,frame_height)); 
  while(1)
  { 
    Mat frame,fliped;
     
    // Capture frame-by-frame 
    cap >> frame;
    flip(frame,fliped,1);
  
    // If the frame is empty, break immediately
    if (fliped.empty())
      break;
     
    // Write the frame into the file 'outcpp.avi'
    video.write(fliped);
    
    // Display the resulting frame    
    imshow( "Frame", fliped );
  
    // Press  ESC on keyboard to  exit
    char c = (char)waitKey(1);
    if( c == 'q' ) 
      break;
  }
 
  // When everything done, release the video capture and write object
  cap.release();
  video.release();
 
  // Closes all the windows
  destroyAllWindows();
  return 0;
}
