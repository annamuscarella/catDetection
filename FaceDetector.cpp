//
// Created by Anna Bendel on 04.08.17.
//

#include "FaceDetector.h"

int FaceDetector::initialize() {
    String face_cascade_name = "../resources/haarcascade_frontalface_default.xml";
    String eyes_cascade_name = "../resources/haarcascade_eye_tree_eyeglasses.xml";
    String smile_cascade_name = "../resources/haarcascade_smile.xml";
    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
    if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
    if( !smile_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
    return 0;
}

int FaceDetector::detectFaces(Mat cameraInput) {
    faces =  std::vector<Face>();
    Mat frame_gray;
    cvtColor( cameraInput, frame_gray, CV_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    face_cascade.detectMultiScale( frame_gray, faceRects, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    for( size_t i = 0; i < faceRects.size(); i++ )
    {
        Face face;
        face.faceRect = faceRects[i];
        face.faceCenter = Point( faceRects[i].x + faceRects[i].width*0.5, faceRects[i].y + faceRects[i].height*0.5 );

        Mat faceROI = frame_gray( faceRects[i] );
        std::vector<Rect> eyes;

        //-- In each face, detect eyes
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

        if (eyes.size() == 2){
            if(eyes[0].x < eyes[1].x){
                face.eyeLeft = eyes[0];
                face.eyeLeftCenter = Point(faceRects[i].x + eyes[0].x + eyes[0].width*0.5, faceRects[i].y + eyes[0].y + eyes[0].height*0.5);
                face.eyeRight = eyes[1];
                face.eyeRightCenter = Point(faceRects[i].x + eyes[1].x + eyes[1].width*0.5, faceRects[i].y + eyes[1].y + eyes[1].height*0.5);
            }
            else{
                face.eyeRight = eyes[0];
                face.eyeRightCenter = Point(faceRects[i].x + eyes[0].x + eyes[0].width*0.5, faceRects[i].y + eyes[0].y + eyes[0].height*0.5);
                face.eyeLeft = eyes[1];
                face.eyeLeftCenter = Point(faceRects[i].x + eyes[1].x + eyes[1].width*0.5, faceRects[i].y + eyes[1].y + eyes[1].height*0.5);
            }
            faces.push_back(face);
        }

    }
    return 0;
}

int FaceDetector::drawFaces(Mat matToDrawOn) {
    for( size_t i = 0; i < faces.size(); i++ )
    {
        ellipse( matToDrawOn, faces[i].faceCenter, Size( faces[i].faceRect.width*0.5, faces[i].faceRect.height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
        ellipse(matToDrawOn, faces[i].eyeLeftCenter, Size(faces[i].eyeLeft.width * 0.5, faces[i].eyeLeft.height * 0.5), 0,0, 360, Scalar(0,255,0), 4, 8, 0);
        ellipse(matToDrawOn, faces[i].eyeRightCenter, Size(faces[i].eyeRight.width * 0.5, faces[i].eyeRight.height * 0.5), 0,0, 360, Scalar(0,255,0), 4, 8, 0);


    }
    return 0;
}

std::vector<Rect> FaceDetector::detectSmile(Face currFace, Mat cameraInput) {
    Mat roi_gray = cameraInput(cv::Range(currFace.faceCenter.x - currFace.faceRect.width, currFace.faceCenter.y - currFace.faceRect.height), cv::Range(currFace.faceCenter.x + currFace.faceRect.width, currFace.faceCenter.y + currFace.faceRect.height));
    cvtColor( cameraInput, roi_gray, CV_BGR2GRAY );
    equalizeHist( roi_gray, roi_gray );
    std::vector<Rect> smiles;
    smile_cascade.detectMultiScale( roi_gray, smiles, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );



    return smiles;
}
