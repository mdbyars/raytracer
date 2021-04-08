#ifndef TRIANGLE_H_
#define TRIANGLE_H_


#include "hittable.h"
#include "AGLM.h"

class triangle : public hittable {
public:
   triangle() : a(0), b(0), c(0), mat_ptr(0) {}
   triangle(const glm::point3& v0, const glm::point3& v1, const glm::point3& v2, 
      std::shared_ptr<material> m) : a(v0), b(v1), c(v2), mat_ptr(m) {};

   virtual bool hit(const ray& r, hit_record& rec) const override
   {
       using namespace glm;
      vec3 V = (b - a);
       vec3 W = (c - a);
  
 
       float magr = length(r.direction());
       vec3 n = r.direction() / magr;
       float bot = dot(n, random_unit_vector());
       vec3 p = cross(n, W);
       float a = dot(V, p);
       if (fabs(a) < eps) {
           return -1;
       }
       float f = 1 / a;
       vec3 s = r.origin() - a;
       float u = f * dot(s, p);
       if (u < 0.0 || u > 1.0) {
           return -1
       }
       vec3 q = cross(s, V);
       float v = f * (dot(n, q));
       if (v < 0.0 || u + v > 1.0) {
           return false;
       }
       float t = f * (dot(W, q));
       return true;

     
   }

public:
   glm::point3 a;
   glm::point3 b;
   glm::point3 c;
   std::shared_ptr<material> mat_ptr;
};

#endif
