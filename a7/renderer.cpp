//
// Created by goksu on 2/25/20.
//

#include "renderer.hpp"
#include "omp.h"
#include <fstream>
#include "scene.hpp"

inline float deg2rad(const float &deg) { return deg * M_PI / 180.0; }

const float EPSILON = 0.00001;

int prog = 0;
omp_lock_t lock;

void paraRender(Vector3f eye_pos, std::vector<Vector3f> &framebuffer,
                const Scene &scene, int spp, float imageAspectRatio, float scale,
                int start, int end)
{
    int width = sqrt(spp), height = sqrt(spp);
    float step = 1.f / sqrt(spp);
    for (int j = start; j < end; j++)
    {
        for (int i = 0; i < scene.width; i++)
        {
            for (int k = 0; k < spp; k++)
            {
                float x = (2 * (i + step / 2 + step * (k % width)) / (float)scene.width - 1) *
                          imageAspectRatio * scale;
                float y = (1 - 2 * (j + step / 2 + step * (k / height)) / (float)scene.height) * scale;
                Vector3f dir = normalize(Vector3f(-x, y, 1));
                framebuffer[j * scene.width + i] += scene.castRay(Ray(eye_pos, dir), 0) / spp;
            }
        }
        omp_set_lock(&lock);
        prog++;
        UpdateProgress(prog / (float)scene.height);
        omp_unset_lock(&lock);
    }
}

// The main render function. This where we iterate over all pixels in the image,
// generate primary rays and cast these rays into the scene. The content of the
// framebuffer is saved to a file.
void Renderer::Render(const Scene &scene)
{
    std::vector<Vector3f> framebuffer(scene.width * scene.height);

    float scale = tan(deg2rad(scene.fov * 0.5));
    float imageAspectRatio = scene.width / (float)scene.height;
    Vector3f eye_pos(278, 273, -800);
    int m = 0;

    // change the spp value to change sample ammount
    int spp = 2048;
    std::cout << "SPP: " << spp << "\n";
    int thread_num = 32;
    int thread_step = scene.height / thread_num;
#pragma omp parallel for
    for (int i = 0; i < thread_num; i++)
        paraRender(eye_pos, std::ref(framebuffer), std::ref(scene), spp,
                   imageAspectRatio, scale, i * thread_step, (i + 1) * thread_step);
    UpdateProgress(1.f);

    // save framebuffer to file
    FILE *fp = fopen("binary.ppm", "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", scene.width, scene.height);
    for (auto i = 0; i < scene.height * scene.width; ++i)
    {
        static unsigned char color[3];
        color[0] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].x), 0.6f));
        color[1] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].y), 0.6f));
        color[2] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].z), 0.6f));
        fwrite(color, 1, 3, fp);
    }
    fclose(fp);
}
