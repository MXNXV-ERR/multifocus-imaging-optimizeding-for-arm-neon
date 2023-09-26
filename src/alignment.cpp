#include "alignment.h"


void align_a_on_b(Mat_<Vec3b> &im_a, const Mat_<Vec3b> &im_b) {

    cv::Mat gray1, gray2;
    cv::cvtColor(im_a, gray1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(im_b, gray2, cv::COLOR_BGR2GRAY);

    // Detect feature points in both images
    std::vector<cv::Point2f> points1, points2;
    cv::goodFeaturesToTrack(gray1, points1, 500, 0.01, 10);
    cv::goodFeaturesToTrack(gray2, points2, 500, 0.01, 10);

    // Calculate optical flow using Lucas-Kanade algorithm
    std::vector<uchar> status;
    std::vector<float> error;
    cv::calcOpticalFlowPyrLK(gray1, gray2, points1, points2, status, error);

    // Filter out points with large tracking error
    std::vector<cv::Point2f> filteredPoints1, filteredPoints2;
    for (size_t i = 0; i < status.size(); i++)
    {
        if (status[i] && error[i] < 10.0)
        {
            filteredPoints1.push_back(points1[i]);
            filteredPoints2.push_back(points2[i]);
        }
    }

    // Calculate the affine transformation matrix using RANSAC
    cv::Mat affineMatrix = cv::estimateAffinePartial2D(filteredPoints1, filteredPoints2);

    // Warp the first image using the affine transformation matrix
    //cv::Mat alignedImage1;
    cv::warpAffine(im_a, im_a, affineMatrix, im_b.size());
    ////-- Step 1: Detect the keypoints and extract descriptors using SURF
    //int minHessian = 400;
    //cv::Ptr<SURF> detector = SURF::create(minHessian);
    //std::vector<cv::KeyPoint> keypoints_a, keypoints_b;
    //cv::Mat descriptors_a, descriptors_b;
    //detector->detectAndCompute(im_a, cv::Mat(), keypoints_a, descriptors_a);
    //detector->detectAndCompute(im_b, cv::Mat(), keypoints_b, descriptors_b);

    ////-- Step 2: Matching descriptor vectors using FLANN matcher
    //cv::FlannBasedMatcher matcher;
    //std::vector<cv::DMatch> matches;
    //matcher.match(descriptors_a, descriptors_b, matches);
    //double max_dist = 0;
    //double min_dist = 100;
    ////-- Quick calculation of max and min distances between keypoints
    //for (int i = 0; i < descriptors_a.rows; i++) {
    //    double dist = matches[i].distance;
    //    if (dist < min_dist) min_dist = dist;
    //    if (dist > max_dist) max_dist = dist;
    //}

    ////-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
    //std::vector<cv::DMatch> good_matches;
    //for (int i = 0; i < descriptors_a.rows; i++) {
    //    if (matches[i].distance < 3 * min_dist) { good_matches.push_back(matches[i]); }
    //}


    ////-- Localize the object
    //std::vector<Point2f> points_a;
    //std::vector<Point2f> points_b;
    //for (size_t i = 0; i < good_matches.size(); i++) {
    //    //-- Get the keypoints from the good matches
    //    points_a.push_back(keypoints_a[good_matches[i].queryIdx].pt);
    //    points_b.push_back(keypoints_b[good_matches[i].trainIdx].pt);
    //}
    //cv::Mat H = cv::findHomography(points_a, points_b, cv::RANSAC);
    //cv::warpPerspective(im_a, im_a, H, im_b.size());

}

void align_on_middle_image(vector<Mat_<Vec3b>> &images) {
    for (int i = 0; i < images.size(); i++) {
        if (i == images.size() / 2) {
            continue;
        }
        align_a_on_b(images[i], images[images.size() / 2]);
    }
}