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
#include "renderer/simpleRT.h"

#include <iostream>


int main() {
    Film film { 192 * 4, 108 * 4 };
    Camera camera{film, {-3.6, 0, 0}, {0, 0, 0}, 45};

    Model model("assets/dragon_871k.obj");
    Sphere sphere{
        {0, 0, 0},
        1
    };
    Plane plane{
        {0, 0, 0},
        {0, 1, 0}
    };

    Scene scene {};
    scene.addShape(
        model,
        {RGB(202, 159, 117)},
        {0, 0, 0},
        {1, 3, 2}
    );
    scene.addShape(
        sphere,
        {{1, 1, 1}, false, RGB(255, 128, 128)},
        {0, 0, 2.5}
    );
    scene.addShape(
        sphere,
        {{1, 1, 1}, false, RGB(128, 128, 255)},
        {0, 0, -2.5}
    );
    scene.addShape(
        sphere,
        {{1, 1, 1}, true},
        {3, 0.5, -2}
    );
    scene.addShape(plane, {RGB(120, 204, 157)}, {0, -0.5, 0});

    NormalRenderer normal_renderer{camera, scene};
    normal_renderer.render(1, "normal.ppm");

    film.clear();

    SimpleRTRenderer simple_rt_renderer{camera, scene};
    simple_rt_renderer.render(32, "test.ppm");

    return 0;
}
