cmake_minimum_required(VERSION 3.7)
project(FaceDetection)

set(CMAKE_CXX_STANDARD 11)

find_package( OpenCV )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(freenect2 REQUIRED)
include_directories("/usr/include/libusb-1.0/")
INCLUDE_DIRECTORIES(
        ${freenect2_INCLUDE_DIR}
)

set(SOURCE_FILES main.cpp FaceDetector.cpp FaceDetector.h Face.cpp Face.h CatDetector.cpp CatDetector.h)
add_executable(FaceDetection ${SOURCE_FILES})
target_link_libraries( FaceDetection ${OpenCV_LIBS} )
target_link_libraries(FaceDetection ${freenect2_LIBRARIES})