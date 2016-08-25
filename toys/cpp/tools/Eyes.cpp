#include <vector>
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


void colorize()
{
    char key;
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return;
    Mat edges;
    namedWindow("edges",1);
    for(;;) {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, CV_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        imshow("edges", edges);
        key = cvWaitKey(10);     //Capture Keyboard stroke
        if (char(key) == 27) {
            break;      //If you hit ESC key loop will break.
        }
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
}


void wire_frame()
{
    char key;
    Mat img;
    VideoCapture cap(0);
    while (true) {
        cap >> img;
        Mat edges;
        cvtColor(img, edges, CV_BGR2GRAY);
        Canny(edges, edges, 30, 60);
        imshow("window label", edges);
        key = cvWaitKey(10);     //Capture Keyboard stroke
        if (char(key) == 27) {
            break;      //If you hit ESC key loop will break.
        }
    }
}


// Slider for the low threshold value of our edge detection
int maxLowThreshold = 1024;
int lowSliderPosition = 150;
// Slider for the high threshold value of our edge detection
int maxHighThreshold = 1024;
int highSliderPosition = 250;

IplImage* tempFrame;

// Function to find the edges of a given IplImage object
IplImage* findEdges(IplImage* sourceFrame, double thelowThreshold, double theHighThreshold, double theAperture)
{
    // Convert source frame to greyscale version (tempFrame has already been initialised to use greyscale colour settings)
    cvCvtColor(sourceFrame, tempFrame, CV_RGB2GRAY);
    // Perform canny edge finding on tempframe, and push the result back into itself!
    cvCanny(tempFrame, tempFrame, thelowThreshold, theHighThreshold, theAperture);
    // Pass back our now processed frame!
    return tempFrame;
}

void camera()
{
    char key;
    cvNamedWindow("Camera_Output", 1);    //Create window
    CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);  //Capture using any camera connected to your system
    IplImage* pProcessedFrame;
    IplImage* pRawFrame; //Create image frames from capture
    IplImage* imgThreshed;

    pRawFrame = cvQueryFrame(capture); //Create image frames from capture
	tempFrame = cvCreateImage(cvSize(pRawFrame->width, pRawFrame->height), IPL_DEPTH_8U, 1);    

    IplImage* imgHSV = cvCreateImage(cvGetSize(pRawFrame), IPL_DEPTH_8U, 3); 
    cvCvtColor(pRawFrame, imgHSV, CV_BGR2HSV);

    while (1) { //Create infinte loop for live streaming
        pRawFrame = cvQueryFrame(capture); //Create image frames from capture
        Mat img(pRawFrame); // cv::Mat replaces the CvMat and IplImage, but it's easy to convert
        Mat img_yuv;
        cvtColor(img, img_yuv, CV_BGR2YCrCb); // convert image to YUV color space. The output image will be created automatically

        imgThreshed = cvCreateImage(cvGetSize(pRawFrame), IPL_DEPTH_8U, 1);  //- See more at: http://aishack.in/tutorials/tracking-colored-objects-opencv/#sthash.iurwzPDm.dpuf

        cvInRangeS(imgHSV, cvScalar(20, 100, 100), cvScalar(30, 255, 255), imgThreshed);

        cvShowImage("Camera_Output", pRawFrame);   //Show image frames on created window
    	//pProcessedFrame = cvCreateImage(cvSize(pRawFrame->width, pRawFrame->height), IPL_DEPTH_8U, 1);
        pProcessedFrame = findEdges(pRawFrame, lowSliderPosition, highSliderPosition, 3);
 		// Showed the processed output in our other window
        cvShowImage("Processed WebCam", pProcessedFrame);
        cvShowImage("Thresholded", imgThreshed);
         
        key = cvWaitKey(10);     //Capture Keyboard stroke
        if (char(key) == 27) {
            break;      //If you hit ESC key loop will break.
        }
    }
    cvReleaseCapture(&capture); //Release capture.
    cvDestroyWindow("Camera_Output"); //Destroy Window
}

