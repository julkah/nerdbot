/*
 * Dieses Programm wurde geschrieben von Nerdbot
 * Version: v1
 * Datum: 22.09.2016
 * Beschreibung: Automatische tracken einer Farbe.
 * 				 Optimiert für einen 2-4 Kern Prozessor
 *
*/
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <bitset>
#include <stdio.h>
#include <thread>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <stddef.h>
#include <cstdlib>
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <time.h>
#include <fstream>

using namespace cv;
using namespace std;

static bool frames = true;
bool fast = true;
int schlafen = 400;

Mat imgO;
Mat imgOriginal;
Mat imgThresholded;
Mat imgThresholdedr;
Mat imgThresholdedt;
Mat imgLines;
Mat imgHSV;
Mat imgHSVt;
Mat imgHs;
Mat imgSetup;
Mat imgSetupt;
Mat imgSetupb;
Mat imgStitch;
Mat imgStitcht;
Mat imgTor;
Mat imgBall;
Mat saveFrame;
Mat saveO;
Mat dilelement;
Mat processimganzeige;
Mat processimganzeiget;
VideoCapture cap(0);
VideoWriter outputVideo; 
double dM01;
double dM10;
double dArea;
double dM01t;
double dM10t;
double dAreat;
int lowh = 0; // 5
int highh = 0; // 27
int lows = 0; // 150
int highs = 0; // 240
int lowv = 0; // 200
int highv = 0; // 255
int lowhr = 0; // 5
int highhr = 0; // 27
int lowsr = 0; // 150
int highsr = 0; // 240
int lowvr = 0; // 200
int highvr = 0; // 255

int lowht = 0; // 5
int highht = 0; // 27
int lowst = 0; // 150
int highst = 0; // 240
int lowvt = 0; // 200
int highvt = 0; // 255

int lowhtg = 0; // 5
int highhtg = 0; // 27
int lowstg = 0; // 150
int highstg = 0; // 240
int lowvtg = 0; // 200
int highvtg = 0; // 255

volatile int iLastXt = 0;
volatile int iLastYt = 0;
volatile int iLastX = 0;
volatile int iLastY = 0;
volatile int torangle = 0;
volatile int rwidth = 0;
volatile int dist = 0;
volatile int distt = 0;
int camhue = 50;
int cambrightness = 50;
int fps;
int comArea = 0;
volatile int camcontrast = 0;
volatile bool critical = true;
volatile bool crit = true;
volatile bool critt = true;
volatile bool crity = false;
volatile bool crityt = false;
volatile bool gotit = false;
volatile bool gotitt = false;
volatile bool onsight = false;
volatile bool onsightt = false;
volatile bool gelb = false;
volatile bool blau = true;
volatile bool fertigball = false;
volatile bool fertigtor = false;
volatile bool front = true;
volatile bool windows = true;
volatile bool dest = false;
volatile bool desttime = false;
volatile bool aufnahme = false;
volatile bool aufnahmefirst = false;
volatile bool aufnahmekill = false;
volatile bool gotitw = false;
volatile bool torview = false;
Point spot = Point(160, 70);

bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
    double i = fabs( contourArea(cv::Mat(contour1)) );
    double j = fabs( contourArea(cv::Mat(contour2)) );
    return ( i < j );
}

