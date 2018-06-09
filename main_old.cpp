#include <iostream>
#include <opencv2/opencv.hpp>
#include "FaceDetector.h"
#include "CatDetector.h"
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/logger.h>


#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

using namespace cv;
using namespace std;

FaceDetector faceDetector;
CatDetector catDetector;

int main_cascade() {

    libfreenect2::Freenect2 freenect2;
    libfreenect2::Freenect2Device *dev;
    libfreenect2::PacketPipeline *pipeline;
    libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color |
                                                  libfreenect2::Frame::Depth |
                                                  libfreenect2::Frame::Ir);
    libfreenect2::FrameMap frames;

    //! [discovery]
    if(!(freenect2.enumerateDevices() == 0)) {

        std::string serial = freenect2.getDefaultDeviceSerialNumber();

        std::cout << "SERIAL: " << serial << std::endl;
        //! [discovery]

        //! [initialize Kinect]

        dev = freenect2.openDevice(serial);

        if (dev == 0) {
            std::cout << "failure opening device!" << std::endl;
            //return -1;
        }

    }

    faceDetector.initialize();
    catDetector.initialize();

    //VideoCapture stream1(0);
    //if (!stream1.isOpened()) { //check if video device has been initialised
      //  std::cout << "cannot open camera";
    //}

    std::string filename = "CatDetectionTest2.mov";
    VideoCapture stream1(filename);
    if (!stream1.isOpened()) { //check if video device has been initialised
         std::cout << "cannot open camera";
        }

//unconditional loop
    while (true) {

        Mat cameraFrame, frame_gray;
        stream1.read(cameraFrame);

        faceDetector.detectFaces(cameraFrame);
        catDetector.detectCats(cameraFrame);
        std::cout << "Amount of faces: " << faceDetector.faces.size() << std::endl;
        std::cout << "Amount of cats: " << faceDetector.faceRects.size() << std::endl;
        faceDetector.drawFaces(cameraFrame);
        catDetector.drawCats(cameraFrame);
        /*for( size_t i = 0; i < faces.size(); i++ )
        {
            Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
            ellipse( cameraFrame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

            Mat faceROI = frame_gray( faces[i] );
            std::vector<Rect> eyes;

            //-- In each face, detect eyes
            eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

            for( size_t j = 0; j < eyes.size(); j++ )
            {
                Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
                int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
                circle( cameraFrame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
            }
        }*/
        /*std::vector<Rect> smiles;
        if (faceDetector.faces.size() > 0){
            smiles = faceDetector.detectSmile(faceDetector.faces[0], cameraFrame);
            if(smiles.size() > 0) std::cout << "smile detected!"<<std::endl;
        }*/


        imshow("cam", cameraFrame);

        if ((waitKey(1) & 0xEFFFFF) == 27)
            break;
    }
    return 0;
}



int main_old(int argc, char **argv)
{
    // List of tracker types in OpenCV 3.2
    // NOTE : GOTURN implementation is buggy and does not work.
    string trackerTypes[6] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN"};
    // vector <string> trackerTypes(types, std::end(types));

    // Create a tracker
    string trackerType = trackerTypes[2];

    Ptr<Tracker> tracker;

#if (CV_MINOR_VERSION < 3)
    {
        tracker = Tracker::create(trackerType);
    }
#else
    {
        if (trackerType == "BOOSTING")
            tracker = TrackerBoosting::create();
        if (trackerType == "MIL")
            tracker = TrackerMIL::create();
        if (trackerType == "KCF")
            tracker = TrackerKCF::create();
        if (trackerType == "TLD")
            tracker = TrackerTLD::create();
        if (trackerType == "MEDIANFLOW")
            tracker = TrackerMedianFlow::create();
        if (trackerType == "GOTURN")
            tracker = TrackerGOTURN::create();
    }
#endif
    // Read video
    VideoCapture video("CatDetectionTest2.mov");

    // Exit if video is not opened
    if(!video.isOpened())
    {
        cout << "Could not read video file" << endl;
        return 1;

    }

    // Read first frame
    Mat frame;
    bool ok = video.read(frame);

    // Define initial boundibg box
    Rect2d bbox(287, 23, 86, 320);

    // Uncomment the line below to select a different bounding box
    bbox = selectROI(frame, false);

    // Display bounding box.
    rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
    imshow("Tracking", frame);

    tracker->init(frame, bbox);

    while(video.read(frame))
    {
        // Start timer
        double timer = (double)getTickCount();

        // Update the tracking result
        bool ok = tracker->update(frame, bbox);

        // Calculate Frames per second (FPS)
        float fps = getTickFrequency() / ((double)getTickCount() - timer);

        if (ok)
        {
            // Tracking success : Draw the tracked object
            rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
        }
        else
        {
            // Tracking failure detected.
            putText(frame, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }

        // Display tracker type on frame
        putText(frame, trackerType + " Tracker", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);

        // Display frame.
        imshow("Tracking", frame);

        // Exit if ESC pressed.
        int k = waitKey(1);
        if(k == 27)
        {
            break;
        }

    }
}