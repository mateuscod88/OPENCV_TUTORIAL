/**
* @file Morphology_2.cpp
* @brief Advanced morphology Transformations sample code
* @author OpenCV team
*/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;
/// Global variables
Mat src, dst;

int morph_elem = 0;
int morph_size = 0;
int morph_operator = 0;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;

const char* window_name = "Morphology Transformations Demo";


/** Function Headers */
void Morphology_Operations(int, void*);

/**
* @function main
*/
int main(int, char** argv)
{
	/// Load an image
	//src = imread(argv[1]);

	//if (src.empty())
	//{
	//	return -1;
	//}

	///// Create window
	//namedWindow(window_name, WINDOW_AUTOSIZE);

	///// Create Trackbar to select Morphology operation
	//createTrackbar("Operator:\n 0: Opening - 1: Closing  \n 2: Gradient - 3: Top Hat \n 4: Black Hat", window_name, &morph_operator, max_operator, Morphology_Operations);

	///// Create Trackbar to select kernel type
	//createTrackbar("Element:\n 0: Rect - 1: Cross - 2: Ellipse", window_name,
	//	&morph_elem, max_elem,
	//	Morphology_Operations);

	///// Create Trackbar to choose kernel size
	//createTrackbar("Kernel size:\n 2n +1", window_name,
	//	&morph_size, max_kernel_size,
	//	Morphology_Operations);

	///// Default start
	//Morphology_Operations(0, 0);

	//const string imgName = argv[1];
	//Mat img, hsv; // do przetrzymywania obiektow 
	//vector<Mat> img_split;
	//img = imread(imgName);
	//if (!img.data)
	//{
	//	cout << "Nie znaleziono pliku" << endl;
	//	return -1;

	//}

	//const string named_window[] = { "BGR","HSV","HUE","SAT","VAL" };
	//namedWindow(named_window[0], CV_WINDOW_AUTOSIZE);// utworzenie okna
	//namedWindow(named_window[1], CV_WINDOW_AUTOSIZE);
	//namedWindow(named_window[2], CV_WINDOW_AUTOSIZE);
	//namedWindow(named_window[3], CV_WINDOW_AUTOSIZE);
	//namedWindow(named_window[4], CV_WINDOW_AUTOSIZE);

	//cvtColor(img, hsv, CV_BGR2HSV); // konwersja z bgr do hsv
	//split(hsv, img_split);
	//
	//imshow(named_window[0], img);
	//imshow(named_window[1], hsv);
	//imshow(named_window[2], img_split[0]);
	//imshow(named_window[3], img_split[1]);
	//imshow(named_window[4], img_split[2]);

	//vector<int>compresion_Params;
	//compresion_Params.push_back(CV_IMWRITE_JPEG_QUALITY);
	//compresion_Params.push_back(100);

	//imwrite("hsv.jpg", hsv, compresion_Params);
	//imwrite("hue.jpg", img_split[0], compresion_Params);
	//imwrite("sat.jpg", img_split[1], compresion_Params);
	/*imwrite("val.jpg", img_split[2], compresion_Params);*/
	
	VideoCapture capture =  VideoCapture(0);
	string windowName[] = { "video","binary","drawing" };
	Mat frame, img, hsv_img, binary;
	Mat cont;
	vector <Mat> hsv_split;
	for (int i = 0; i < 2; i++)
		namedWindow(windowName[i], CV_WINDOW_AUTOSIZE);
	int lowerband = 100; 
	int uperband = 109;
	createTrackbar("lb", windowName[0], &lowerband, 255, NULL);
	createTrackbar("ub", windowName[0], &uperband, 255, NULL);

	while (waitKey(20) != 27)
	{
		capture >> frame; //z video zlapanie jednej klatki
		frame.copyTo(img);
		cvtColor(img, hsv_img, CV_BGR2HSV);// zmiana formatu z BGR na HSV
		split(hsv_img, hsv_split); //rozdzielenie obrazu hsv na warstwy H hue barwa S saturatiin nasycenie V value wartosc
		inRange(hsv_split[0], lowerband, uperband, binary); // progowanie  czyli jak cos ma wartosc mieszczaca sie w zakresie to zmienia na biale a inaczej na czarne 
		Mat element(3, 3, CV_8U, Scalar(1));// do erodowania parametry 
		blur(binary, binary, Size(3, 3));// rozmycie
		erode(binary, binary, element);// erozja

		vector<vector<Point>> contours; // kontury obiektow
		vector<Point> contours_poly;  //  do obrysu prostokatnego
		Rect boundRect;// do przechowywania danych o obrysie
		binary.copyTo(cont);
		findContours(cont, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));// zwraca kontury obiektu  3 parametr zewnetrzne kontury

		int max = 0, i_cont = -1;
		Mat drawing = Mat::zeros(cont.size(), CV_8UC3);// pusty obrazek do konturow
		for (int i = 0; i < contours.size(); i++)// znajdujemy najwiekszy obszar
		{
			if (abs(contourArea(Mat(contours[i]))) > max) 
			{
				max = abs(contourArea(Mat(contours[i])));
				i_cont = i;
			}
		}
		if (i_cont >= 0)
		{
			approxPolyDP( Mat(contours[i_cont]), contours_poly, 3, true); //przyblizenie kontur
			boundRect = boundingRect(Mat(contours_poly));// dane o wielokacie
			fillConvexPoly(img, contours_poly, contours_poly.size());
			rectangle(img, boundRect.tl(), boundRect.br(), Scalar(250, 125, 125), 2, 8, 0);
			line(img, boundRect.tl(), boundRect.br(), Scalar(250, 125, 125), 2, 8, 0);
			line(img, Point(boundRect.x + boundRect.width, boundRect.y), Point(boundRect.x, boundRect.y + boundRect.height), Scalar(250, 125, 125), 2, 8, 0);
			string s;
			stringstream out;
			out << boundRect.x + boundRect.width / 2 << "x" << boundRect.y + boundRect.height / 2;
			s = out.str();
			putText(img, s, Point(50, 50), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(20, 40, 80), 3, 8);
			drawContours(drawing, contours, i_cont, Scalar(125, 125, 250), 2);

		}


		imshow(windowName[2], drawing);

		imshow(windowName[0], img);
		imshow(windowName[1], binary);
	}
	capture.release();
	//waitKey(0);
	return 0;
}

/**
* @function Morphology_Operations
*/
void Morphology_Operations(int, void*)
{

	// Since MORPH_X : 2,3,4,5 and 6
	int operation = morph_operator + 2;

	Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));

	/// Apply the specified morphology operation
	morphologyEx(src, dst, operation, element);
	imshow(window_name, dst);
}
