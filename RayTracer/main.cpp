/**
* Ordering of includes matters.
* stb_image_write.h must be included first of all, otherwise
* define _CRT_SECURE_NO_WARNINGS does not go through and VS will
* build with errors saying that fopen may be unsafe.
* I do not know what causes this specific behaviour.
**/

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifndef EIGENH
#define EIGENH
#include <Eigen/Core>
#include <Eigen/Geometry>
#endif 

#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "Material.h"
#include "utils.h"

#include <array>
#include <chrono>
#include <iostream>
#include <execution>


typedef uint8_t byte;
#define T_MIN utils::EPSILON

constexpr int RGB_CHANNELS = 3;
constexpr int ns           = 100;

using Eigen::Vector4f;

/**
* Recursively trace rays with the geometry until background is hit.
* Assumes normalized Ray direction vector.
**/
Vector4f color(const Ray &r, Hitable *world, int depth) {
  hit_record rec;
  if (world->hit(r, T_MIN, FLT_MAX, rec)) {
    Ray scatter_ray;
    Vector4f attenuation;
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scatter_ray)) {
      return attenuation.cwiseProduct(color(scatter_ray, world, depth + 1));
    }

    return Vector4f(0, 0, 0, 0);
  }

  //No intersection. Give background color instead.
  float t = 0.5f*(r.direction().y() + 1.0f);
  return (1.0f - t)*Vector4f(1.0f, 1.0f, 1.0f, 0.0f) + t * Vector4f(0.5f, 0.7f, 1.0f, 0.0f);
}


Hitable *random_scene() {
  int n = 500;
  Hitable **list = new Hitable*[n + 1];
  list[0] = new Sphere(Vector4f(0, -1000, 0, 0), 1000, new Lambertian(Vector4f(0.5, 0.5, 0.5, 0.0f)));

  int i = 1;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float choose_mat = utils::randf();
      Vector4f center(a + 0.9*utils::randf(), 0.2, b + 0.9*utils::randf(), 0.0f);
      if ((center - Vector4f(4, 0.2, 0, 0)).norm() > 0.9) {
        if (choose_mat < 0.8) { // choose diffuse material
          list[i++] = new Sphere(center, 0.2, new Lambertian(Vector4f(utils::randf()*utils::randf(), 
                                                                      utils::randf()*utils::randf(), 
                                                                      utils::randf()*utils::randf(), 
                                                                      0.0f)));
        }
        else if (choose_mat < 0.95) { // choose metal material
          list[i++] = new Sphere(center, 0.2, new Metal(Vector4f(0.5*(1 + utils::randf()), 
                                                                 0.5*(1 + utils::randf()), 
                                                                 0.5*(1 + utils::randf()),
                                                                 0.0f), 0.5*(1 + utils::randf())));
        }
        else { //choose glass material
          list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
        }
      }
    }
  }

  list[i++] = new Sphere(Vector4f(0, 1, 0, 0), 1.0, new Dielectric(1.5));
  list[i++] = new Sphere(Vector4f(-4, 1, 0, 0), 1.0, new Lambertian(Vector4f(0.4, 0.2, 0.1, 0)));
  list[i++] = new Sphere(Vector4f(4, 1, 0, 0), 1.0, new Metal(Vector4f(0.7, 0.6, 0.5, 0), 0.0));

  return new HitableList(list, i);
}

Ray sample_ray(const Camera &cam, const int nx, const int ny, const int ix, const int iy) {
  float u = (float(ix) + utils::randf()) / float(nx);
  float v = (float(ny - iy - 1) + utils::randf()) / float(ny);
  Ray r = cam.get_ray(u, v);
  r.normalize_direction();
  return r;
}

Vector4f pixel_color(const Camera &cam, Hitable *world, int nx, int ny, int ix, int iy) {
  Vector4f final_color(0, 0, 0, 0);
  for (int is = 0; is < ns; is++) {
    Ray r = sample_ray(cam, nx, ny, ix, iy);
    final_color += color(r, world, 0);
  }
  final_color /= float(ns);
  final_color = Vector4f(sqrt(final_color.x()) * 255.99,
                         sqrt(final_color.y()) * 255.99,
                         sqrt(final_color.z()) * 255.99,
                         0);
  return final_color;
}

int main() {
  const int nx = 1920;
  const int ny = 1080;

  byte *rgb_image = new byte[nx*ny*RGB_CHANNELS];

  Hitable *world = random_scene();

  Vector4f lookfrom = Vector4f(13.0f, 2.0f, 3.0f, 0.0f);
  Vector4f lookat = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
  float dist_to_focus = 10.0f;
  float aperture = 0.1f;
  float vfov = 20.0f;
  Camera cam(lookfrom, lookat, Vector4f(0.0f, 1.0f, 0.0f, 0.0f), vfov
    , static_cast<float>(nx) / static_cast<float>(ny)
    , aperture, dist_to_focus);

  std::cout << "Starting..." << std::endl;
  auto t1 = std::chrono::high_resolution_clock::now();
  int i = 0;
  for (int iy = 0; iy < ny; iy++) {
    //enumerate this pixel row
    std::array<int, nx> all_x; 
    std::iota(all_x.begin(), all_x.end(), 0);
    
    //colors for each x-pixel in this row
    std::array<Vector4f, nx> all_colors; 
    auto tracer = [&all_colors, &cam, &world, nx, ny, iy](auto ix) {  //update all_colors as a side effect
      all_colors[ix] = pixel_color(cam, world, nx, ny, ix, iy);
    };
    std::for_each(std::execution::par,        //for_each guarantees in-order execution
                  all_x.begin(), all_x.end(), 
                  tracer);

    //record traced colors
    for (auto pixel : all_colors) {
      rgb_image[i]     = byte(pixel.x());
      rgb_image[i + 1] = byte(pixel.y());
      rgb_image[i + 2] = byte(pixel.z());
      i += RGB_CHANNELS;
    }
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
  
  std::cout << duration << " ms" << std::endl;

  stbi_write_png("ch12.png", nx, ny, RGB_CHANNELS, rgb_image, 0);


  // de-allocation 
  delete[] rgb_image;
  delete world; //owns its list of Hitables and deletes them in its destructor
}
