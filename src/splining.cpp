#include "splining.h"


typedef vector<Mat_<float>> Pyramid;

Mat_<float> expand(Mat_<float> &src, const Mat_<float> &w, const cv::Size &size) {
    //Mat_<float> expanded(size.height, size.width, 0.0f);

    //for (int i = 0; i < src.rows; i++) {
    //    for (int j = 0; j < src.cols; j++) {
    //        expanded(2 * i, 2 * j) = 4 * src(i, j);
    //    }
    //}

    //float32x4_t neon_w = vld1q_f32(&w(0, 0));  // Load w coefficients into a NEON register

    //for (int i = 0; i < expanded.rows; i += 4) {
    //    for (int j = 0; j < expanded.cols; j++) {
    //        float32x4_t expanded_vals = vld1q_f32(&expanded(i, j));  // Load 4 values from expanded
    //        float32x4_t result = vmulq_f32(expanded_vals, neon_w);   // Multiply using NEON
    //        vst1q_f32(&expanded(i, j), result);                      // Store back the result
    //    }
    //}

    //return convolution_with_reflection(expanded, w);

    Mat_<float> expanded(size.height, size.width, 0.0f);
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            expanded(2 * i, 2 * j) = 4 * src(i, j);
        }
    }
    return convolution_with_reflection(expanded, w);

}


Mat_<float> reduce(Mat_<float> &src, Mat_<float> &w) {
    Mat_<float> reduced(src.rows / 2, src.cols / 2);
    Mat_<float> blurred = convolution_with_reflection(src, w);
    for (int i = 0; i < reduced.rows; i++) {
        for (int j = 0; j < reduced.cols; j++) {
            reduced(i, j) = blurred(2 * i, 2 * j);
        }
    }

    return reduced;
}

//Mat_<float> reduce(Mat_<float>& src, Mat_<float>& w) {
//
//    Mat_<float> reduced(src.rows / 2, src.cols / 2);
//
//    Mat_<float> blurred = convolution_with_reflection(src, w);
//
//    float32x4_t vsum = vdupq_n_f32(0.0f);
//
//    for (int i = 0; i < reduced.rows; i++) {
//        for (int j = 0; j < reduced.cols; j++) {
//
//            // Load 4 adjacent pixels from blurred
//            float32x4_t v1 = vld1q_f32(&blurred(2 * i, 2 * j));
//            float32x4_t v2 = vld1q_f32(&blurred(2 * i, 2 * j + 1));
//            float32x4_t v3 = vld1q_f32(&blurred(2 * i + 1, 2 * j));
//            float32x4_t v4 = vld1q_f32(&blurred(2 * i + 1, 2 * j + 1));
//
//            // Add adjacent pixels
//            float32x4_t vsum = vaddq_f32(v1, v2);
//            vsum = vaddq_f32(vsum, v3);
//            vsum = vaddq_f32(vsum, v4);
//
//            // Store sum to reduced
//            reduced(i, j) = vgetq_lane_f32(vsum,0);
//
//           
//        }
//    }
//
//    return reduced;
//}

vector<Mat_<float>> compute_gaussian_pyramid(Mat_<float> &src, Mat_<float> &w) {
    Pyramid G = {src};
    while (std::min(G.back().rows, G.back().cols) > 2) {
        G.push_back(reduce(G.back(), w));
    }
    return G;
}


vector<Mat_<float>> compute_laplacian_pyramid(Mat_<float> src, Mat_<float> w) {
    vector<Mat_<float>> G = compute_gaussian_pyramid(src, w);
    vector<Mat_<float>> L(G.size());
    L.back() = G.back().clone();

    for (int l = G.size() - 2; l >= 0; l--) {
        L[l] = G[l] - expand(G[l + 1], w, G[l].size());
    }
    return L;
}

//Mat_<Vec3b> merge_images(Mat_<Vec3b> &im_a, Mat_<Vec3b> &im_b, Mat_<float> region, int max_height) {
//    vector<Mat_<float>> vec_a = separate_channels(im_a);
//    vector<Mat_<float>> vec_b = separate_channels(im_b);
//    vector<Mat_<float>> vec_s;
//    for (int i = 0; i < 3; i++) {
//        vec_s.push_back(merge_images(vec_a[i], vec_b[i], region, max_height));
//    }
//    return merge_channels(vec_s);
//}
//
//
//Mat_<float> merge_images(Mat_<float> &im_a, Mat_<float> &im_b, Mat_<float> region, int max_height) {
//
//    Mat_<float> result;
//    Mat_<float> w = kernel.clone();
//    if (region.empty()) {
//        region = generate_half_mask(im_a.rows, im_a.cols);
//    }
//
//    vector<Mat_<float>> LA = compute_laplacian_pyramid(im_a, w);
//    vector<Mat_<float>> LB = compute_laplacian_pyramid(im_b, w);
//    vector<Mat_<float>> LS;
//    vector<Mat_<float>> masks = compute_gaussian_pyramid(region, w);
//
//
//    for (int l = 0; l < LA.size(); l++) {
//        LS.push_back(LA[l].mul(masks[l]) + LB[l].mul(1.0f - masks[l]));
//    }
//
//    result = LS.back().clone();
//    for (int l = LS.size() - 2; l >= 0; l--) {
//        result = LS[l] + expand(result, w, LS[l].size());
//    }
//
//    return result;
//}



Mat_<float> merge_multiple_images(vector<Mat_<float>> &images,
                                  vector<Mat_<float>> &regions) {
    vector<Pyramid> image_pyramids, region_pyramids;
    Mat_<float> w = kernel.clone();


    for (int i = 0; i < images.size(); i++) {
        image_pyramids.push_back(compute_laplacian_pyramid(images[i], w));
        region_pyramids.push_back(compute_gaussian_pyramid(regions[i], w));
    }

    Pyramid merge_pyramid;
    for (int l = 0; l < region_pyramids[0].size(); l++) {
        merge_pyramid.emplace_back(region_pyramids[0][l].rows,
                                   region_pyramids[0][l].cols, 0.f);
        for (int i = 0; i < region_pyramids.size(); i++) {
            merge_pyramid[l] += region_pyramids[i][l].mul(image_pyramids[i][l]);
        }
    }



    Mat_<float> merged = merge_pyramid.back().clone();

    for (int l = merge_pyramid.size() - 2; l >= 0; l--) {
        merged = merge_pyramid[l] + expand(merged, w, merge_pyramid[l].size());
    }

    return merged;


}


Mat_<Vec3b> merge_multiple_images(vector<Mat_<Vec3b>> &images,
                                  vector<Mat_<float>> &regions) {
    vector<vector<Mat_<float>>> separated(3);
    for (auto im : images) {
        auto channels = separate_channels(im);
        separated[0].push_back(channels[0]);
        separated[1].push_back(channels[1]);
        separated[2].push_back(channels[2]);
    }
    vector<Mat_<float>> separated_merge;

    separated_merge.push_back(merge_multiple_images(separated[0], regions));
    separated_merge.push_back(merge_multiple_images(separated[1], regions));
    separated_merge.push_back(merge_multiple_images(separated[2], regions));

    return merge_channels(separated_merge);

}