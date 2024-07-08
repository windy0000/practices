#pragma once
#include "constants.h"
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/utility.hpp>


/**
* @brief 卷积层
* 
* @note 可训练参数为卷积核的各个参数+1个偏置
*/
class ConvolutionalLayer
{
public:
	std::vector<cv::Mat> inputVector;
	std::vector<cv::Mat> outputVector;

	int numKernel;					// 卷积核数量
	int KernelSize;					// 卷积大小
	std::vector<cv::Mat> kernels;	// 卷积核
	double bias;					// 偏置项

	int stride;						// 步长
	int padding;					// 填充

	std::vector<std::vector<int>> convIndices;  // 每个卷积核要卷积哪几个特征图的索引
	std::vector<cv::Mat> gradients;	// 存梯度


	// 参数分别为卷积核数量、大小、通道、步长、填充
	void init(int num, int size ,int stride, int paddding, std::vector<std::vector<int>> convIndices);

	//  计算二维特征图和单张二维卷积图卷积结果
	cv::Mat Conv(cv::Mat feature_map,cv::Mat kernel,int stride,int padding);
	// 抽象卷积层所有卷积核卷积,卷积结果存在ouputVector中并返回激活后的值
	std::vector<cv::Mat> forward(const std::vector<cv::Mat>& input);
	std::vector<cv::Mat> backward(const std::vector<cv::Mat>& sensitivity, std::vector<std::vector<int>> rt,int k);
	void optim(double alpha);
	void clear();

	std::vector<cv::Mat> get_outputVector();
};



/**
* @brief 池化层
* 
* @note 没有训练的参数，通过选择池化
*/
class PoolingLayer
{
public:
	enum PoolingType
	{
		MAX_POOLING,
		AVERAGE_POOLING
	};

	PoolingType poolingType;

	int kernelSize;
	cv::Mat kernel;
	int stride;

	std::vector<cv::Mat> inputVector;
	std::vector<cv::Mat> outputVector;
};
// 平均池化层
class AveragePoolingLayer:public PoolingLayer
{
public:
	void init(int size,int stride);

	// @brief 计算二维特征图和单张二维池化结果
	cv::Mat AvgPool(cv::Mat feature_map, cv::Mat kernel, int stride);
	std::vector<cv::Mat> forward(const std::vector<cv::Mat>& input);
	/**
	* @brief 池化层没有要学习的参数，所以只负责把误差向前传递
	* 
	* @parm sensitivity 对池化层输出值偏导
	* @parm pooledSize 池化后的大小
	* @return 对卷积激活值的偏导
	*/	
	std::vector<cv::Mat> backward(std::vector<cv::Mat> sensitivity,int pooledSize);
	void clear();
};



/**
* @brief 全连接层
* 
* 
*/
class FullyConnectedLayer
{
public:
	cv::Mat inputVector;
	cv::Mat outputVector;

	int InputSize;
	int OutputSize;

	cv::Mat w;
	cv::Mat b;

	cv::Mat gradient;	// 存梯度

	void init(int inputSize, int outputSize);

	cv::Mat forward(const cv::Mat& input);

	
	// 返回对前一层激活值的偏导
	cv::Mat backward(const cv::Mat& sensitivity);
	void optim(double alpha);
	void clear();
	
	cv::Mat get_OutputVector();
	cv::Mat get_w();
	cv::Mat get_b();
};