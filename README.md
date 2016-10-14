CUDA Path Tracer
================

**University of Pennsylvania, CIS 565: GPU Programming and Architecture, Project 3**

* Liang Peng
* Tested on: Windows 10, i7-6700HQ @ 2.6GHz 2.6GHz 8GB, GTX 960M (Personal Laptop)

### Feature
* [x] Ray Scattering
  * [x] Diffuse
  * [x] Refraction
  * [x] Specular Reflection
  * [x] Glossy Reflection
* [x] Depth of Field
* [x] Stratified Antialiasing
* [ ] Performance Analysis

### Overview
<img src="img/cover_depth8_5000spp.png"><br>
The image is rendered by a path tracer on GPU. Features include diffuse, glossy, specular reflection, refraction, depth of field, and stratified antialiasing.

### Ray Scattering
* Lambert Diffuse

Trace Depth 2, SPP 1000 | Trace Depth 8, SPP 1000
--- | ---
<img src="img/lambert_depth2_1000spp.png" width="400"> | <img src="img/lambert_depth8_1000spp.png" width="400">
_observation_ Images rendered with deeper trace level appear darker, which is due to the calculation of ray color. The resulted color is calculated by multiplying the material color of all intersections along the path.

* Refraction

Refraction 1, SPP 1000 | Refraction 2, SPP 5000
--- | ---
<img src="img/refraction_depth8_1000spp.png" width="400"> | <img src="img/refraction2_depth8_1000spp.png" width="400">
_observation_ Since large amount of paths are traced, caustics produced by refraction are free.

* Specular Reflection

Trace Depth 2, SPP 1000 | Trace Depth 8, SPP 1000
--- | ---
<img src="img/mirror_depth2_1000spp.png" width="400"> | <img src="img/mirror_depth8_1000spp.png" width="400">

* Glossy Reflection

Less Glossy Floor, SPP 3000 | Glossier Floor, SPP 3000
--- | ---
<img src="img/glossy_depth8_3000spp.png" width="400"> | <img src="img/glossy2_depth8_3000spp.png" width="400">
_obervation_ For different types of material, glossiness can be adjusted by tuning the specular exponent. Larger exponent produces reflection closer to mirror.

### Depth of Field

Depth of Field, Far Focal | Depth of Field, Near Focal
--- | ---
<img src="img/DoF_depth8_3000spp.png" width="400"> | <img src="img/DoF2_depth8_3000spp.png" width="400">
_obervation_ There are 2 parameters we can play around with in terms of depth of field, lens radius and focal distance. Larger lens radius produces stronger DoF effect. Focal distance affects the distance the virtual camera is focusing on.

### Antialiasing

Antialiasing OFF | Antialiasing ON
--- | ---
<img src="img/aa3_depth8_2000spp.png" width="400"> | <img src="img/aa4_depth8_2000spp.png" width="400">
_obersvation_ By jittering the camera ray within the ray's own pixel and performing large number of iterations, the final color of a pixel is computed by averaging all rays going through the pixel, producing antialiased image.
