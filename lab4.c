Kuba Grzybowski
#include <stdio.h>
using namespace std;

#include <iostream>

// OpenCV includes
#include <opencv2/opencv.hpp>
using namespace cv;
#include "Typedefs.h"

int main()
{
	// reading source file srcImage
	Mat srcImage;
	srcImage = imread( "s184670_D.jpg" );

	if ( !srcImage.data )
	{
		cout << "Error! Cannot read source file. Press ENTER.";
		waitKey();
		return( -1 );
	}

	//namedWindow("s184670");
	//zmienic potem pozycje
	//moveWindow("s184670", -1920, 0);
	//imshow("s184670", srcImage);

	const int scale = 3;
	Mat resizedImage;
	resize(srcImage, resizedImage, Size(srcImage.cols/scale, srcImage.rows/scale));
	namedWindow("Jakub Grzybowski");
	//zmienic potem pozycje
	moveWindow("Jakub Grzybowski", -1920, 0);
	imshow("Jakub Grzybowski", resizedImage);
	Mat grayImage;
	cvtColor(resizedImage, grayImage, COLOR_BGR2GRAY);
	Size patternsize(CheckerboardInnerWidth[0], CheckerboardInnerHeight[0]);
	vector<Point2f> corners;
	for (int i = 0; i < 4; i++) {
		Point2f tmp(CheckerboardInnerWidth[i], CheckerboardInnerHeight[i]);
		corners.push_back(tmp);
	}

	bool arg = findChessboardCorners(grayImage, patternsize, corners);
	if (!arg) {
		cout << "Panika! Press ENTER.\n";
		waitKey();
		return(-1);
	}

	cornerSubPix(grayImage, corners, patternsize, Size(CheckerboardInnerWidth[3], CheckerboardInnerHeight[3]), TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.1));
	drawChessboardCorners(grayImage, patternsize, corners, arg);

	int delta_x = corners[1].x - corners[0].x;
	int delta_y = corners[1].y - corners[0].y;
	float d = sqrt(pow(delta_x, 2) + pow(delta_y, 2));
	cout << d << endl;
	for (int i = 0; i < 39; i++) {
		putText(grayImage, to_string(i), corners[i], 1, 1, Scalar(255, 0, 0), 1, 8, false);
	}

	namedWindow("184670");
	moveWindow("184670", -1920, 0);
	imshow("184670", grayImage);
	imwrite("184670_TopChessboardFound.jpg", grayImage);

	float reverseScale = 35.0f;
	vector<Point2f> templateCorners;
	for (int i = 1; i <= CheckerboardInnerHeight[TOP]; i++) {
		for (int j = 1; j <= CheckerboardInnerWidth[TOP]; j++) {
			Point2f coord;
			coord.x = CheckerboardLTCoordinatesWithMargin[0].x + j * CHECKERBOARD_FIELD_SIZE;
			coord.y = CheckerboardLTCoordinatesWithMargin[0].y + i * CHECKERBOARD_FIELD_SIZE;
			templateCorners.push_back(coord * reverseScale);
		}
	}

	Mat homography;
	homography = findHomography(corners, templateCorners, RANSAC);
	Mat dewarpedImage(resizedImage.size(), resizedImage.type());
	warpPerspective(resizedImage, dewarpedImage, homography, resizedImage.size());

	namedWindow("Dewarped Image");
	moveWindow("Dewarped Image", -1920, 0);
	imshow("Dewarped Image", dewarpedImage);
	imwrite("s184670_Dewarped.jpg", dewarpedImage);

	Mat dewarpedROI = dewarpedImage(Rect(reverseScale * TemplateLT, reverseScale * TemplateRB));
	Mat debugImage = dewarpedROI.clone();
	vector<Point2f> dewarpedCorners;
	perspectiveTransform(corners, dewarpedCorners, homography);
	for (int i = 0; i < dewarpedCorners.size(); i++) {
		circle(debugImage, dewarpedCorners[i], 5, Scalar(255, 0, 0), 3);
	}

	namedWindow("Dewarped Image 2");
	moveWindow("Dewarped Image 2", -1920, 0);
	imshow("Dewarped Image 2", dewarpedImage);

	waitKey();
	return(0);
}


#include <stdio.h>
#include <cmath>
using namespace std;

#include <iostream>

// OpenCV includes
#include <opencv2/opencv.hpp>
using namespace cv;
#include "TypeDefs.h"
#define IMIE "Kamil Czepiel"
int scale = 4;

