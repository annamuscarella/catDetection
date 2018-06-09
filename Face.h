//
// Created by Anna Bendel on 04.08.17.
//


#include <iostream>
#include <opencv/cv.hpp>
using namespace cv;

#ifndef FACEDETECTION_FACE_H
#define FACEDETECTION_FACE_H


class Face {
public:
Rect faceRect, eyeLeft, eyeRight;
    Point faceCenter, eyeLeftCenter, eyeRightCenter;

};


#endif //FACEDETECTION_FACE_H
