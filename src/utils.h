#ifndef MULTI_FOCUS_UTILS_H
#define MULTI_FOCUS_UTILS_H


#include <iostream>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using cv::Mat_;
using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::cerr;
using cv::Vec3b;
using cv::Point2f;

const cv::Mat_<float> kernel = (cv::Mat_<float>(5, 5) <<
    0.01f,  0.025f, 0.03f, 0.025f, 0.01f,
    0.025f, 0.0625f,0.075f,0.0625f,0.025f,
    0.03f,  0.075f, 0.09f, 0.075f, 0.03f,
    0.025f, 0.0625f,0.075f,0.0625f,0.025f,
    0.01f,  0.025f, 0.03f, 0.025f, 0.01f
    );


template<typename TimeT = std::chrono::milliseconds>
struct measure {
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F &&func, Args &&... args) {
        auto start = std::chrono::steady_clock::now();
        std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast<TimeT>
                (std::chrono::steady_clock::now() - start);
        return duration.count();
    }
};


inline void scale(cv::Mat &m, double alpha = 0, double beta = 1) {
    cv::normalize(m, m, alpha, beta, cv::NORM_MINMAX);
}

//inline Mat_<float> laplacian(Mat_<float> &src, int ksize = 3) {
//    Mat_<float> gradx, grady, laplacian;
//    cv::Sobel(src, gradx, CV_32F, 2, 0, ksize);
//    cv::Sobel(src, grady, CV_32F, 0, 2, ksize);
//
//    laplacian = cv::abs(gradx) + cv::abs(grady);
//    return laplacian;
//}


inline Mat_<float> load_grayscale(const string &filename) {
    Mat_<float> original, out;
    original = cv::imread(filename, cv::IMREAD_GRAYSCALE); // Read the file
    if (original.empty())                      // Check for invalid input
    {
        cerr << "Could not open or find the image" << endl;
    }
    original.convertTo(out, CV_32F);
    out /= 255.f;
    return out;
}

inline Mat_<float> to_grayscale(Mat_<Vec3b> &src) {
    cv::Mat out;
    cv::cvtColor(src, out, cv::COLOR_BGR2GRAY);
    out.convertTo(out, CV_32F);
    out /= 255.f;
    return out;
}

inline void display_and_block(cv::Mat &im) {
    cv::imshow("window", im);
    cv::waitKey();
}

inline Mat_<Vec3b> load_color(const string &filename) {
    Mat_<Vec3b> original = cv::imread(filename);
    if (original.empty())                      // Check for invalid input
    {
        cerr << "Could not open or find " << filename << endl;
    }
    return original;
}

template<class T>
inline Mat_<T> subrect_clone(Mat_<T> &m, const cv::Rect &r) {
    return m.colRange(r.x, r.x + r.width).rowRange(r.y, r.y + r.height).clone();
}

inline vector<Mat_<float>> separate_channels(Mat_<Vec3b> &im) {
    vector<Mat_<float>> result(3);
    cv::Mat split[3];
    cv::split(im, split);
    for (int i = 0; i < 3; i++) {
        split[i].convertTo(result[i], CV_32F);
        result[i] /= 255.f;
    }

    return result;
}

inline Mat_<Vec3b> merge_channels(vector<Mat_<float>> &vec) {
    Mat_<cv::Vec3f> merged;
    Mat_<Vec3b> result;
    cv::merge(vec, merged);
    merged *= 255.f;
    merged.convertTo(result, CV_8UC3);
    return result;
}

inline Mat_<float> convolution_with_reflection(Mat_<float> &a, Mat_<float> k) {
    Mat_<float> result;
    cv::filter2D(a, result, CV_32F, k, cv::Point(-1, -1), 0, cv::BORDER_REFLECT101);

    //const cv::Mat_<float> kernelX = (cv::Mat_<float>(1, 5) << 0.01f, 0.025f, 0.03f, 0.025f, 0.01f);
    //const cv::Mat_<float> kernelY = kernelX.t();
    //cv::sepFilter2D(a, result, CV_32F, kernelX, kernelY, cv::Point(-1, -1), 0, cv::BORDER_REFLECT101);
    return result;
}

inline Mat_<Vec3b> convolution_with_reflection(Mat_<Vec3b> &a, Mat_<float> k) {
    Mat_<Vec3b> result;
    cv::filter2D(a, result, CV_8UC3, k, cv::Point(-1, -1), 0, cv::BORDER_REFLECT101);
    //const cv::Mat_<float> kernelX = (cv::Mat_<float>(1, 5) << 0.01f, 0.025f, 0.03f, 0.025f, 0.01f);
    //const cv::Mat_<float> kernelY = kernelX.t();
    //cv::sepFilter2D(a, result, CV_8UC3, kernelX, kernelY, cv::Point(-1, -1), 0, cv::BORDER_REFLECT101);
    return result;
}

#endif //MULTI_FOCUS_UTILS_H
