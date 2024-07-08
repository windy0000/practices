#include "utils.h"

cv::Mat flatten(const cv::Mat& feature_map)
{
	int rows = feature_map.rows;
	int cols = feature_map.cols;
	return feature_map.reshape(1, rows * cols);
}
cv::Mat flattenFeatureMaps(std::vector<cv::Mat> feature_maps)
{
	std::vector<cv::Mat> flattened_feature_maps;

	int num = feature_maps.size();
	for (int i = 0; i < num; i++)
	{
		cv::Mat flattened = flatten(feature_maps[i]);
		flattened_feature_maps.push_back(flattened);
	}

	cv::Mat flattened_mat;
	cv::vconcat(flattened_feature_maps, flattened_mat);

	return flattened_mat;
}

cv::Mat Reshape(cv::Mat& flattened, int r)
{
	return flattened.reshape(1, r);
}
std::vector<cv::Mat> ReshapeFlattened(cv::Mat flattened, int num, int r, int c)
{
	std::vector<cv::Mat> reshaped_mats;
	int size = r * c;
	for(int i=0;i<num;++i)
	{
		cv::Mat tmp = flattened.rowRange(i*size,i*size+size);
		tmp = Reshape(tmp, r);
		reshaped_mats.push_back(tmp);
	}

	return reshaped_mats;
}

void train_progress_bar(int epoch, int total_num, int total_loaded, int num)
{
	int barWidth = 50;
	int trained = total_loaded + num;
	int pos = (int)(1.0 * trained / total_num * 50);

	std::cout << "µÚ" << epoch << "ÂÖ:[";
	for(int i=0;i<barWidth;++i)
	{
		if (i < pos)	std::cout << "=";
		else if (i == pos) std::cout << ">";
		else std::cout << " ";
	}

	std::cout << "]" << std::endl;
}