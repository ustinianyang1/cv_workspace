#include <opencv2/opencv.hpp>
#include <cctype>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

constexpr const char* IMAGE_DIR = "image";
constexpr const char* OUTPUT_DIR = "processed_images";

cv::Mat img_preprocess(const std::string& imgPath) {
    cv::Mat img = cv::imread(imgPath, cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        return cv::Mat();
    }

    const int sampledRows = (img.rows + 3) / 4;
    const int sampledCols = (img.cols + 3) / 4;
    cv::Mat sampled(sampledRows, sampledCols, CV_8UC1);

    int sr = 0;
    for (int r = 0; r < img.rows; r += 4, ++sr) {
        int sc = 0;
        for (int c = 0; c < img.cols; c += 4, ++sc) {
            sampled.at<uchar>(sr, sc) = img.at<uchar>(r, c);
        }
    }

    for (int r = 0; r < sampled.rows; ++r) {
        for (int c = 0; c < sampled.cols; ++c) {
            sampled.at<uchar>(r, c) &= 0xF0;
        }
    }

    return sampled;
}

fs::path get_root_dir(const fs::path& executablePath) {
    const fs::path exeDir = fs::absolute(executablePath).parent_path();
    return exeDir.empty() ? fs::current_path() : exeDir.parent_path();
}

bool is_supported_image(const fs::path& p) {
    std::string ext = p.extension().string();
    for (char& ch : ext) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".bmp";
}

int main(int argc, char* argv[]) {
    const fs::path rootDir = get_root_dir(argv[0]);
    const fs::path imageDir = rootDir / IMAGE_DIR;
    const fs::path outputDir = rootDir / OUTPUT_DIR;

    fs::create_directories(outputDir);

    if (!fs::exists(imageDir) || !fs::is_directory(imageDir)) {
        return 1;
    }

    int processedCount = 0;
    for (const auto& entry : fs::directory_iterator(imageDir)) {
        if (!entry.is_regular_file() || !is_supported_image(entry.path())) {
            continue;
        }

        cv::Mat processed = img_preprocess(entry.path().string());
        if (processed.empty()) {
            continue;
        }

        const fs::path outputPath = outputDir / entry.path().filename();
        if (!cv::imwrite(outputPath.string(), processed)) {
            continue;
        }
        ++processedCount;
    }

    return processedCount > 0 ? 0 : 2;
}