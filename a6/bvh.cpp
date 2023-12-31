#include <algorithm>
#include <cassert>
#include "bvh.hpp"

BVHAccel::BVHAccel(std::vector<Object *> p, int maxPrimsInNode,
                   SplitMethod splitMethod)
    : maxPrimsInNode(std::min(255, maxPrimsInNode)), splitMethod(splitMethod),
      primitives(std::move(p))
{
    time_t start, stop;
    time(&start);
    if (primitives.empty())
        return;

    root = recursiveBuild(primitives);

    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff / 3600;
    int mins = ((int)diff / 60) - (hrs * 60);
    int secs = (int)diff - (hrs * 3600) - (mins * 60);

    printf(
        "\rBVH Generation complete: \nTime Taken: %i hrs, %i mins, %i secs\n\n",
        hrs, mins, secs);
}

BVHBuildNode *BVHAccel::recursiveBuild(std::vector<Object *> objects)
{
    BVHBuildNode *node = new BVHBuildNode();

    // Compute bounds of all primitives in BVH node
    Bounds3 bounds;
    for (int i = 0; i < objects.size(); ++i)
        bounds = Union(bounds, objects[i]->getBounds());
    if (objects.size() == 1)
    {
        // Create leaf _BVHBuildNode_
        node->bounds = objects[0]->getBounds();
        node->object = objects[0];
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }
    else if (objects.size() == 2)
    {
        node->left = recursiveBuild(std::vector{objects[0]});
        node->right = recursiveBuild(std::vector{objects[1]});

        node->bounds = Union(node->left->bounds, node->right->bounds);
        return node;
    }
    else
    {
        Bounds3 centroidBounds;
        for (int i = 0; i < objects.size(); ++i)
            centroidBounds =
                Union(centroidBounds, objects[i]->getBounds().Centroid());
        int dim = centroidBounds.maxExtent();
        switch (dim)
        {
        case 0:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2)
                      { return f1->getBounds().Centroid().x <
                               f2->getBounds().Centroid().x; });
            break;
        case 1:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2)
                      { return f1->getBounds().Centroid().y <
                               f2->getBounds().Centroid().y; });
            break;
        case 2:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2)
                      { return f1->getBounds().Centroid().z <
                               f2->getBounds().Centroid().z; });
            break;
        }

        auto beginning = objects.begin();
        auto middling = objects.begin() + (objects.size() / 2);
        auto ending = objects.end();

        bool sah = this->splitMethod == SplitMethod::SAH;
        if (sah)
        {
            int properCut = 0;
            int part = 16;
            int size = objects.size();
            auto minCost = std::numeric_limits<float>::max();
            for (int i = 0; i < part; i++)
            {
                middling = objects.begin() + size * i / part;
                // middling = objects.begin() + i;
                auto leftshapes = std::vector<Object *>(beginning, middling);
                auto rightshapes = std::vector<Object *>(middling, ending);
                Bounds3 leftBounds, rightBounds;
                for (int i = 0; i < leftshapes.size(); ++i)
                    leftBounds = Union(leftBounds, leftshapes[i]->getBounds().Centroid());
                for (int i = 0; i < rightshapes.size(); ++i)
                    rightBounds = Union(rightBounds, rightshapes[i]->getBounds().Centroid());
                assert(objects.size() == leftshapes.size() + rightshapes.size());
                auto leftArea = leftBounds.SurfaceArea();
                auto rightArea = rightBounds.SurfaceArea();
                auto area = leftArea + rightArea;
                auto cost = (leftshapes.size() * leftArea + rightshapes.size() * rightArea) / area;
                if (cost < minCost)
                {
                    minCost = cost;
                    properCut = i;
                }
            }
            middling = objects.begin() + size * properCut / part;
            // middling = objects.begin() + properCut;
        }

        auto leftshapes = std::vector<Object *>(beginning, middling);
        auto rightshapes = std::vector<Object *>(middling, ending);

        assert(objects.size() == (leftshapes.size() + rightshapes.size()));

        node->left = recursiveBuild(leftshapes);
        node->right = recursiveBuild(rightshapes);

        node->bounds = Union(node->left->bounds, node->right->bounds);
    }

    return node;
}

Intersection BVHAccel::Intersect(const Ray &ray) const
{
    Intersection isect;
    if (!root)
        return isect;
    isect = BVHAccel::getIntersection(root, ray);
    return isect;
}

Intersection BVHAccel::getIntersection(BVHBuildNode *node, const Ray &ray) const
{
    // Traverse the BVH to find intersection
    Intersection isect;
    std::array<int, 3> dirIsNeg = {ray.direction.x > 0, ray.direction.y > 0, ray.direction.z > 0};
    bool isIntersect = node->bounds.IntersectP(ray, ray.direction_inv, dirIsNeg);
    if (!isIntersect)
        return isect;
    if (node->left == nullptr && node->right == nullptr)
    {
        isect = node->object->getIntersection(ray);
        return isect;
    }
    Intersection leftIsect = getIntersection(node->left, ray);
    Intersection rightIsect = getIntersection(node->right, ray);
    if (leftIsect.happened && rightIsect.happened)
    {
        if (leftIsect.distance < rightIsect.distance)
        {
            isect = leftIsect;
        }
        else
        {
            isect = rightIsect;
        }
    }
    else if (leftIsect.happened)
    {
        isect = leftIsect;
    }
    else if (rightIsect.happened)
    {
        isect = rightIsect;
    }
    return isect;
}