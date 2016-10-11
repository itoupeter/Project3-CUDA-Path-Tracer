#pragma once

#include <vector>
#include "scene.h"

void pathtraceInit(Scene *scene);
void pathtraceFree();
void pathtrace(uchar4 *pbo, int frame, int iteration);

__global__ void gatherAndTerminate(int nPaths, glm::vec3 * image,
		PathSegment *iterationPaths, ShadeableIntersection *intersctions);

__global__ void shadeAllMaterial(int iter, int num_paths,
		ShadeableIntersection *shadeableIntersections,
		PathSegment *pathSegments, Material *materials);

__global__ void computeIntersections(int depth, int num_paths,
		PathSegment *pathSegments, Geom * geoms, int geoms_size,
		ShadeableIntersection *intersections);

__global__ void generateRayFromCamera(Camera cam, int iter, int traceDepth,
		PathSegment* pathSegments);

__global__ void scatterAliveRays(int iter, int depth, int num_paths,
		PathSegment *dev_paths, ShadeableIntersection *dev_intersections,
		Material *dev_materials);
