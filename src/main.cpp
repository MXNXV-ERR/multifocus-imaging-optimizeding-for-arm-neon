#include <iostream>
#include <vector>
#include "utils.h"
#include "splining.h"
#include "alignment.h"
#include "custom_multifocus.h"

vector<Mat_<Vec3b>> load_images(string name, unsigned long n, string ext) {
    vector<Mat_<Vec3b>> images(n);
    for (int i = 0; i < n; i++) {
        std::string filename = name + std::to_string(i) + '.' + ext;
        images[i] = load_color(filename);
    }

    return images;
}


int main(int argc, char *argv[]) {

    if (argc < 6) {
        cout << "Usage: multifocus path_prefix n_images extension realign size_of_morph_mask" << endl;
        cout << "Example: multifocus small_stuff_anim0 6 jpg 2" << endl;
        return 0;
    }
    auto images = load_images(argv[1], std::stoi(string(argv[2])), argv[3]);
    bool realign = string(argv[4]) == "1";
    int size_of_mask = std::stoi(string(argv[5]));
    if (realign) {
        align_on_middle_image(images);
    }
    vector<Mat_<Vec3b>> results;
    for (auto i : images)
        cv::pyrDown(i,i);



    auto start = std::chrono::high_resolution_clock::now();

    auto elapsed = measure<>::execution([&results, &images, size_of_mask]() {
        results.push_back(custom_multifocus(images, size_of_mask));
    });
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

    cv::imwrite(string(argv[1]) + "_out_" + std::to_string(1) + ".png", results[0]);

    return 0;
}