#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <string>

using namespace std;
using namespace cv;

int lowThreshold = 125;
Mat src = imread("Side Goal.png");

int main() {
    
    //cv::VideoCapture("Vision.mp4");
    
    //VideoCapture input("Vision.mp4");
    VideoCapture input;
    input.open("http://10.13.60.3/mjpg/video.mjpg");
    
    int loop = 0;
    
    namedWindow("My Window", 1);
    
    int VSub = 29; //42
    int VAdd = 222; //150
    
    int SSub = 235;
    int SAdd = 255;
    
    int HSub = 170;
    int HAdd = 255;
    
    createTrackbar("Value+", "My Window", &VAdd, 255);
    createTrackbar("Value-", "My Window", &VSub, 255);
    
    createTrackbar("Saturation+", "My Window", &SAdd, 255);
    createTrackbar("Saturation-", "My Window", &SSub, 255);
    
    createTrackbar("Brightness+", "My Window", &HSub, 255);
    createTrackbar("Brightness-", "My Window", &HAdd, 255);
    
    for(;;)
    {
        loop++;
        
        
        //if(!input.read(src))
        //break;
        input.read(src);
        
        //cv::imshow("img", src);
        //char c = cv::waitKey(30);
        
        //if(c == ' ')
        //break;
        
        
        
        //cv::Mat img(512, 512, CV_8UC3, cv::Scalar(0));
        
        //Mat src_gray;
        
        //cvtColor( src, src_gray, CV_BGR2GRAY );
        
        //resize(src, src, cv::Size(src.cols / 2.5, src.rows / 2.5));
        
        
        
        
        
        
        //blur(src, src, Size(3, 3));
        
        //cv::rectangle(img, cv::Point(112, 112), cv::Point(412, 412), 100, 3);
        
        //cv::putText(src, "Hello, World! (Cathal Sucks)", cv::Point(10, img.rows / 2), cv::FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(118, 185, 0), 2);
        
        
        
        
        Mat dst;
        
        
        
        
        /*
         createTrackbar("Value-", "My Window", &VSub, 255);
         createTrackbar("Value+", "My Window", &VAdd, 255);
         
         createTrackbar("Saturation-", "My Window", &SSub, 255);
         createTrackbar("Saturation+", "My Window", &SAdd, 255);
         
         createTrackbar("Brightness-", "My Window", &HSub, 255);
         createTrackbar("Brightness+", "My Window", &HAdd, 255);
         
         while (cv::waitKey(500)) {
         Scalar hsvLow = Scalar(HSub, SSub, VSub);
         Scalar hsvHigh = Scalar(HAdd, HAdd, VAdd);
         
         cv::inRange(src, hsvLow, hsvHigh, dst);
         
         //cv::Canny( dst, dst, lowThreshold, lowThreshold*3, 3);
         //cv::Sobel(src, dst, CV_32F, 1, 1);
         //cv::imwrite("HighGoalCanny.png", dst);
         
         cv::imshow("My Window", dst);
         }*/
        
        int BlockSize = 1;
        int kSize = 1;
        
        //cv::cornerHarris(dst, dst, blockSize, kSize, 0.04);
        
        //while (cv::waitKey(500)) {
        Scalar hsvLow = Scalar(HSub, SSub, VSub);
        Scalar hsvHigh = Scalar(HAdd, SAdd, VAdd);
        
        cv::inRange(src, hsvLow, hsvHigh, dst);
        
        
        
        
        
        
        
        
        //cv::cornerHarris(dst, dst, 2, 1, 0.04);
        
        
        
        //std::vector<cv::Point2f> cornersB;
        //cornersB.reserve(10);
        
        //cv::goodFeaturesToTrack(src, cornersB, 10, 0.05, 2, cv::Mat(), 3, false, 0.04);
        
        //Mat src_gray;
        
        //cvtColor( dst, src_gray, CV_BGR2GRAY );
        
        vector<vector<Point>> contours1;
        vector<vector<Point>> contours2;
        
        vector<Vec4i> hierarchy;
        
        Mat goalFiltered = imread("FilteredGoal.png");
        
        cvtColor( goalFiltered, goalFiltered, CV_BGR2GRAY );
        
        cv::findContours(goalFiltered, contours1, 2, 1);
        cv::findContours(dst, contours2, 2, 1);
        
        
        Mat out2;
        src.copyTo(out2);
        
        for(int i = 0; i < contours1.size(); i++)
        {
            cv::drawContours(out2, contours1, i, Scalar(255, 0, 255), 2, 8, hierarchy, 0, Point());
        }
        
        
        
        
        
        
        
        //cv::imshow("My window 2", out2);
        
        
        
        cv::findContours(goalFiltered, contours1, 2, 1);
        cv::findContours(dst, contours2, 2, 1);
        
        
        for(int i = 0; i < contours2.size(); i++)
        {
            //cv::drawContours(out, contours2, i, Scalar(255, 0, 255), 2, 8, hierarchy, 0, Point());
            double val = cv::matchShapes(contours2[i], contours1[0], 1,  0.0);
            //printf("attempt %d value = %f", i, val);
            if(val > 600) contours2.erase(contours2.begin() + i);
        }
        
        
        
        int largest = 0;
        int index = -1;
        
        for(int i = 0; i < contours2.size(); i++)
        {
            //cv::drawContours(out, contours2, i, Scalar(255, 0, 255), 2, 8, hierarchy, 0, Point());
            double val = cv::contourArea( contours2[i], true);
            //printf("size = %f", val);
            //if(val < 500) contours2.erase(contours2.begin() + i);
            
            double size = cv::boundingRect(contours2[i]).width;
            
            if(size > largest)
            {
                index = i;
                largest = size;
            }
        }
        
        
        
        
        
        Mat out;
        src.copyTo(out);
        
        
        cv::drawContours(out, contours2, index, Scalar(255, 0, 255), 2, 8, hierarchy, 0, Point());
        
        
        
        
        
        if(index > -1)
        {
            printf("size %d index %d", largest, index);
            
            int x = cv::boundingRect(contours2[index]).x;
            int y = cv::boundingRect(contours2[index]).y;
            
            //Rect Rect1= cv::boundingRect(contours2[index]);
            
            int width = cv::boundingRect(contours2[index]).width;
            int height = cv::boundingRect(contours2[index]).height;
            
            //printf("x = %d, y = %d", x, y);
            
            cv::rectangle(out, Point(x, y), Point(x+width, y+height), Scalar(255, 128, 0), 3, 2, 0);
            
            //printf("size %d index %d", largest, index);
            
            
            
            
            
            //RotatedRect rect = minAreaRect(contours2[index]);
            RotatedRect rect = minAreaRect(contours2[index]);
            //cv::imshow("Cathal smokes weed", rekt);
            Point2f vtx[4];
            rect.points(vtx);
            
            for(int i = 0; i < 4; i++)
            {
                line(out, vtx[i], vtx[(i+1)%4], Scalar(0, 128, 255), 3, LINE_AA);
            }
            
            
            
            
            
        }
        
        //cv::rectangle(out, Rect1, Scalar(255, 128, 0), 2, 2, 0);
        
        
        cv::imshow("LOL", out);
        
        //string tempLoop = to_string(loop);
        //string LoopText = "img" + tempLoop + ".png";
        
        //cv::imwrite(LoopText, out);
        
        cv::waitKey(30);
        
    }
    /*
     
     
     
     //printf("%f", cv::matchShapes(contours1, contours2, 1, 0.0));
     
     
     /*
     vector<Point2f> corners;
			  double qualityLevel = 0.75;//81
			  double minDistance = 0.1;
			  int blockSize = 3;
			  bool useHarrisDetector = true;
			  double k = 0.04;
			  int maxCorners = 20;
     
     
     
			  //createTrackbar("minDistance+", "My Window", &, 255);
     
			  /// Copy the source image
			  Mat copy;
			  copy = src.clone();
     
			  while(cv::waitKey(500))
			  {
     
     Scalar hsvLow = Scalar(HSub, SSub, VSub);
     Scalar hsvHigh = Scalar(HAdd, HAdd, VAdd);
     
     cv::inRange(src, hsvLow, hsvHigh, dst);
			  /// Apply corner detection
			  cv::goodFeaturesToTrack( dst,
     corners,
     maxCorners,
     qualityLevel,
     minDistance,
     Mat(),
     blockSize,
     useHarrisDetector,
     k );
     
			  //printf("x = %d, y = %d, Total = %d", corners[C1].x, corners[C1].y, corners.size());
     
     for(int i = 0; i < corners.size(); i++)
     {
     cv::circle(dst, corners[i], 10, Scalar(255, 255, 0), 1, LINE_8, 0);
     }
     
     cv::imshow("My Window", dst);
			  }
			  */
    
    
    
			 //cv::putText(dst, corners[0], cv::Point(10, dst.rows/2), cv::FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(118, 255, 0), 2);
    
    //std::cout << cornersB;
    
    //int C1 = 0;
    
    
    //printf("x = %d, y = %d, Total = %d", corners[C1].x, corners[C1].y, corners.size());
    
    /*
     for(int i = 0; i < corners.size(); i++)
     {
     cv::circle(dst, corners[i], 10, Scalar(255, 255, 0), 1, LINE_8, 0);
     }
     */
    
    //cv::circle(dst, corners[C1], 10, Scalar(255, 255, 0), 1, LINE_8, 0);
    
    //unsigned int result = cvLabel(&dst, labelImg, blobs);
    
    //cv::Canny( dst, dst, lowThreshold, lowThreshold*3, 3);
    //cv::Sobel(src, dst, CV_32F, 1, 1);
    //cv::imwrite("HighGoalCanny.png", dst);
    
    
    //}
    
    //cv::imshow("My Window", dst);
    
    //cv::imwrite("SideFilterOut.png", out);
    
    cv::waitKey();
    
    return 0;
}
