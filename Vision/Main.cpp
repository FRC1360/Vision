#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <string>

using namespace std;
using namespace cv;

int lowThreshold = 125;
Mat src = imread("Side Goal.png");

int main() {

	//Gets the video input
	VideoCapture input("Vision.mp4");

	//Loops forever
	for (;;) {

		//Reads the next image in the video and saves it to src
		input.read(src);

		//HSV filtering values
		int VSub = 29; //42
		int VAdd = 181; //150

		int SSub = 235;
		int SAdd = 255;

		int HSub = 170;
		int HAdd = 255;

		//Output mask
		Mat dst;

		//Sets HSV values
		Scalar hsvLow = Scalar(HSub, SSub, VSub);
		Scalar hsvHigh = Scalar(HAdd, SAdd, VAdd);

		//Filters by Scalar HSV
		cv::inRange(src, hsvLow, hsvHigh, dst);

		//makes two vector points for contours2
		vector<vector<Point>> contours1;
		vector<vector<Point>> contours2;

		//Makes a vector hierarchy
		vector<Vec4i> hierarchy;

		//Gets the touched up filtered image
		Mat goalFiltered = imread("FilteredGoal.png");

		//Converts the filtered goal to black and white
		cvtColor(goalFiltered, goalFiltered, CV_BGR2GRAY);

		//Finds the contours in both the goal and in the image
		cv::findContours(goalFiltered, contours1, 2, 1);
		cv::findContours(dst, contours2, 2, 1);

		//filters by closeness to touched up goal removes all contours that aren't within the threshold
		for (int i = 0; i < contours2.size(); i++) {
			double val = cv::matchShapes(contours2[i], contours1[0], 1, 0.0);

			if (val > 600)
				contours2.erase(contours2.begin() + i);
		}


		int largest = 0; //Makes a varibale for size
		int index = -1; //Index is the position of the wanted contour

		//Only keeps the largest contour
		for (int i = 0; i < contours2.size(); i++) {
			double size = cv::boundingRect(contours2[i]).width;

			if (size > largest) {
				index = i;
				largest = size;
			}
		}

		//Copys src to make a layerable image
		Mat out;
		src.copyTo(out);

		//draws wanted contour on out
		cv::drawContours(out, contours2, index, Scalar(255, 0, 255), 2, 8,
				hierarchy, 0, Point());

		//if it found a contour it draws a rectangle
		if (index > -1) {
			printf("size %d index %d", largest, index);

			int x = cv::boundingRect(contours2[index]).x;
			int y = cv::boundingRect(contours2[index]).y;

			int width = cv::boundingRect(contours2[index]).width;
			int height = cv::boundingRect(contours2[index]).height;

			cv::rectangle(out, Point(x, y), Point(x + width, y + height),
					Scalar(255, 128, 0), 2, 2, 0);
		}

		//shows on screen
		cv::imshow("Filtered Image", out);

		//waits 30 milliseconds before going to the next frame
		cv::waitKey(30);
	}

	cv::waitKey();

	return 0;
}
