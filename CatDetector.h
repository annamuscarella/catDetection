//
// Created by Anna Bendel on 11.05.18.
//

#ifndef FACEDETECTION_CATDETECTOR_H
#define FACEDETECTION_CATDETECTOR_H


#include <vector>
#include "Face.h"

class CatDetector {
public:
    std::vector<Rect> catRects;
    CascadeClassifier cat_cascade;
    CascadeClassifier cat_cascade_extended;

    int initialize();
    int detectCats(Mat cameraInput);
    int drawCats(Mat matToDrawOn);

};


#endif //FACEDETECTION_CATDETECTOR_H
