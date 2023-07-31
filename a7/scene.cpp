//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "scene.hpp"

void Scene::buildBVH()
{
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k)
    {
        if (objects[k]->hasEmit())
        {
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k)
    {
        if (objects[k]->hasEmit())
        {
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum)
            {
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
    const Ray &ray,
    const std::vector<Object *> &objects,
    float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k)
    {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear)
        {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }

    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // Implement Path Tracing Algorithm here
    Intersection inter = intersect(ray);
    if (!inter.happened)
    {
        return Vector3f(0.0, 0.0, 0.0);
    }
    if (inter.m->hasEmission())
    {
        if (depth == 0)
        {
            return inter.m->getEmission();
        }
        else
        {
            return Vector3f(0.0, 0.0, 0.0);
        }
    }
    Vector3f l_dir(0.0, 0.0, 0.0);
    Intersection inter_light;
    float pdf_light = 0.0;
    sampleLight(inter_light, pdf_light);
    Vector3f obj_pos = inter.coords;
    Vector3f light_pos = inter_light.coords;
    Vector3f obj_normal = inter.normal;
    Vector3f light_normal = inter_light.normal;
    Vector3f pos_diff = light_pos - obj_pos;
    Vector3f light_dir = normalize(pos_diff);
    Ray shadow_ray(obj_pos, light_dir);
    Intersection inter_shadow = intersect(shadow_ray);
    if (inter_shadow.happened && (inter_shadow.coords - light_pos).norm() < 1e-2)
    {
        Vector3f f_r = inter.m->eval(ray.direction, light_dir, obj_normal);
        float light_distance = dotProduct(pos_diff, pos_diff);
        l_dir = f_r * inter_light.emit * dotProduct(light_dir, obj_normal) * dotProduct(-light_dir, light_normal) / (light_distance * pdf_light);
    }
    if (get_random_float() > RussianRoulette)
    {
        return l_dir;
    }
    Vector3f l_indir(0.0, 0.0, 0.0);
    Vector3f new_dir = inter.m->sample(ray.direction, obj_normal);
    new_dir = normalize(new_dir);
    Ray new_ray(obj_pos, new_dir);
    Intersection inter_new = intersect(new_ray);
    if (inter_new.happened)
    {
        Vector3f f_r = inter.m->eval(ray.direction, new_dir, obj_normal);
        float pdf = inter.m->pdf(ray.direction, new_dir, obj_normal);
        l_indir = castRay(new_ray, depth + 1) * f_r * dotProduct(new_dir, obj_normal) / (pdf * RussianRoulette);
    }
    return l_dir + l_indir;
}