int main()
{
	// reading source file srcImage
	Mat srcImage;
	srcImage = imread( "Samples/184374_D.jpg" );
	if ( !srcImage.data )
	{
		cout << "Error! Cannot read source file. Press ENTER.";
		waitKey();
		return( -1 );
	}

	Mat grayImage(srcImage.size().height / scale, srcImage.size().width / scale, srcImage.type());
	resize(srcImage, grayImage, grayImage.size());
	

	cvtColor(grayImage, grayImage, COLOR_BGR2GRAY);

	Size patternSize(CheckerboardInnerWidth[0], CheckerboardInnerHeight[0]);
	vector<Point2f> corners;
	Mat output(srcImage.size().height / scale, srcImage.size().width / scale, srcImage.type());

	for (int i = 0; i < 4; i++) {
		Point2f tmp(CheckerboardInnerWidth[i], CheckerboardInnerHeight[i]);
		corners.push_back(tmp);
	}
	bool found = findChessboardCorners(grayImage, patternSize,corners);
	if (!found)
	{
		cout << "Nie znaleziono szachownicy";
		waitKey();
		return(-1);
	}
	cout << "Znaleziono";
	try {
		cornerSubPix(grayImage, corners, patternSize, Size(CheckerboardInnerWidth[3], CheckerboardInnerHeight[3]), TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.1));
	}
	catch (Exception e) {

		cout << "Blad: " << e.err;
	}
	drawChessboardCorners(grayImage, patternSize, corners, found);
	imwrite("NazwaPliku_TopChessboardFound.JPG", grayImage);

	int x = pow(corners[0].x - corners[1].x, 2);
	int y = pow(corners[0].y - corners[1].y, 2);
	float euklides = sqrt(x + y);
	cout << "Euklides: " << euklides;
	for (int i = 0; i < corners.size(); i++) {
		Point2f corner = corners[i];
		putText(grayImage, to_string(i), corner,FONT_HERSHEY_COMPLEX,.6,(255,255,255),1,2);
	}
	namedWindow(IMIE);
	moveWindow(IMIE, 0, 0);
	imshow(IMIE, grayImage);
	if (corners.front().y > corners.back().y)
		reverse(corners.begin(), corners.end());

	float reverseScake = 100.0f;
	vector<Point2f> templateCorners;
	for (int i = 0; i < corners.size(); i++) {
		Point2f coord;

	}

	waitKey();
	
	return 0;
}


#include <stdio.h>
#include <cmath>
using namespace std;

#include <iostream>

// OpenCV includes
#include <opencv2/opencv.hpp>
using namespace cv;
#include "TypeDefs.h"
#define IMIE "Kamil Czepiel"
int scale = 3;

int main()
{
	// reading source file srcImage
	Mat srcImage;
	srcImage = imread( "Samples/184374_D.jpg" );
	if ( !srcImage.data )
	{
		cout << "Error! Cannot read source file. Press ENTER.";
		waitKey();
		return( -1 );
	}

	Mat resizedImage;

	resize(srcImage, resizedImage, Size(srcImage.cols/scale,srcImage.rows/scale));
	
	Mat grayImage;

	cvtColor(resizedImage, grayImage, COLOR_BGR2GRAY);

	Size patternSize(CheckerboardInnerWidth[0], CheckerboardInnerHeight[0]);
	vector<Point2f> corners;
	Mat output(srcImage.size().height / scale, srcImage.size().width / scale, srcImage.type());

	bool found = findChessboardCorners(grayImage, patternSize,corners);
	if (!found)
	{
		cout << "Nie znaleziono szachownicy";
		waitKey();
		return(-1);
	}
	else {
		cout << "Znaleziono";
		try {
			cornerSubPix(grayImage, corners, patternSize, Size(CheckerboardInnerWidth[3], CheckerboardInnerHeight[3]), TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.1));
		}
		catch (Exception e) {

			cout << "Blad: " << e.err;
		}
		drawChessboardCorners(grayImage, patternSize, corners, found);

		int x = pow(corners[0].x - corners[1].x, 2);
		int y = pow(corners[0].y - corners[1].y, 2);
		float euklides = sqrt(x + y);
		cout << "Euklides: " << euklides;

		for (int i = 0; i < corners.size(); i++) {
			Point2f corner = corners[i];
			putText(grayImage, to_string(i), corner,FONT_HERSHEY_COMPLEX,.6,(255,255,255),1,2);
		}
		namedWindow(IMIE);
		moveWindow(IMIE, 0, 0);
		imshow(IMIE, grayImage);
		imwrite("184374_D_TopChessboardFound.JPG", grayImage);
		if (corners.front().y > corners.back().y)
			reverse(corners.begin(), corners.end());

		float reverseScale = 40.0f;
		vector<Point2f> templateCorners;
		for (int i = 1; i <= CheckerboardInnerHeight[TOP]; i++) {
			for (int j = 1; j <= CheckerboardInnerWidth[TOP]; j++) {
				Point2f coord;
				coord.x = CheckerboardLTCoordinatesWithMargin[0].x + j * CHECKERBOARD_FIELD_SIZE;
				coord.y = CheckerboardLTCoordinatesWithMargin[0].y + i * CHECKERBOARD_FIELD_SIZE;
				templateCorners.push_back(coord * reverseScale);
			}
		}

		Mat homography;
		homography = findHomography(corners, templateCorners, RANSAC);

		Mat dewarpedImage(srcImage.size(), srcImage.type());
		warpPerspective(resizedImage, dewarpedImage, homography, resizedImage.size());
		namedWindow("Dewarped Image");
		moveWindow("Dewarped Image", 0, 0);
		imshow("Dewarped Image", dewarpedImage);
		imwrite("184374_D_Dewarped.JPG",dewarpedImage);
	}


	waitKey();
	
	return 0;
}

