#pragma once

#include "intersections.h"
#include <glm/gtx/rotate_vector.hpp>

// CHECKITOUT
/**
 * Computes a cosine-weighted random direction in a hemisphere.
 * Used for diffuse lighting.
 */
__host__ __device__
glm::vec3 calculateRandomDirectionInHemisphere(
        glm::vec3 normal, thrust::default_random_engine &rng) {
    thrust::uniform_real_distribution<float> u01(0, 1);

    float up = sqrt(u01(rng)); // cos(theta)
    float over = sqrt(1 - up * up); // sin(theta)
    float around = u01(rng) * TWO_PI;

    // Find a direction that is not the normal based off of whether or not the
    // normal's components are all equal to sqrt(1/3) or whether or not at
    // least one component is less than sqrt(1/3). Learned this trick from
    // Peter Kutz.

    glm::vec3 directionNotNormal;
    if (abs(normal.x) < SQRT_OF_ONE_THIRD) {
        directionNotNormal = glm::vec3(1, 0, 0);
    } else if (abs(normal.y) < SQRT_OF_ONE_THIRD) {
        directionNotNormal = glm::vec3(0, 1, 0);
    } else {
        directionNotNormal = glm::vec3(0, 0, 1);
    }

    // Use not-normal direction to generate two perpendicular directions
    glm::vec3 perpendicularDirection1 =
        glm::normalize(glm::cross(normal, directionNotNormal));
    glm::vec3 perpendicularDirection2 =
        glm::normalize(glm::cross(normal, perpendicularDirection1));

    return glm::normalize(up * normal
        + cos(around) * over * perpendicularDirection1
        + sin(around) * over * perpendicularDirection2);
}

/**
 * Scatter a ray with some probabilities according to the material properties.
 * For example, a diffuse surface scatters in a cosine-weighted hemisphere.
 * A perfect specular surface scatters in the reflected ray direction.
 * In order to apply multiple effects to one surface, probabilistically choose
 * between them.
 *
 * The visual effect you want is to straight-up add the diffuse and specular
 * components. You can do this in a few ways. This logic also applies to
 * combining other types of materias (such as refractive).
 *
 * - Always take an even (50/50) split between a each effect (a diffuse bounce
 *   and a specular bounce), but divide the resulting color of either branch
 *   by its probability (0.5), to counteract the chance (0.5) of the branch
 *   being taken.
 *   - This way is inefficient, but serves as a good starting point - it
 *     converges slowly, especially for pure-diffuse or pure-specular.
 * - Pick the split based on the intensity of each material color, and divide
 *   branch result by that branch's probability (whatever probability you use).
 *
 * This method applies its changes to the Ray parameter `ray` in place.
 * It also modifies the color `color` of the ray in place.
 *
 * You may need to change the parameter list for your purposes!
 */
__host__ __device__
void scatterRay(PathSegment &path, const ShadeableIntersection &intersection,
        const Material &material, thrust::default_random_engine &rng) {

    // TODO: implement this.
    // A basic implementation of pure-diffuse shading will just call the
    // calculateRandomDirectionInHemisphere defined above.

	const Ray &ray = path.ray;
	Ray newRay = path.ray;
	thrust::uniform_real_distribution<float> u01(0, 1);

	if (material.hasReflective > 0.f) {
		// specular reflection
		float theta = acos(pow(u01(rng), 1.f / (material.specular.exponent + 1.f)));
		float phi = TWO_PI * u01(rng);
		glm::vec3 mirror = glm::normalize(glm::reflect(ray.direction,
				intersection.surfaceNormal));
		glm::vec3 up = {0.f, 0.f, 1.f};
		glm::vec3 axis = glm::normalize(glm::cross(up, mirror));
		float angle = acos(glm::dot(up, mirror));

		newRay.direction = glm::vec3(cos(phi) * sin(theta),
				sin(phi) * sin(theta), cos(theta));
		newRay.direction = glm::normalize(
				glm::rotate(newRay.direction, angle, axis));
	} else if(material.hasRefractive > 0.f) {
		// refraction
		float cosine = glm::dot(intersection.surfaceNormal, ray.direction);

		if (glm::abs(cosine) < 1e-2f) {
			newRay.direction = ray.direction;
		} else if (cosine < 0.f) {
			newRay.direction = glm::refract(ray.direction,
					intersection.surfaceNormal,	1.f / material.indexOfRefraction);
		} else {
			newRay.direction = glm::refract(ray.direction,
					-intersection.surfaceNormal, material.indexOfRefraction);
		}
		// total reflection
		if (glm::length(newRay.direction) < 1e-3f){
			newRay.direction = glm::reflect(ray.direction,
					-intersection.surfaceNormal);
		}
	} else {
		// lambert reflection
		newRay.direction = calculateRandomDirectionInHemisphere(
				intersection.surfaceNormal, rng);
	}

	newRay.origin = ray.origin + ray.direction * intersection.t
			+ newRay.direction * 1e-3f;
	path.ray = newRay;
}
