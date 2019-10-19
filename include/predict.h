/******************************************************************************
 Copyright© HITwh HERO-Robomaster2020 Group

 Author: Song Haodi on 2019.9.11

 Detail:
 *****************************************************************************/
#include <iostream>
#include <opencv2/opencv.hpp>

class Predict{
public:
    struct myPoint{
        cv::Point3f point3f[10];
        int num;
    }myPoint;

public:
    Predict();
    ~Predict();
    void run(cv::Point3f point3f ,double yaw, double pitch, double &x, double &y, double &z);
    void insert(cv::Point3f point3f);
    void adjust(double yaw, double pitch);
    void predict(double &x, double &y, double &z);
};
