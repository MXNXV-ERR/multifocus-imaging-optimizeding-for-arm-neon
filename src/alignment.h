#ifndef MULTI_FOCUS_ALIGNMENT_H
#define MULTI_FOCUS_ALIGNMENT_H

#include <numeric>
#include <opencv2/opencv.hpp>
//#include <opencv2/xfeatures2d.hpp>
//#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d.hpp>
//#include <opencv2/xfeatures2d/nonfree.hpp>

#include "utils.h"




//void align_images(vector<Mat_<Vec3b>> &images);

void align_a_on_b(Mat_<Vec3b> &im_a, const Mat_<Vec3b> &im_b);

void align_on_middle_image(vector<Mat_<Vec3b>> &images);

#endif //MULTI_FOCUS_ALIGNMENT_H
