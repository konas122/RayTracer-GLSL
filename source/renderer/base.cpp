#include "renderer/base.h"
#include "util/progress.h"
#include "util/formatter.h"
#include "thread/thread_pool.h"

#include <format>
#include <iostream>

void BaseRenderer::render(const size_t spp, const std::filesystem::path &file) {
    size_t current_spp = 0, increase = 1;
    auto &film = camera.getFilm();
    film.clear();
    Progress progress(film.getWidth() * film.getHeight() * spp, 20);
    while (current_spp < spp) {
        ThreadPool::getThreadPool()->parallelFor(
            film.getWidth(), film.getHeight(),
            [&](size_t x, size_t y) {
                for (int i = 0; i < static_cast<int>(increase); i ++) {
                    film.addSample(x, y, renderPixel({ x, y }));
                }
                progress.update(increase);
            }
        );
        ThreadPool::getThreadPool()->wait();

        current_spp += increase;
        increase = std::min<size_t>(current_spp, 32);

        film.save(file);
        std::cout << std::format("{}spp has been saved to {}", current_spp, file.filename()) << std::endl;
    }
}
