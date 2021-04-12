#ifndef MATERIAL_H
#define MATERIAL_H

#include <cmath>
#include "AGLM.h"
#include "ray.h"
#include "hittable.h"

class material {
public:
  virtual bool scatter(const ray& r_in, const hit_record& rec, 
     glm::color& attenuation, ray& scattered) const = 0;
  virtual ~material() {}
};

class lambertian : public material {
public:
  lambertian(const glm::color& a) : albedo(a) {}

  virtual bool scatter(const ray& r_in, const hit_record& rec, 
     glm::color& attenuation, ray& scattered) const override 
  {
      using namespace glm; 
      vec3 unitn = normalize(rec.normal);
      vec3 lightDir = normalize(vec3(5, 5, 0) - rec.p);
      glm::color diffuse = max(vec3(0), dot(unitn, lightDir)) * albedo;
      scattered = ray(rec.p, rec.normal + random_unit_vector());
      //check if 0
      attenuation = albedo;
      return true; //it hits!
      //check reading listing 46 
      //attenuation is like the color the scattered is the reflected ray 
  }

public:
  glm::color albedo;
};

class vantaBlack : public material {
public:
    vantaBlack(const glm::color& a) : albedo(a) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec,
        glm::color& attenuation, ray& scattered) const override
    {
        using namespace glm;
        //making a kitchy material based on the famous paint that absorbs most light 
        //having troubles pushing
        attenuation = color(0, 0, 0);
        return false; //it does not reflect any light 
        //check reading listing 46 
        //attenuation is like the color the scattered is the reflected ray 
    }

public:
    glm::color albedo;
};

class phong : public material {
public:
  phong(const glm::vec3& view) :
     diffuseColor(0,0,1), 
     specColor(1,1,1),
     ambientColor(.01f, .01f, .01f),
     lightPos(5,5,0),
     viewPos(view), 
     kd(0.45), ks(0.45), ka(0.1), shininess(10.0) 
  {}

  phong(const glm::color& idiffuseColor, 
        const glm::color& ispecColor,
        const glm::color& iambientColor,
        const glm::point3& ilightPos, 
        const glm::point3& iviewPos, 
        float ikd, float iks, float ika, float ishininess) : 
     diffuseColor(idiffuseColor), 
     specColor(ispecColor),
     ambientColor(iambientColor),
     lightPos(ilightPos),
     viewPos(iviewPos), kd(ikd), ks(iks), ka(ika), shininess(ishininess) 
  {}

  virtual bool scatter(const ray& r_in, const hit_record& hit, 
     glm::color& attenuation, ray& scattered) const override 
  {
      using namespace glm;
    //  color is = albedo;
    //  color ia = ka * ambientColor;
     // color id = albedo;
      //I = id + is + ia
      //ambient ia = ka la
      //diffuse id = kd (l.nhat)ld md
      //is = ks ls v.r^alpha

      color ia = ka * ambientColor;
      vec3 l = lightPos - hit.p;
      color id = kd * diffuseColor * max(vec3(0, 0, 0), dot(normalize(l), normalize(hit.normal)));
     // if (dot(r_in.dir, hit.normal) <= 0) {
      //    id = albedo;
    //  }
      vec3 r = 2 * dot(normalize(l), normalize(hit.normal)) * hit.normal - l;
  //    else {
     //     id = kd * dot(r_in.dir, hit.normal) * ambientColor * albedo;
   //   }
      point3 v = viewPos - hit.p;
      color is = albedo;
      if (dot(normalize(v), r) > 0) {
          is = ks * specColor * pow(dot(normalize(v),r), shininess);

      }

     // if (dot(r_in.direction(), scattered.direction()) <= 0) {
       //   is = albedo;
    //  }
      else {
          is = ks * specColor * dot(normalize(v), r);
      }
     // else {
       //   is = ks * ambientColor * dot(r_in.direction(), scattered.direction());

     // }
      //not sure what to do here as no matter what I return it comes out black 
      attenuation = is + ia + id;
      
      //attenuation = color(1, 1, 0);
      //using the pieces from lambertian to perform scattering 
      vec3 unitn = normalize(hit.normal);
      vec3 lightDir = normalize(vec3(5, 5, 0) - hit.p);
      glm::color diffuse = max(vec3(0), dot(unitn, lightDir)) * albedo;
      scattered = ray(hit.p, hit.normal + random_unit_vector());
      return true;
  }

public:
  glm::color diffuseColor;
  glm::color specColor;
  glm::color ambientColor;
  glm::point3 lightPos;
  glm::point3 viewPos; 
  glm::color albedo;

  float kd; 
  float ks;
  float ka; 
  float shininess;

};

class metal : public material {
public:
   metal(const glm::color& a, float f) : albedo(a), fuzz(glm::clamp(f,0.0f,1.0f)) {}

   virtual bool scatter(const ray& r_in, const hit_record& rec, 
      glm::color& attenuation, ray& scattered) const override 
   {
       glm::vec3 reflected = reflect(r_in.direction(), rec.normal);
       scattered = ray(rec.p, reflected + fuzz * random_unit_vector());
       attenuation = albedo;
       return (dot(scattered.direction(), rec.normal) > 0);
      //return false;
   }

public:
   glm::color albedo;
   float fuzz;
};

class dielectric : public material {
public:
  dielectric(float index_of_refraction) : ir(index_of_refraction) {}

  virtual bool scatter(const ray& r_in, const hit_record& rec, 
     glm::color& attenuation, ray& scattered) const override 
   {
      using namespace glm;
    attenuation = color(1.0, 1.0, 1.0);
      float refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

      vec3 unit_direction = normalize(r_in.direction());
      float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
      float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

      bool cannot_refract = refraction_ratio * sin_theta > 1.0;
      vec3 direction;

      if (cannot_refract || reflect(cos_theta, refraction_ratio) > random_float())
          direction = reflect(unit_direction, rec.normal);
      else
          direction = refract(unit_direction, rec.normal, refraction_ratio);

      scattered = ray(rec.p, direction);
      
      return true;
   }

public:
  float ir; // Index of Refraction
};


#endif

//new feature 