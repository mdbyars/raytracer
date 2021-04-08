#ifndef BOX_H_
#define BOX_H_

#include "hittable.h"
#include "AGLM.h"

class box : public hittable {
public:
   box() : c(0), ax(0), ay(0), az(0), hx(0), hy(0), hz(0), mat_ptr(0) {}
   box(const glm::point3& center, 
       const glm::vec3& xdir, const glm::vec3& ydir, const glm::vec3& zdir,
       const glm::vec3& halfx, const glm::vec3& halfy, const glm::vec3& halfz,
       std::shared_ptr<material> m) : c(center), ax(xdir), ay(ydir), az(zdir), 
          hx(halfx), hy(halfy), hz(halfz), mat_ptr(m) {};

   virtual bool hit(const ray& r, hit_record& rec) const override
   {
       using namespace glm;
       float tmin = -LONG_MAX; 
       float tmax = LONG_MAX;
       vec3 sides[] = { ax, ay, az};
       vec3 tops[] = { hx, hy, hz };
       point3 p = c - r.orig;
       for (int i = 0; i < 2; i++) {
           float e = dot(sides[i], p);
           float f = dot(sides[i], r.dir);
           if (abs(f) > eps) {
               //not sure what hi was intended to be in the doctumentation 
               float t1 = (e + tops[i].length()) / f;
               float t2 = (e - tops[i].length()) / f; 
               if (t1 > t2) {
                   swap(t1, t2);
               }
               if (t1 > tmin) {
                   tmin = t1;
               }
               if (t2 < tmax) {
                   tmax = t2;
               }
               if (tmin > tmax) {
                   return false;
               }
               if (tmax < 0) {
                   return false;
               }
               else if (-e - hi > 0 || -e + hi < 0) return -1;
               if (tmin > 0) {
                   return true;
            }
               else {
                   return true;
               }
           }
       }

      return false;
   }

public:
   glm::vec3 c;
   glm::vec3 ax;
   glm::vec3 ay;
   glm::vec3 az;
   glm::vec3 hx;
   glm::vec3 hy;
   glm::vec3 hz;
   std::shared_ptr<material> mat_ptr;
};

#endif
