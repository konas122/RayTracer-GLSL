#include "camera/camera.h"
#include "shape/sphere.h"
#include "shape/model.h"
#include "shape/plane.h"
#include "shape/scene.h"
#include "util/rgb.h"
#include "material/diffuse.h"
#include "material/specular.h"
#include "material/dielectric.h"
#include "material/conductor.h"
#include "material/ground.h"
#include "light/infinite_light.hpp"
#include "renderer/pathRT.h"
#include "renderer/misPathRT.h"
#include "renderer/previewer.h"

int main() {
    Film film { 1280, 720 };
    Camera camera { film, { 0, 1.25, -6 }, { 0, 1.95, 0 }, 45 };

    Scene scene {};

    Model model("assets/buddha.obj");
    scene.addShape(
        model,
        std::make_shared<SpecularMaterial>(RGB(241, 191, 79)),
        { -3, 1.75, 0 },
        { 4, 4, 4 }
    );
    scene.addShape(
        model,
        std::make_shared<ConductorMaterial>(
            glm::vec3(1.2, 1.2, 5.3),
            glm::vec3(3.4, 3.4, 2.1),
            0.8, 0.2
        ),
        { -1, 1.75, 0 },
        { 4, 4, 4 }
    );
    scene.addShape(
        model,
        std::make_shared<DielectricMaterial>(
            1.4,
            glm::vec3(1, 1, 1),
            RGB(180, 180, 154),
            0.1, 0.3
        ),
        { 1, 1.75, 0 },
        { 4, 4, 4 }
    );
    scene.addShape(
        model,
        std::make_shared<DiffuseMaterial>(RGB(241, 191, 79)),
        { 3, 1.75, 0 },
        { 4, 4, 4 }
    );

    Sphere sphere {
        { 0, 0, 0 },
        1
    };
    scene.addShape(
        sphere,
        std::make_shared<SpecularMaterial>(glm::vec3(1, 1, 1)),
        {0, 3.75, 3}
    );

    Plane ground {
        { 0, 0, 0 },
        { 0, 1, 0 },
        100
    };
    scene.addShape(
        ground,
        std::make_shared<GroundMaterial>(glm::vec3(1, 1, 1))
    );

    scene.addInfiniteLight(new InfiniteLight { { 0.5, 0.5, 0.5 } });

    scene.build();

    PathTracingRenderer path_tracing_renderer { camera, scene };
    Previewer previewer(path_tracing_renderer);
    if (previewer.preview()) {
        path_tracing_renderer.render(32, "PT_MIS_TEST.ppm");
    }

    return 0;
}
