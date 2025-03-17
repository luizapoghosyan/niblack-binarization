#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
#include "lodepng/lodepng.h" 

using namespace std;

void Niblack(
    uint8_t* target,
    uint8_t* source,
    int image_width,
    int image_height,
    int image_stride,
    int window_width,
    int window_height,
    double stdev_coeff
) {
    int half_w = window_width / 2;
    int half_h = window_height / 2;

    vector<vector<double>> integral(image_height + 1, vector<double>(image_width + 1, 0));
    vector<vector<double>> integral_sq(image_height + 1, vector<double>(image_width + 1, 0));

    for (int y = 0; y < image_height; ++y) {
        for (int x = 0; x < image_width; ++x) {
            int pixel = source[y * image_stride + x];
            double pixel_sq = pixel * pixel;

            integral[y + 1][x + 1] = integral[y + 1][x] + integral[y][x + 1] - integral[y][x] + pixel;
            integral_sq[y + 1][x + 1] = integral_sq[y + 1][x] + integral_sq[y][x + 1] - integral_sq[y][x] + pixel_sq;
        }
    }

    for (int y = 0; y < image_height; ++y) {
        for (int x = 0; x < image_width; ++x) {
            int x0 = max(0, x - half_w);
            int x1 = min(image_width - 1, x + half_w);
            int y0 = max(0, y - half_h);
            int y1 = min(image_height - 1, y + half_h);

            int area = (x1 - x0 + 1) * (y1 - y0 + 1);
            double sum = integral[y1 + 1][x1 + 1] - integral[y0][x1 + 1] - integral[y1 + 1][x0] + integral[y0][x0];
            double sum_sq = integral_sq[y1 + 1][x1 + 1] - integral_sq[y0][x1 + 1] - integral_sq[y1 + 1][x0] + integral_sq[y0][x0];
            
            double mean = sum / area;
            double variance = (sum_sq / area) - (mean * mean);
            double std_dev = sqrt(variance);
            double threshold = mean + stdev_coeff * std_dev;

            target[y * image_stride + x] = (source[y * image_stride + x] >= threshold) ? 255 : 0;
        }
    }
}

void load_png(const char* filename, vector<uint8_t>& image, unsigned& width, unsigned& height) {
    unsigned error = lodepng::decode(image, width, height, filename, LCT_GREY, 8);
    if (error) {
        cerr << "Error loading PNG: " << lodepng_error_text(error) << endl;
        exit(1);
    }
}

void save_png(const char* filename, const vector<uint8_t>& image, unsigned width, unsigned height) {
    unsigned error = lodepng::encode(filename, image, width, height, LCT_GREY, 8);
    if (error) {
        cerr << "Error saving PNG: " << lodepng_error_text(error) << endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " input.png output.png window_size stdev_coeff" << endl;
        return 1;
    }
    
    const char* input_filename = argv[1];
    const char* output_filename = argv[2];
    int window_size = stoi(argv[3]);
    double stdev_coeff = stod(argv[4]);

    unsigned width, height;
    vector<uint8_t> image;
    load_png(input_filename, image, width, height);

    vector<uint8_t> output_image(image.size());
    Niblack(output_image.data(), image.data(), width, height, width, window_size, window_size, stdev_coeff);

    save_png(output_filename, output_image, width, height);
    cout << "Binarization completed: " << output_filename << endl;
    return 0;
}
