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

#include "material/ground.h"
#include "material/diffuse.h"
#include "material/specular.h"
#include "material/conductor.h"
#include "material/dielectric.h"

#include "renderer/previewer.h"

#include <memory>
#include <iostream>


int main() {
    Film film{192 * 4, 108 * 4};
    Camera camera{film, {-10, 2, 0}, {0, 1.5, 0}, 45};

    Model model("assets/dragon_871k.obj");
    Sphere sphere{
        {0, 0, 0},
        1
    };
    Plane plane{
        {0, 0, 0},
        {0, 1, 0}
    };

    RNG rng{};
    Scene scene{};
    for (int i = -3; i <= 3; i ++) {
        scene.addShape(
            sphere,
            std::make_shared<DielectricMaterial>(
                1.f + 0.2f * (i + 3),
                glm::vec3(1, 1, 1),
                (3.f - i) / 18.f,
                (3.f - i) / 6.f
            ),
            {0, 0.5, i * 2},
            {0.8, 0.8, 0.8}
        );
    }
    for (int i = 0; i <= 3; i ++) {
        scene.addShape(
            sphere,
            std::make_shared<DiffuseMaterial>(glm::vec3(rng.uniform(), rng.uniform(), rng.uniform())),
            {0, 4.5, i * 2},
            {0.8, 0.8, 0.8}
        );
    }
    for (int i = -3; i < 0; i ++) {
        scene.addShape(
            sphere,
            std::make_shared<SpecularMaterial>(glm::vec3(rng.uniform(), rng.uniform(), rng.uniform())),
            {0, 4.5, i * 2},
            {0.8, 0.8, 0.8}
        );
    }
    for (int i = -3; i <= 3; i ++) {
        glm::vec3 c = RGB::GenerateHeatmapRGB((i + 3.f) / 6.f);
        scene.addShape(
            sphere,
            std::make_shared<ConductorMaterial>(
                glm::vec3(2.f - c * 2.f),
                glm::vec3(2.f + c * 3.f),
                (3.f - i) / 18.f,
                (3.f - i) / 6.f
            ),
            {0, 2.5, i * 2},
            {0.8, 0.8, 0.8}
        );
    }
    scene.addShape(
        model,
        std::make_shared<DielectricMaterial>(
            1.8, RGB(128, 191, 131),
            0.4, 0.4
        ),
        {-5, 0.4, 1.5},
        {2, 2, 2}
    );
    scene.addShape(
        model,
        std::make_shared<ConductorMaterial>(
            glm::vec3(0.1, 1.2, 1.8),
            glm::vec3(5, 2.5, 2),
            0.4, 0.4
        ),
        {-5, 0.4, -1.5},
        {2, 2, 2}
    );
    scene.addShape(plane, std::make_shared<GroundMaterial>(RGB(120, 204, 157)), {0, -0.5, 0});
    auto light_material = std::make_shared<DiffuseMaterial>(glm::vec3(1, 1, 1));
    light_material->setEmissive({0.95 * 2, 0.95 * 2, 1 * 2});
    scene.addShape(plane, light_material, {0, 10, 0});
    scene.build();

    // NormalRenderer normal_renderer{camera, scene};
    // normal_renderer.render(1, "normal.ppm");

    // BoundsTestCountRenderer btc_renderer{camera, scene};
    // btc_renderer.render(1, "BTC.ppm");
    // TriangleTestCountRenderer ttc_renderer{camera, scene};
    // ttc_renderer.render(1, "TTC.ppm");

    PathTracingRenderer path_tracing_renderer{camera, scene};
    Previewer previewer(path_tracing_renderer);
    if (previewer.preview()) {
        path_tracing_renderer.render(4096, "PT_microfacet_test.ppm");
    }

    return 0;
}
