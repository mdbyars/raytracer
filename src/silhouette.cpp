#include "ppm_image.h"
#include "AGLM.h"
#include "ray.h"

using namespace glm;
using namespace agl;

float hit_sphere(const ray& ray, const vec3& center, float radius) {
    vec3 oc = ray.origin() - center;
    float a = dot(ray.direction(), ray.direction());
    float b = 2.0f * dot(oc, ray.direction());
    float c = dot(oc, oc) - radius * radius;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return -1;
    return (-b - sqrt(discriminant)) / (2 * a);
}
color ray_color(const ray& ray)
{
    vec3 sphere_center(0, 0, -1);
    float t = hit_sphere(ray, sphere_center, 0.5f);
    if (t > 0) {
        return color(1.0, 0.0, 0.0);
    }

    vec3 unit_direction = normalize(ray.direction());

    float alpha = 0.5f * (unit_direction.y + 1.0f);
    color c1(0.5f, 0.7f, 1.0f);
    color c2(1.0);
    color c = c2 * (1 - alpha) + c1 * alpha;
    return c;
}

/*ray_color(ray)
….
sphere_center = point3(0, 0, -1)
t = hit_sphere(sphere_center, 0.5, ray)
if t > 0
hit_point = ray.origin + t * ray.direction
normal = normalize(hit_point – sphere_center);
color = 0.5 * (normal + vec3(1))
return color
dir = normalize(ray.direction);
t = 0.5f * (dir.y + 1.0f);
return white * (1 - t) + lightBlue * t;

*/
void ray_trace(ppm_image& image)
{
    int height = image.height();
    int width = image.width();

    float aspect = width / (float)height;
    float world_height = 2.0f;
    float world_width = world_height * aspect;
    float focal_length = 1.0f;

    vec3 camera_pos = vec3(0);
    vec3 lower_left = camera_pos
        - 0.5f * vec3(world_width, world_height, 0)
        - vec3(0, 0, focal_length);

    for (int i = 0; i < height; i++) // rows
    {
        for (int j = 0; j < width; j++) // cols
        {
            float ux = j / ((float)width - 1) * world_width;
            float uy = i / ((float)height) * world_height;
            ray r(camera_pos, lower_left + vec3(ux, uy, 0));
            color c = ray_color(r); 
            ppm_pixel p = { c.r*255, c.b*255, c.g*255 };
            image.set(i, j, p);
        }
    }
    image.save("silhouette.png");
}