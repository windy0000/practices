#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/utility.hpp>

// 将一张特征图拉成列为一维
cv::Mat flatten(const cv::Mat& feature_map);
cv::Mat flattenFeatureMaps(std::vector<cv::Mat> feature_maps);

// 把一维的向量拉回特征图
cv::Mat Reshape(cv::Mat& flattened, int r);
std::vector<cv::Mat> ReshapeFlattened(cv::Mat flattened,int num,int r,int c);
/*
* @brief 画训练时进度条,共25个长度
* 
* @parm epoch 第epoch轮
* @parm total_num 数据集总大小
* @parm total_loaded 之前批次加载总量
* @parm num 当前第多少张
*/
void train_progress_bar(int epoch,int total_num,int total_loaded,int num);