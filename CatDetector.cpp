//
// Created by Anna Bendel on 11.05.18.
//

#include "CatDetector.h"
int CatDetector::initialize() {
    String cat_cascade_name = "haarcascade_frontalcatface.xml";
    String cat_cascade_extended_name = "haarcascade_frontalcatface_extended.xml";

    if( !cat_cascade.load( cat_cascade_name) ){ printf("--(!)Error loading\n"); return -1; };
    if( !cat_cascade_extended.load( cat_cascade_extended_name) ){ printf("--(!)Error loading\n"); return -1; };
    return 0;
}

int CatDetector::detectCats(Mat cameraInput) {
    Mat frame_gray;
    cvtColor( cameraInput, frame_gray, CV_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    cat_cascade_extended.detectMultiScale(frame_gray, catRects, 1.3, 2, 0|CV_HAAR_SCALE_IMAGE, Size(50,50));

    return 0;
}

int CatDetector::drawCats(Mat matToDrawOn) {
    for( size_t i = 0; i < catRects.size(); i++ )
    {
        Point cat_center = Point( catRects[i].x + catRects[i].width*0.5, catRects[i].y + catRects[i].height*0.5 );
        ellipse( matToDrawOn, cat_center, Size( catRects[i].width*0.5, catRects[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
    }
    return 0;
}