#ifndef PLANE_H_
#define PLANE_H_

#include "hittable.h"
#include "AGLM.h"

class plane : public hittable {
public:
   plane() : a(0), n(0), mat_ptr(0) {}
   plane(const glm::point3& p, const glm::vec3& normal, 
      std::shared_ptr<material> m) : a(p), n(normal), mat_ptr(m) {};

   virtual bool hit(const ray& r, hit_record& rec) const override
   {/* using namespace glm; 
      vec3 unitn = normalize(rec.normal);
      vec3 lightDir = normalize(vec3(5, 5, 0) - rec.p);
      glm::color diffuse = max(vec3(0), dot(unitn, lightDir)) * albedo;
      scattered = ray(rec.p, rec.normal + random_unit_vector());
      //check if 0
      attenuation = albedo;
      return true; //it hits!
      //check reading listing 46 
      //attenuation is like the color the scattered is the reflected ray */

       using namespace glm; 
       float bot = dot(n, random_unit_vector());
       if (bot > LONG_MIN) {
           vec3 p = rec.normal - r.dir;
           float t = dot(rec.normal, n);
           return (t >= 0);
       }
       return false;


   }



public:
   glm::vec3 a;
   glm::vec3 n;
   std::shared_ptr<material> mat_ptr;
};

#endif
