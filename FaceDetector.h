//
// Created by Anna Bendel on 04.08.17.
//

#include <iostream>
#include <opencv/cv.hpp>
#include "Face.h"

using namespace cv;

#ifndef FACEDETECTION_FACEDETECTOR_H
#define FACEDETECTION_FACEDETECTOR_H


class FaceDetector {

public:
    std::vector<Rect> faceRects;
    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;
    CascadeClassifier smile_cascade;

    std::vector<Face> faces;

    int initialize();
    int detectFaces(Mat cameraInput);
    int drawFaces(Mat matToDrawOn);
    std::vector<Rect> detectSmile(Face currFace, Mat cameraMat);
};


#endif //FACEDETECTION_FACEDETECTOR_H
