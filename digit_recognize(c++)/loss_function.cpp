#include "loss_function.h"

cv::Mat cross_entropy_softmax_backward(const cv::Mat& y_hat, const cv::Mat y_true)
{
    return y_hat - y_true;
}
