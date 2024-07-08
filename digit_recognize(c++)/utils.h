#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/utility.hpp>

// ��һ������ͼ������Ϊһά
cv::Mat flatten(const cv::Mat& feature_map);
cv::Mat flattenFeatureMaps(std::vector<cv::Mat> feature_maps);

// ��һά��������������ͼ
cv::Mat Reshape(cv::Mat& flattened, int r);
std::vector<cv::Mat> ReshapeFlattened(cv::Mat flattened,int num,int r,int c);
/*
* @brief ��ѵ��ʱ������,��25������
* 
* @parm epoch ��epoch��
* @parm total_num ���ݼ��ܴ�С
* @parm total_loaded ֮ǰ���μ�������
* @parm num ��ǰ�ڶ�����
*/
void train_progress_bar(int epoch,int total_num,int total_loaded,int num);