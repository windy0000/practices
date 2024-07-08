#pragma once
#include "constants.h"
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/utility.hpp>


/**
* @brief �����
* 
* @note ��ѵ������Ϊ����˵ĸ�������+1��ƫ��
*/
class ConvolutionalLayer
{
public:
	std::vector<cv::Mat> inputVector;
	std::vector<cv::Mat> outputVector;

	int numKernel;					// ���������
	int KernelSize;					// �����С
	std::vector<cv::Mat> kernels;	// �����
	double bias;					// ƫ����

	int stride;						// ����
	int padding;					// ���

	std::vector<std::vector<int>> convIndices;  // ÿ�������Ҫ����ļ�������ͼ������
	std::vector<cv::Mat> gradients;	// ���ݶ�


	// �����ֱ�Ϊ�������������С��ͨ�������������
	void init(int num, int size ,int stride, int paddding, std::vector<std::vector<int>> convIndices);

	//  �����ά����ͼ�͵��Ŷ�ά���ͼ������
	cv::Mat Conv(cv::Mat feature_map,cv::Mat kernel,int stride,int padding);
	// �����������о���˾��,����������ouputVector�в����ؼ�����ֵ
	std::vector<cv::Mat> forward(const std::vector<cv::Mat>& input);
	std::vector<cv::Mat> backward(const std::vector<cv::Mat>& sensitivity, std::vector<std::vector<int>> rt,int k);
	void optim(double alpha);
	void clear();

	std::vector<cv::Mat> get_outputVector();
};



/**
* @brief �ػ���
* 
* @note û��ѵ���Ĳ�����ͨ��ѡ��ػ�
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
// ƽ���ػ���
class AveragePoolingLayer:public PoolingLayer
{
public:
	void init(int size,int stride);

	// @brief �����ά����ͼ�͵��Ŷ�ά�ػ����
	cv::Mat AvgPool(cv::Mat feature_map, cv::Mat kernel, int stride);
	std::vector<cv::Mat> forward(const std::vector<cv::Mat>& input);
	/**
	* @brief �ػ���û��Ҫѧϰ�Ĳ���������ֻ����������ǰ����
	* 
	* @parm sensitivity �Գػ������ֵƫ��
	* @parm pooledSize �ػ���Ĵ�С
	* @return �Ծ������ֵ��ƫ��
	*/	
	std::vector<cv::Mat> backward(std::vector<cv::Mat> sensitivity,int pooledSize);
	void clear();
};



/**
* @brief ȫ���Ӳ�
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

	cv::Mat gradient;	// ���ݶ�

	void init(int inputSize, int outputSize);

	cv::Mat forward(const cv::Mat& input);

	
	// ���ض�ǰһ�㼤��ֵ��ƫ��
	cv::Mat backward(const cv::Mat& sensitivity);
	void optim(double alpha);
	void clear();
	
	cv::Mat get_OutputVector();
	cv::Mat get_w();
	cv::Mat get_b();
};