void processimg()
{
    try
    {
        for (;;)
        {
            while ((critical == true) || (gotit == false))
            {
                usleep(schlafen);
            }
            crity = true;
            inRange(imgHSV, Scalar(lowh, lows, lowv), Scalar(highh, highs, highv), imgThresholded);
            inRange(imgHSV, Scalar(lowhr, lowsr, lowvr), Scalar(highhr, highsr, highvr), imgThresholdedr);
            imgThresholded = imgThresholded + imgThresholdedr;
            crity = false;
            dilate(imgThresholded, imgThresholded, dilelement);
			medianBlur(imgThresholded, imgThresholded, 5);
            if (fast == false)
            {
                imgThresholded.copyTo(processimganzeige);
            }
			RNG rng(12345);
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;
			findContours(imgThresholded, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
			//imgThresholded = Mat::zeros( imgThresholded.size(), CV_8UC3 );
			vector<vector<Point> > contours_poly(contours.size());
			vector<RotatedRect> boundRect(contours.size());
			vector<Point2f> center(contours.size());
			vector<float> radius(contours.size());
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			int num = contours.size()-1;
			if(contours.size() > 0)
			{
				if(contours.size() > 1)
				{
					sort(contours.begin(), contours.end(), compareContourAreas);
				}
				approxPolyDP( Mat(contours[num]), contours_poly[num], 3, true );
				minEnclosingCircle( (Mat)contours_poly[num], center[num], radius[num] );
				boundRect[num] = minAreaRect(Mat(contours_poly[num]));
				circle( imgThresholded, center[num], (int)radius[num], color, 2, 8, 0 );
				crit = true;
				dist = (int)radius[num];
				//cout << "Distance: " << dist << endl;
				iLastX = boundRect[num].center.x;
				iLastY = boundRect[num].center.y;
				onsight = true;
			}
			else
			{
				onsight = false;
			}
			if (fast == false)
            {
				processimganzeige = imgThresholded + processimganzeige;
                fertigball = false;
                processimganzeige.copyTo(imgBall);
                fertigball = true;
            }
			/*fps++;
			cout << "Frame: " << fps << endl;*/
            crit = false;
            gotit = false;
        }
    }
    catch (const exception& e)
    {
    }
}

void uarttry()
{
    for (;;)
    {
        try
        {
            ofstream myfile;
            myfile.open("/dev/ttyS2");
            ifstream myfilei;
            myfilei.open("/dev/ttyS2");
            for (;;)
            {
                string line;
                while (getline(myfilei, line))
                {
                    uint16_t message;
                    switch (stoi(line))
                    {

                        case 1:
                            if (front)
                            {
                                message = 0;
                            }
                            else
                            {
                                message = 2;
                            }
                            message <<= 8;
                            if (onsight)
                            {
                                message += ((iLastX * 796875) / 1000000);
                            }
							else
							{
								
							}                            
                            message <<= 4;
							if(dist > 50)
							{
								dist = 50;
							}
                            message += ((dist * 32) / 100);
                            message <<= 1;
                            message += 1;
                            myfile << message << endl;
                            break;
                        case 2:
                            if (front)
                            {
                                message = 1;
                            }
                            else
                            {
                                message = 3;
                            }
                            message <<= 7;
							if(torview)
							{
                            message += ((iLastXt * 4) / 10);
							}
                            message <<= 4;
							if(distt > 80)
							{
								distt = 80;
							}
                            message += ((distt * 2) / 10);
                            message <<= 2;
                            if (onsightt)
                            {
                                message += 2;
                            }
                            message += 1;
                            myfile << message << endl;
							//cout << message << endl;
                            break;
                        case 3:
							cout << "GEEEEEEEEEEEEEEEELLLLLLLLLLLLBBBBBBBB" << endl;
                            gelb = true;
                            blau = false;
                            break;
                        case 4:
							cout << "BLLLLLLLLLLLLLAAAAAAAAAAAAAAAAAAUUUUU" << endl;
                            blau = true;
                            gelb = false;
                            break;
                        case 5:
                            windows = true;
                            fast = false;
                            break;
                        case 6:
                            dest = true;
                            fast = true;
                            break;
                        case 7:
                            while (getline(myfilei, line))
                            {
                                lowh = stoi(line);
                                break;
                            }
                            break;
                        case 8:
                            while (getline(myfilei, line))
                            {
                                lows = stoi(line);
                                break;
                            }
                            break;
                        case 9:
                            while (getline(myfilei, line))
                            {
                                lowv = stoi(line);
                                break;
                            }
                            break;
                        case 10:
                            while (getline(myfilei, line))
                            {
                                highh = stoi(line);
                                break;
                            }
                            break;
                        case 11:
                            while (getline(myfilei, line))
                            {
                                highs = stoi(line);
                                break;
                            }
                            break;
                        case 12:
                            while (getline(myfilei, line))
                            {
                                highv = stoi(line);
                                break;
                            }
                            break;
                        case 13:
                            while (getline(myfilei, line))
                            {
                                lowhr = stoi(line);
                                break;
                            }
                            break;
                        case 14:
                            while (getline(myfilei, line))
                            {
                                lowsr = stoi(line);
                                break;
                            }
                            break;
                        case 15:
                            while (getline(myfilei, line))
                            {
                                lowvr = stoi(line);
                                break;
                            }
                            break;
                        case 16:
                            while (getline(myfilei, line))
                            {
                                highhr = stoi(line);
                                break;
                            }
                            break;
                        case 17:
                            while (getline(myfilei, line))
                            {
                                highsr = stoi(line);
                                break;
                            }
                            break;
                        case 18:
                            while (getline(myfilei, line))
                            {
                                highvr = stoi(line);
                                break;
                            }
                            break;
                        case 19:
                            while (getline(myfilei, line))
                            {
                                cambrightness = stoi(line);
                                cap.set(CV_CAP_PROP_BRIGHTNESS, ((double)cambrightness / 100));
                                break;
                            }
                            break;
                        case 20:
                            while (getline(myfilei, line))
                            {
                                camhue = stoi(line);
                                cap.set(CV_CAP_PROP_HUE, ((double)camhue / 100));
                                break;
                            }
                            break;
                        case 21:
                            while (getline(myfilei, line))
                            {
                                lowht = stoi(line);
                                break;
                            }
                            break;
                        case 22:
                            while (getline(myfilei, line))
                            {
                                lowst = stoi(line);
                                break;
                            }
                            break;
                        case 23:
                            while (getline(myfilei, line))
                            {
                                lowvt = stoi(line);
                                break;
                            }
                            break;
                        case 24:
                            while (getline(myfilei, line))
                            {
                                highht = stoi(line);
                                break;
                            }
                            break;
                        case 25:
                            while (getline(myfilei, line))
                            {
                                highst = stoi(line);
                                break;
                            }
                            break;
                        case 26:
                            while (getline(myfilei, line))
                            {
                                highvt = stoi(line);
                                break;
                            }
                            break;
                        case 27:
                            while (getline(myfilei, line))
                            {
                                lowhtg = stoi(line);
                                break;
                            }
                            break;
                        case 28:
                            while (getline(myfilei, line))
                            {
                                lowstg = stoi(line);
                                break;
                            }
                            break;
                        case 29:
                            while (getline(myfilei, line))
                            {
                                lowvtg = stoi(line);
                                break;
                            }
                            break;
                        case 30:
                            while (getline(myfilei, line))
                            {
                                highhtg = stoi(line);
                                break;
                            }
                            break;
                        case 31:
                            while (getline(myfilei, line))
                            {
                                highstg = stoi(line);
                                break;
                            }
                            break;
                        case 32:
                            while (getline(myfilei, line))
                            {
                                highvtg = stoi(line);
                                break;
                            }
                            break;
                        case 33:
                            front = true;
                            break;
                        case 34:
                            front = false;
                            break;
						case 35:
							desttime = true;
							break;
						case 36:
							desttime = false;
							break;
						case 37:
							dest = true;
							break;
						case 38:
							dest = false;
							break;
						case 39:
							windows = true;
							break;
						case 40:
							aufnahme = true;
							aufnahmefirst = true;
							break;
						case 41:
							aufnahme = false;
							aufnahmekill = true;
							break;
						case 42:
							while (getline(myfilei, line))
                            {
                                camcontrast = stoi(line);
								break;
                            }
                        default:
                            cout << "Uart Fehler!" << endl;
                    }
                }
            }
			usleep(10);
        }
        catch (exception e)
        {
        }
    }
}


void torprocessimg()
{
    try
    {
        for (;;)
        {
            while ((critical == true) || (gotitt == false))
            {
                usleep(schlafen);
            }
            crityt = true;
            if (blau)
            {
                inRange(imgHSVt, Scalar(lowht, lowst, lowvt), Scalar(highht, highst, highvt),
                    imgThresholdedt);
            }
            else
            {
                inRange(imgHSVt, Scalar(lowhtg, lowstg, lowvtg), Scalar(highhtg, highstg, highvtg),
                    imgThresholdedt);
            }
            crityt = false;
            dilate(imgThresholdedt, imgThresholdedt, dilelement);
			medianBlur(imgThresholdedt, imgThresholdedt, 5);
			if (fast == false)
            {
                imgThresholdedt.copyTo(processimganzeiget);
            }
			RNG rngt(12345);
			vector<vector<Point> > contourst;
			vector<Vec4i> hierarchyt;
			findContours(imgThresholdedt, contourst, hierarchyt, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
			//imgThresholdedt = Mat::zeros( imgThresholdedt.size(), CV_8UC3 );
			vector<vector<Point> > contours_polyt(contourst.size());
			vector<Point2f> centert(contourst.size());
			vector<RotatedRect> boundRectt(contourst.size());
			vector<float> radiust(contourst.size());
			Scalar colort = Scalar(rngt.uniform(0, 255), rngt.uniform(0, 255), rngt.uniform(0, 255));
			int num = contourst.size()-1;
			if(contourst.size() > 0)
			{	
				if(contourst.size() > 1)
				{
					sort(contourst.begin(), contourst.end(), compareContourAreas);
				}
				approxPolyDP( Mat(contourst[num]), contours_polyt[num], 3, true );
				boundRectt[num] = minAreaRect(Mat(contours_polyt[num]));	
				torangle = boundRectt[num].angle;
				distt = boundRectt[num].size.height;
				rwidth = boundRectt[num].size.width;
				critt = true;
				if (boundRectt[num].size.width <= boundRectt[num].size.height) 
				{
					torangle = 90 + torangle;
					distt = boundRectt[num].size.width;
					rwidth = boundRectt[num].size.height;
				}
				//cout << "Angle: " << torangle << endl;
				Point2f rect_points[4]; boundRectt[num].points( rect_points );
				for( int j = 0; j < 4; j++ )
				{
					line( imgThresholdedt, rect_points[j], rect_points[(j+1)%4], colort, 1, 8 );
				}
				//cout << "Goal Distance: " << distt << endl;
				iLastXt = boundRectt[num].center.x;
				iLastYt = boundRectt[num].center.y;
				if(iLastXt >= 160)
				{
					if((iLastXt - (rwidth/2) - 160) < -5)
					{
						onsightt = true;
					}
					else
					{
						onsightt = false;
					}
				}
				else
				{
					if((iLastXt + (rwidth/2) - 160) > 5)
					{
						onsightt = true;
					}
					else
					{
						onsightt = false;
					}
				}
				torview = true;
			}
			else
			{
				torview = false;
				onsightt = false;
			}
            critt = false;
			//cout << "Schusssicher: " << onsightt << " Target: " << iLastXt << endl;
            gotitt = false;
			if (fast == false)
            {
				processimganzeiget = imgThresholdedt + processimganzeiget;
                fertigtor = false;
                processimganzeiget.copyTo(imgTor);
                fertigtor = true;
            }
        }
    }
    catch (const exception& e)
    {
    }
}

inline bool exists_test (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

void setup()
{
    string setup = "stty -F /dev/ttyS2 115200";
    system(setup.c_str()); // Setup Serial connenction
    cap.set(CV_CAP_PROP_HUE, ((double)camhue / 100));
    cap.set(CV_CAP_PROP_BRIGHTNESS, ((double)cambrightness / 100));
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    cap.set(CV_CAP_PROP_FPS, 60);
	cap.set(CV_CAP_PROP_CONTRAST, 0.5);
    cap >> imgO;
    cap >> imgSetup;
    cap >> imgStitch;
    cap >> imgStitcht;
	int dilation_size = 4;
	dilelement = getStructuringElement( MORPH_RECT, Size( 2*dilation_size + 1, 2*dilation_size+1 ), Point( dilation_size, dilation_size ) );
    imgStitch = Scalar(255, 255, 255);
    circle(imgStitch, Point(120, 125), 80, Scalar(0, 0, 0), -1, 8, 0);
    circle(imgStitch, Point(160, 115), 92, Scalar(0, 0, 0), -1, 8, 0);
    circle(imgStitch, Point(200, 117), 80, Scalar(0, 0, 0), -1, 8, 0);
    circle(imgStitch, Point(-20, 255), 150, Scalar(255, 255, 255), -1, 8, 0);
    circle(imgStitch, Point(350, 255), 150, Scalar(255, 255, 255), -1, 8, 0);

    imgStitcht = Scalar(255, 255, 255);
    circle(imgStitcht, Point(120, 97), 60, Scalar(0, 0, 0), -1, 8, 0);
    circle(imgStitcht, Point(160, 106), 80, Scalar(0, 0, 0), -1, 8, 0);
    circle(imgStitcht, Point(200, 97), 60, Scalar(0, 0, 0), -1, 8, 0);
    circle(imgStitcht, Point(160, 305), 200, Scalar(255, 255, 255), -1, 8, 0);
}

void fpsc()
{
    for (;;)
    {
        usleep(1000000);
        cout << "FPS: " << fps << endl;
		fps = 0;
	}
}

void aufnahmen()
{
	for(;;)
	{
		while(critical || !gotitw)
		{
			usleep(schlafen);
		}
		if(aufnahme){outputVideo << saveFrame; gotitw = true;}else{usleep(10000);}
	}
}

int main(int argc, char** argv)
{
    if (cap.isOpened() == false)
    {
        cout << "VideoDeviceError! BIATCHING ;)" << endl;
        return -1;
    }
    setup();
    thread first(uarttry);
    first.detach();
    thread seccond(processimg);
    seccond.detach();
    thread third(torprocessimg);
    third.detach();
	if(frames){thread fpscounter(fpsc); fpscounter.detach();}
	thread videos(aufnahmen);
	videos.detach();
    for (;;)
    {
		if(aufnahmefirst)
		{
			try
			{
				Size S = Size(320,240);
				int filen = 0;
				while(exists_test("/home/odroid/aufnahmen/video(" + to_string(filen) + ").mpeg"))
				{
					filen++;
				}
				outputVideo.open(("/home/odroid/aufnahmen/video(" + to_string(filen) + ").mpeg") , CV_FOURCC('P','I','M','1'), 30, S, true);
				if (!outputVideo.isOpened())
				{
					cout  << "Could not open the output video for write: " << endl;
				}
				aufnahmefirst = false;
			}catch(exception e)
			{
			}
		}
		if(aufnahmekill)
		{
			try
			{
				outputVideo.release();
				aufnahmekill = false;
			}
			catch(exception e)
			{
			}
		}
        cap >> imgO;
		if(frames){fps++;}
		if(aufnahme){imgO.copyTo(saveFrame);}
        if (front)
        {
            imgO = imgO - imgStitch;
        }
        else
        {
            imgO = imgO - imgStitcht;
        }
        cvtColor(imgO, imgHs, COLOR_BGR2HSV);
        critical = true;
        while (crity == true || crityt == true)
        {
            usleep(schlafen);
        }
        gotit = true;
        gotitt = true;
		gotitw = true;
        imgOriginal = imgO;
        imgHSV = imgHs;
        imgHSVt = imgHs;
        critical = false;
        if (fast == false)
        {
            try
            {
                cap >> imgSetupb;
                if (front)
                {
                    imgSetupb = imgSetupb - imgStitch;
                }
                else
                {
                    imgSetupb = imgSetupb - imgStitcht;
                }
                cvtColor(imgSetupb, imgSetupt, COLOR_BGR2HSV);
                putText(imgSetupb,
                    format("(%d,%d,%d)", imgSetupt.at<Vec3b>(spot).val[0], imgSetupt.at<Vec3b>(spot).val[1], imgSetupt.at<Vec3b>(spot).val[2]), Point(10, 50), CV_FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 0, 255));
                imgSetupb.at<Vec3b>(spot).val[0] = 200;
                imgSetupb.at<Vec3b>(spot).val[1] = 0;
                imgSetupb.at<Vec3b>(spot).val[2] = 0;
                if (windows)
                {
                    namedWindow("5,0 Original", WINDOW_AUTOSIZE);
                    namedWindow("Was guckst du so?", WINDOW_AUTOSIZE);
                    namedWindow("Teest", WINDOW_AUTOSIZE);
                    moveWindow("5,0 Original", 0, 0);
                    moveWindow("Was guckst du so?", 320, 0);
                    moveWindow("Teest", 0, 240);
                    windows = false;
                }
                circle(imgSetupb, spot, 7, Scalar(165, 0, 0), 2);
                imshow("5,0 Original", imgSetupb);
                while (fertigball == false)
                {
                    usleep(schlafen);
                }
                circle(imgBall, spot, 7, Scalar(165, 0, 0), 2);
                imshow("Was guckst du so?", imgBall);
                while (fertigtor == false)
                {
                    usleep(schlafen);
                }
                circle(imgTor, spot, 7, Scalar(165, 0, 0), 2);
                imshow("Teest", imgTor);
                waitKey(30);
            }
            catch (exception e)
            {
            }
        }
		if(dest)
		{
			destroyAllWindows();
			dest = false;
		}
		if(desttime){waitKey(30);}
		
                                cap.set(CV_CAP_PROP_CONTRAST, 0.6);
		//usleep(100);
    }
    return 0;
}
