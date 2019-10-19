/******************************************************************************
 Copyright© HITwh HERO-Robomaster2020 Group

 Author: Song Haodi on 2019.9.11

 Detail:
 *****************************************************************************/

#include "predict.h"
#include <iostream>
#include <opencv2/opencv.hpp>


//没识别到的情况是角度为零还是x，y，z为零

Predict::Predict() {
    this->myPoint.num=0;
    for(int i=0;i<10;i++){
        this->myPoint.point3f[i]=cv::Point3f(0,0,0);
    }
}

Predict::~Predict(){

}

//收集
void Predict::adjust(double yaw, double pitch) {
    for(int i=0;i<this->myPoint.num;i++){
        double x_temp = this->myPoint.point3f[i].x;
        double y_temp = this->myPoint.point3f[i].y;
        double z_temp = this->myPoint.point3f[i].z;

        this->myPoint.point3f[i].x = x_temp*cos(yaw)-z_temp*sin(yaw);
        this->myPoint.point3f[i].y = -x_temp*sin(pitch)*sin(yaw)+y_temp*cos(pitch)-z_temp*sin(pitch)*cos(yaw);
        this->myPoint.point3f[i].z = x_temp*cos(pitch)*sin(yaw)+y_temp*sin(pitch)+z_temp*cos(pitch)*cos(yaw);
    }
}

//预测
void Predict::predict(double &x, double &y, double &z) {
    x = 0;y = 0;z = 0;
    int arrX[]={1,2,3,4,5,6,7,8,9,10};
    //有可能因速率不匹配等问题不是11
    double predict_num=11;
    double LValue[10];
    //插值基函数中的上下累乘temp1,temp2
    double temp1,temp2;
    for(int i=0; i<10; i++)
    {
        temp1 = 1.0;//分子和分母不能为0，所以初始化为1.0
        temp2 = 1.0;
        for(int j=0; j<10; j++)
        {
            if(j==i)
            {
                continue;
            }
            temp1 *=(predict_num-arrX[j]);
            temp2 *=(arrX[i]-arrX[j]);
        }
        LValue[i] = temp1/temp2;//求出的一个分式
    }
    for(int i=0; i<10; i++)
    {
        x += this->myPoint.point3f[i].x*LValue[i];
        y += this->myPoint.point3f[i].y*LValue[i];
        z += this->myPoint.point3f[i].z*LValue[i];
    }
}

void Predict::insert(cv::Point3f point3f) {
    if(this->myPoint.num<10){
        this->myPoint.point3f[myPoint.num]=point3f;
        ++myPoint.num;
    }else{
        for(int i=0;i<9;i++){
            myPoint.point3f[i]=myPoint.point3f[i+1];
        }
        this->myPoint.point3f[myPoint.num-1]=point3f;
    }
}


//yaw和pitch是在转到当前point3f下的角度
void Predict::run(cv::Point3f point3f ,double yaw, double pitch, double &x, double &y, double &z) {
    insert(point3f);
    adjust(yaw,pitch);
    if(this->myPoint.num==10) {
        predict(x, y, z);
    }else{
        x=point3f.x;
        y=point3f.y;
        z=point3f.z;
    }
}
