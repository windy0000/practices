#include <iostream>
#include <cmath>
#include "activation_function.h"
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
cv::Mat softmax(const cv::Mat& x)
{
	// 防止上溢出
	double max_value;
	cv::minMaxLoc(x, NULL, &max_value);

	cv::Mat exp_x;
	cv::exp(x-max_value, exp_x);
	double sum = 0;
	int num = exp_x.rows;
	for (int i = 0; i < num; ++i)
	{
		sum += exp_x.at<double>(i, 0);
	}

	cv::Mat ret = exp_x / sum;
	return ret;
}

cv::Mat sigmoid(const cv::Mat& x)
{	
	// 防止上溢,得对公式进行变换
	int r = x.rows, c = x.cols;
	cv::Mat result = cv::Mat(r, c, CV_64F);

	for(int i=0;i<r;++i)
	for(int j=0;j<c;++j)
	{
		double exp_x = x.at<double>(i, j);

		if (exp_x >= 0)
		{
			exp_x = std::exp(-exp_x);
			result.at<double>(i, j) = 1 / (1 + exp_x);
		}
		else
		{
			exp_x = std::exp(exp_x);
			result.at<double>(i, j) = exp_x / (1 + exp_x);
		}
			
	}

	return result;
}
std::vector<cv::Mat> get_sigmoid(std::vector<cv::Mat>& x)
{
	std::vector<cv::Mat> Result;
	int num = x.size();
	for(int i=0;i<num;++i)
	{
		cv::Mat tmp = x[i];
		tmp = sigmoid(tmp);

		Result.push_back(tmp);
	}
	return	Result;
}
cv::Mat sigmoid_gradient(cv::Mat x)
{
	// 计算sigmoid函数
	cv::Mat sigmoid_output = sigmoid(x);

	// 计算sigmoid函数的梯度
	cv::Mat sigmoid_gradient_output = sigmoid_output.mul(1 - sigmoid_output);

	return sigmoid_gradient_output;
}
std::vector<cv::Mat> get_sigmoid_gradient(std::vector<cv::Mat>& x)
{
	std::vector<cv::Mat> Result;
	int num = x.size();
	for(int i=0;i<num;++i)
	{
		cv::Mat tmp = x[i];
		tmp = sigmoid_gradient(tmp);

		Result.push_back(tmp);
	}

	return Result;
}
std::vector<cv::Mat> mul(std::vector<cv::Mat>& x, std::vector<cv::Mat>& y)
{
	std::vector<cv::Mat> Result;
	int num = x.size();
	for(int i=0;i<num;++i)
	{
		cv::Mat tmp = x[i].mul(y[i]);
		Result.push_back(tmp);
	}

	return Result;
}