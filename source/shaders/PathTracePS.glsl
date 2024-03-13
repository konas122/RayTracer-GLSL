#version 330 core
#define PI 3.14159265

in vec2 screenCoord;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D envMap;
uniform vec2 screenSize;

out vec4 FragColor;


// ================================================

uint m_u = uint(521288629);
uint m_v = uint(362436069);

uint GetUintCore(inout uint u, inout uint v) {
	v = uint(uint(36969) * (v & uint(65535)) + (v >> 16));
	u = uint(uint(18000) * (u & uint(65535)) + (u >> 16));
	return (v << 16) + u;
}

float GetUniformCore(inout uint u, inout uint v) {
	// 0 <= u <= 2^32
	uint z = GetUintCore(u, v);
	// The magic number is 1/(2^32 + 1) and so result is positive and less than 1.
	return float(z) / uint(4294967295);
}

float GetUniform() {
	return GetUniformCore(m_u, m_v);
}

uint GetUint() {
	return GetUintCore(m_u, m_v);
}

float rand() {
	return GetUniform();
}

vec2 rand2() {
	return vec2(rand(), rand());
}

vec3 rand3() {
	return vec3(rand(), rand(), rand());
}

vec4 rand4() {
	return vec4(rand(), rand(), rand(), rand());
}

vec3 random_in_unit_sphere() {
	vec3 p;
	
	float theta = rand() * 2.0 * PI;
	float phi   = rand() * PI;
	p.y = cos(phi);
	p.x = sin(phi) * cos(theta);
	p.z = sin(phi) * sin(theta);

	return p;
}


// ================================================

struct Ray {
    vec3 origin;
    vec3 direction;
};

Ray RayConstructor(vec3 origin, vec3 direction) {
    Ray ray;
    ray.origin = origin;
    ray.direction = direction;
    return ray;
}

vec3 RayAt(Ray ray, float t) {
    return ray.origin + t * ray.direction;
}


// ================================================

struct Camera {
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
};

Camera CameraConstructor(vec3 lower_left_corner, vec3 horizontal, vec3 vertical, vec3 origin) {
	Camera camera;

	camera.lower_left_corner = lower_left_corner;
	camera.horizontal = horizontal;
	camera.vertical = vertical;
	camera.origin = origin;

	return camera;
}


Ray CameraGetRay(Camera camera, vec2 offset) {
	Ray ray = RayConstructor(camera.origin, 
		                     camera.lower_left_corner + 
		                     offset.x * camera.horizontal + 
                             offset.y * camera.vertical);
	return ray;
}


// ================================================

struct Sphere {
    vec3 center;
    float radius;
};

struct HitRecord
{
	float t;
	vec3 position;
	vec3 normal;
};

struct World
{
	int objectCount;
	Sphere objects[10];
};


Sphere SphereConstructor(vec3 center, float radius) {
    Sphere sphere;
    sphere.center = center;
    sphere.radius = radius;
    return sphere;
}

World WorldConstructor() {
	World world;

	world.objectCount = 2;
	world.objects[0] = SphereConstructor(vec3(0.0, 0.0, -1.0), 0.5);
	world.objects[1] = SphereConstructor(vec3(0.0, -100.5, -1.0), 100.0);

	return world;
}


bool SphereHit(Sphere sphere, Ray ray, float t_min, float t_max, inout HitRecord hitRecord) {
    vec3 oc = ray.origin - sphere.center;
	
	float a = dot(ray.direction, ray.direction);
	float b = 2.0 * dot(oc, ray.direction);
	float c = dot(oc, oc) - sphere.radius * sphere.radius;

	float discriminant = b * b - 4 * a * c;
    
    if(discriminant > 0.0) {
		float temp = (-b - sqrt(discriminant)) / (a);
        if (temp < t_max && temp > t_min) {
            hitRecord.t = temp;
            hitRecord.position = RayAt(ray, temp);
            hitRecord.normal = (hitRecord.position - sphere.center) / sphere.radius;
            return true;
        }

        temp = (-b + sqrt(discriminant)) / (a);
        if (temp < t_max && temp > t_min) {
            hitRecord.t = temp;
            hitRecord.position = RayAt(ray, temp);
            hitRecord.normal = (hitRecord.position - sphere.center) / sphere.radius;
            return true;
        }
	}
    return false;
}


// ================================================

bool WorldHit(World world, Ray ray, float t_min, float t_max, inout HitRecord records) {
    HitRecord tempRecord;
	float cloestSoFar = t_max;
	bool hitSomething = false;

    for (int i = 0; i < world.objectCount; i++) {
        if (SphereHit(world.objects[i], ray, t_min, cloestSoFar, tempRecord)) {
            records = tempRecord;
            cloestSoFar = tempRecord.t;
            hitSomething = true;
        }
    }
    return hitSomething;
}


vec3 WorldTrace(World world, Ray ray) {
    HitRecord hitRecord;

    if(WorldHit(world, ray, 0.0, 1000000.0, hitRecord)) {
        return 0.5 * vec3(hitRecord.normal.x+1, hitRecord.normal.y+1, hitRecord.normal.z+1);
    }
    else {
        vec3 unit_direction = normalize(ray.direction);
        float t = 0.5 * (unit_direction.y + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}


void main() {
	float u = screenCoord.x;
    float v = screenCoord.y;

    World world = WorldConstructor();
    Camera camera = CameraConstructor(
                        vec3(-2.0, -1.5, -1.0),
                        vec3(4.0, 0.0, 0.0),
                        vec3(0.0, 3.0, 0.0),
                        vec3(0.0, 0.0, 0.0));
	// Ray ray = RayConstructor(
    //             camera.origin,
    //             camera.lower_left_corner + screenCoord.x * camera.horizontal + screenCoord.y * camera.vertical);

    // ray.origin = camera.origin;
    // ray.direction = camera.lower_left_corner +
    //                 u * camera.horizontal +
    //                 v * camera.vertical -
    //                 camera.origin;

    int sampleCount = 1000;
	vec3 col = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < sampleCount; i++) {
        Ray ray = CameraGetRay(camera, screenCoord);
        col += WorldTrace(world, ray);
    }
    col /= sampleCount;
    FragColor.xyz = col;
    FragColor.w = 1.0;
}
