#pragma once

#include<iostream>
#include <vector>
#include "constants.h"
#include <opencv2/opencv.hpp>

struct MNISTData
{
	// 本来是用28*28的vector存的，但为了方便后面运算
	std::vector<cv::Mat> image;
	// 标签数据，存储手写数字的标签（对应的数字）
	std::vector<double> label;
};
/**
* @brief 从txt文件中获取数据集位置
* 
* @parm path txt文件路径
* @return 数据集路径
* 
* @note 返回路径只能在当前项目文件下
*/
std::string getDirfromtxt(std::string path);
/**
* @brief 加载指定大小的数据集
* 
* @parm imageFile 要加载的图片路径
* @parm labelFile 要加载的标签路径
* @parm loaded 加载了多少张图片了 
* @parm batchSize 要加载的数据大小
* @return MNISTData 结构，包含加载的数据集
* 
* @note 有个bug，当数据集剩余数量<64时，还是会读取64个数据
* @note 加载数据后未打乱，因为不知道怎样保持图片标签对应顺序不变
*/
MNISTData load_data(std::string imageFile,std::string labelFile,int loaded,int batchsize);