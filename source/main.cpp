#include "util/rgb.h"
#include "util/rand.h"
#include "util/frame.h"
#include "camera/film.h"
#include "camera/camera.h"
#include "shape/model.h"
#include "shape/plane.h"
#include "shape/scene.h"
#include "shape/sphere.h"
#include "thread/thread_pool.h"
#include "renderer/normal.h"
#include "renderer/pathRT.h"
#include "renderer/debugRT.h"

#include <iostream>


int main() {
    Film film{192 * 4, 108 * 4};
    Camera camera{film, {-12, 5, -12}, {0, 0, 0}, 45};

    Model model("assets/dragon_87k.obj");
    Sphere sphere{
        {0, 0, 0},
        1
    };
    Plane plane{
        {0, 0, 0},
        {0, 1, 0}
    };

    Scene scene {};
    RNG rng{};
    for (int i = 0; i < 10000; i ++) {
        glm::vec3 random_pos{
            rng.uniform() * 100 - 50,
            rng.uniform() * 2,
            rng.uniform() * 100 - 50,
        };
        float u = rng.uniform();
        if (u < 0.9) {
            scene.addShape(
                model,
                {RGB(202, 159, 117), rng.uniform() > 0.5},
                random_pos,
                {1, 1, 1},
                {rng.uniform() * 360, rng.uniform() * 360, rng.uniform() * 360}
            );
        }
        else if (u < 0.95) {
            scene.addShape(
                sphere,
                {{rng.uniform(), rng.uniform(), rng.uniform()}, true},
                random_pos,
                {0.4, 0.4, 0.4}
            );
        }
        else {
            random_pos.y += 6;
            scene.addShape(
                sphere,
                {{1, 1, 1}, false, {rng.uniform() * 4, rng.uniform() * 4, rng.uniform() * 4}},
                random_pos
            );
        }
    }
    scene.addShape(plane, { RGB(120, 204, 157) }, { 0, -0.5, 0 });
    scene.build();

    // NormalRenderer normal_renderer{camera, scene};
    // normal_renderer.render(1, "normal.ppm");

    // BoundsTestCountRenderer btc_renderer{camera, scene};
    // btc_renderer.render(1, "BTC.ppm");
    // TriangleTestCountRenderer ttc_renderer{camera, scene};
    // ttc_renderer.render(1, "TTC.ppm");

    PathTracingRenderer path_tracing_renderer{camera, scene};
    path_tracing_renderer.render(32, "PT_cosine_test.ppm");

    return 0;
}
