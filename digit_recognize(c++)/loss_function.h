#pragma once
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/utility.hpp>

// ��Ϊ�����غ�softmax����������
cv::Mat cross_entropy_softmax_backward(const cv::Mat& y_hat,const cv::Mat y_true);