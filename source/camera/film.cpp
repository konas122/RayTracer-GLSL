#include "util/rgb.h"
#include "util/profile.h"
#include "camera/film.h"
#include "image/image.h"
#include "thread/thread_pool.h"

#include <vector>

Film::Film(size_t width, size_t height) : width(width), height(height) {
    pixels.resize(width * height);
}

void Film::save(const std::filesystem::path &filename) const {
    std::vector<glm::vec3> buffer(width * height);
    ThreadPool::getThreadPool()->parallelFor(width, height, [&](size_t x, size_t y) {
        auto pixel = getPixel(x, y);
        if (pixel.sample_count == 0) {
            return;
        }
        buffer[y * width + x] = pixel.color / static_cast<float>(pixel.sample_count);
    }, false);
    ThreadPool::getThreadPool()->wait();

    Image image(std::move(buffer), width, height);
    image.save(filename);
}


std::vector<uint8_t> Film::generateRGBABuffer() {
    std::vector<uint8_t> buffer(width * height * 4);

    for (size_t y = 0; y < height; y ++) {
        for (size_t x = 0; x < width; x ++) {
            auto pixel = getPixel(x, y);
            if (pixel.sample_count == 0) {
                continue;;
            }
            RGB rgb(pixel.color / static_cast<float>(pixel.sample_count));
            auto idx = (y * width + x) * 4;
            buffer[idx + 0] = rgb.r;
            buffer[idx + 1] = rgb.g;
            buffer[idx + 2] = rgb.b;
            buffer[idx + 3] = 255;
        }
    }

    return buffer;
}
