#pragma once

#include<iostream>
#include <vector>
#include "constants.h"
#include <opencv2/opencv.hpp>

struct MNISTData
{
	// ��������28*28��vector��ģ���Ϊ�˷����������
	std::vector<cv::Mat> image;
	// ��ǩ���ݣ��洢��д���ֵı�ǩ����Ӧ�����֣�
	std::vector<double> label;
};
/**
* @brief ��txt�ļ��л�ȡ���ݼ�λ��
* 
* @parm path txt�ļ�·��
* @return ���ݼ�·��
* 
* @note ����·��ֻ���ڵ�ǰ��Ŀ�ļ���
*/
std::string getDirfromtxt(std::string path);
/**
* @brief ����ָ����С�����ݼ�
* 
* @parm imageFile Ҫ���ص�ͼƬ·��
* @parm labelFile Ҫ���صı�ǩ·��
* @parm loaded �����˶�����ͼƬ�� 
* @parm batchSize Ҫ���ص����ݴ�С
* @return MNISTData �ṹ���������ص����ݼ�
* 
* @note �и�bug�������ݼ�ʣ������<64ʱ�����ǻ��ȡ64������
* @note �������ݺ�δ���ң���Ϊ��֪����������ͼƬ��ǩ��Ӧ˳�򲻱�
*/
MNISTData load_data(std::string imageFile,std::string labelFile,int loaded,int batchsize);