void color_filter()
{
    char key;
    cvNamedWindow("Camera_Output", 1);    //Create window
    IplImage* pRawFrame; //Create image frames from capture
    IplImage* upper_red_hue_range;
    IplImage* lower_red_hue_range;
    IplImage* imgHSV; 
    CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);  //Capture using any camera connected to your system
    unsigned long long frame_no = 0;

    //pRawFrame = cvQueryFrame(capture); //Create image frames from capture

    while (1) { //Create infinte loop for live streaming
        pRawFrame = cvQueryFrame(capture); //Create image frames from capture
        Mat raw_img_mat(pRawFrame);
        imgHSV = cvCreateImage(cvGetSize(pRawFrame), IPL_DEPTH_8U, 3); 
        cvCvtColor(pRawFrame, imgHSV, CV_BGR2HSV);

        upper_red_hue_range = cvCreateImage(cvGetSize(pRawFrame), IPL_DEPTH_8U, 1);  //- See more at: http://aishack.in/tutorials/tracking-colored-objects-opencv/#sthash.iurwzPDm.dpuf
        lower_red_hue_range = cvCreateImage(cvGetSize(pRawFrame), IPL_DEPTH_8U, 1);  //- See more at: http://aishack.in/tutorials/tracking-colored-objects-opencv/#sthash.iurwzPDm.dpuf

        cvInRangeS(imgHSV, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
        cvInRangeS(imgHSV, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);

        Mat red_hue_image;
        Mat red_hue_up(upper_red_hue_range);
        Mat red_hue_lo(lower_red_hue_range);
        addWeighted(red_hue_lo, 1.0, red_hue_up, 1.0, 0.0, red_hue_image);
        GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);

        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(red_hue_image, circles, CV_HOUGH_GRADIENT, 1, red_hue_image.rows/8, 100, 20, 0, 0);

        // Loop over all detected circles and outline them on the original image
        if(circles.size() != 0) {
            for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle) {
                cv::Point center(std::round(circles[current_circle][0]), std::round(circles[current_circle][1]));
                int radius = std::round(circles[current_circle][2]);
                cv::circle(raw_img_mat, center, radius, cv::Scalar(0, 255, 0), 5);
            }
        
            if (!circles.empty()) {
                int roi_x = circles[0][0];
                int roi_y = circles[0][1];
                if ((frame_no % 100) == 0) {
                    cout << "x: " << roi_x << ", y: " << roi_y << endl;
                }
    		}
        }

        cvShowImage("Camera_Output", pRawFrame);   //Show image frames on created window
 		// Showed the processed output in our other window
        cvShowImage("Lower", lower_red_hue_range);
        cvShowImage("Upper", upper_red_hue_range);
         
        cvReleaseImage(&imgHSV);
        cvReleaseImage(&upper_red_hue_range);
        cvReleaseImage(&lower_red_hue_range);

        key = cvWaitKey(10);     //Capture Keyboard stroke
        if (char(key) == 27) {
            break;      //If you hit ESC key loop will break.
        }
    }
    cvReleaseCapture(&capture); //Release capture.
    cvDestroyWindow("Camera_Output"); //Destroy Window
}

int jpg_image(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << " Usage: display_image ImageToLoadAndDisplay" << std::endl;
        return -1;
    }
    cv::Mat image;
    image = cv::imread(argv[1], cv::IMREAD_COLOR); // Read the file
    if (!image.data) { // Check for invalid input
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }
    cv::namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
    cv::imshow("Display window", image); // Show our image inside it.
    cv::waitKey(0); // Wait for a keystroke in the window
    return 0;
}

int main(int argc, char** argv)
{
    //jpg_image(argc, argv);
    //camera();
    //wire_frame();
    //colorize();
    color_filter();
    return 0;
}
