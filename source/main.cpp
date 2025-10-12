#include "util/rgb.h"
#include "util/rand.h"
#include "camera/film.h"
#include "camera/camera.h"
#include "shape/model.h"
#include "shape/plane.h"
#include "shape/scene.h"
#include "shape/sphere.h"
#include "renderer/normal.h"
// #include "renderer/pathRT.h"
#include "renderer/debugRT.h"
#include "renderer/misPathRT.h"

#include "material/ground.h"
#include "material/diffuse.h"
#include "material/specular.h"
#include "material/conductor.h"
#include "material/dielectric.h"

#include "renderer/previewer.h"

#include <memory>


int main() {
    Film film { 960, 540 };
    glm::vec3 camera_pos = { 0, 37, -61 };
    Camera camera { film, camera_pos, { 0, 8, 0 }, 16 };

    Scene scene {};
    Triangle triangles[] = {
        { { -17, 0, -1.5 }, { -17, 0, 1.5 }, { 17, 0, 1.5 } },
        { { -17, 0, -1.5 }, { 17, 0, 1.5 }, { 17, 0, -1.5 } },
    };
    Sphere light_sphere_1 { { -15 + 0 / 3, 12, 8 }, 2 };
    Sphere light_sphere_2 { { -15 + 30 / 3, 12, 8 }, 1 };
    Sphere light_sphere_3 { { -15 + 60 / 3, 12, 8 }, 0.5 };
    Sphere light_sphere_4 { { -15 + 90 / 3, 12, 8 }, 0.1 };
    AreaLight area_light_1 { light_sphere_1, { 1, 1, 1 }, false };
    AreaLight area_light_2 { light_sphere_2, { 4, 4, 4 }, false };
    AreaLight area_light_3 { light_sphere_3, { 16, 16, 16 }, false };
    AreaLight area_light_4 { light_sphere_4, { 400, 400, 400 }, false };
    scene.addAreaLight(&area_light_1, std::make_shared<DiffuseMaterial>());
    scene.addAreaLight(&area_light_2, std::make_shared<DiffuseMaterial>());
    scene.addAreaLight(&area_light_3, std::make_shared<DiffuseMaterial>());
    scene.addAreaLight(&area_light_4, std::make_shared<DiffuseMaterial>());
    glm::vec3 light_pos_center = { 0, 12, 8 };

    float alphas[] = { 0.4, 0.25, 0.16, 0.04 };
    for (size_t i = 0; i < 4; i ++) {
        float theta = glm::radians(i * 15.f);
        glm::vec3 center { 0, 17 * (1 - glm::cos(theta)), 17 * glm::sin(theta) };
        glm::vec3 normal = glm::normalize(glm::normalize(light_pos_center - center) + glm::normalize(camera_pos - center));
        float rotate_x = -glm::degrees(glm::acos(normal.y));
        auto surface_material = std::make_shared<ConductorMaterial>(
            glm::vec3(2, 2, 1),
            glm::vec3(3, 3, 15),
            alphas[i],
            alphas[i]
        );
        scene.addShape(triangles[0], surface_material, center, { 1, 1, 1 }, { rotate_x, 0, 0 });
        scene.addShape(triangles[1], surface_material, center, { 1, 1, 1 }, { rotate_x, 0, 0 });
    }

    Plane ground {
        { 0, -0.5, 0 },
        { 0, 1, 0 },
        100
    };
    Plane wall {
        { 0, 0, 15 },
        { 0, 0, -1 },
        100
    };
    scene.addShape(ground, std::make_shared<GroundMaterial>(glm::vec3(1, 1, 1)));
    scene.addShape(wall, std::make_shared<DiffuseMaterial>(glm::vec3(1, 1, 1)));
    // scene.addInfiniteLight(new InfiniteLight { { 0.5, 0.5, 0.5 } });

    scene.build();

    NormalRenderer normal_renderer{camera, scene};
    normal_renderer.render(1, "normal.ppm");

    BoundsTestCountRenderer btc_renderer{camera, scene};
    btc_renderer.render(1, "BTC.ppm");
    TriangleTestCountRenderer ttc_renderer{camera, scene};
    ttc_renderer.render(1, "TTC.ppm");

    MISPathTracingRenderer path_tracing_renderer{camera, scene};
    Previewer previewer(path_tracing_renderer);
    // if (previewer.preview()) {
        path_tracing_renderer.render(32, "PT_microfacet_test.ppm");
    // }

    return 0;
}
