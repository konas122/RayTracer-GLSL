#version 330 core
#define PI 3.14159265
#define RAYCAST_MAX 100000.0

#define MAT_LAMBERTIAN  0
#define MAT_METALLIC    1
#define MAT_DIELECTRIC  2
#define MAT_PBR			3

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
	v = uint(36969) * (v & uint(65535)) + (v >> 16);
	u = uint(18000) * (u & uint(65535)) + (u >> 16);
	return (v << 16) + u;
}

float GetUniformCore(inout uint u, inout uint v) {
	uint z = GetUintCore(u, v);
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
    int materialType;
	int material;
};

struct HitRecord {
	float t;
	vec3 position;
	vec3 normal;

    int materialType;
	int material;
};

struct World {
	int objectCount;
	Sphere objects[10];
};


Sphere SphereConstructor(vec3 center, float radius, int materialType, int material) {
    Sphere sphere;
    sphere.center = center;
    sphere.radius = radius;
    sphere.materialType = materialType;
	sphere.material = material;
    return sphere;
}


World WorldConstructor() {
	World world;

	world.objectCount = 3;
	world.objects[0] = SphereConstructor(vec3( 0.0,  0.0, -3.0), 0.50, MAT_LAMBERTIAN, 0);
	world.objects[1] = SphereConstructor(vec3( 0.7, -0.3, -1.0), 0.20, MAT_METALLIC,   1);
	world.objects[2] = SphereConstructor(vec3(-0.7,  0.3, -2.0), 0.40, MAT_DIELECTRIC, 2);

	return world;
}


bool SphereHit(Sphere sphere, Ray ray, float t_min, float t_max, inout HitRecord hitRecord) {
    vec3 oc = ray.origin - sphere.center;
	
	float a = dot(ray.direction, ray.direction);
	float b = 2.0 * dot(oc, ray.direction);
	float c = dot(oc, oc) - sphere.radius * sphere.radius;

	float discriminant = b * b - 4 * a * c;
    
    if (discriminant > 0.0) {
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

struct Lambertian {
    vec3 albedo;
};

Lambertian LambertianConstructor(vec3 albedo) {
	Lambertian lambertian;
	lambertian.albedo = albedo;
	return lambertian;
}

bool LambertianScatter(in Lambertian lambertian, in Ray incident, in HitRecord hitRecord, out Ray scattered, out vec3 attenuation) {
    attenuation = lambertian.albedo;
    scattered.origin = hitRecord.position;
	scattered.direction = hitRecord.normal + random_in_unit_sphere();
	return true;
}


struct Metallic {
	vec3 albedo;
	float roughness;
};

Metallic MetallicConstructor(vec3 albedo, float roughness) {
	Metallic metallic;

	metallic.albedo = albedo;
	metallic.roughness = roughness;

	return metallic;
}


float schlick(float cosine, float ior) {
	float r0 = (1 - ior) / (1 + ior);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}


vec3 reflect(in vec3 incident, in vec3 normal) {
	return incident - 2 * dot(normal, incident) * normal;
}


bool refract(vec3 v, vec3 n, float ni_over_nt, out vec3 refracted) {
	vec3 uv = normalize(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.0 - dt * dt);
	if (discriminant > 0)
	{
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
		return false;
}

bool MetallicScatter(in Metallic metallic, in Ray incident, in HitRecord hitRecord, out Ray scattered, out vec3 attenuation) {
	attenuation = metallic.albedo;

	scattered.origin = hitRecord.position;
	scattered.direction = reflect(incident.direction, hitRecord.normal);

	return dot(scattered.direction, hitRecord.normal) > 0.0;
}


struct Dielectric {
	vec3 albedo;
	float roughness;
	float ior;
};

Dielectric DielectricConstructor(vec3 albedo, float roughness, float ior) {
	Dielectric dielectric;

	dielectric.albedo = albedo;
	dielectric.roughness = roughness;
	dielectric.ior = ior;

	return dielectric;
}

bool DielectricScatter(in Dielectric dielectric, in Ray incident, in HitRecord hitRecord, out Ray scattered, out vec3 attenuation) {
	attenuation = dielectric.albedo;
	vec3 reflected = reflect(incident.direction, hitRecord.normal);

	vec3 outward_normal;
	float ni_over_nt;
	float cosine;
	if (dot(incident.direction, hitRecord.normal) > 0.0) {  // hit from inside
		outward_normal = -hitRecord.normal;
		ni_over_nt = dielectric.ior;
		cosine = dot(incident.direction, hitRecord.normal) / length(incident.direction); // incident angle
	}
	else {  // hit from outside
		outward_normal = hitRecord.normal;
		ni_over_nt = 1.0 / dielectric.ior;
		cosine = -dot(incident.direction, hitRecord.normal) / length(incident.direction); // incident angle
	}

	float reflect_prob;
	vec3 refracted;
	if (refract(incident.direction, outward_normal, ni_over_nt, refracted)) {
		reflect_prob = schlick(cosine, dielectric.ior);
	}
	else {
		reflect_prob = 1.0;
	}

	if (rand() < reflect_prob) {
		scattered = Ray(hitRecord.position, refracted);
	}
	else {
		scattered = Ray(hitRecord.position, refracted);
	}
	return true;
}


// ================================================

World world;
Camera camera;
Lambertian lambert[4];
Metallic metallic[4];
Dielectric dielectric[4];


void InitScene() {
	world = WorldConstructor();
	camera = CameraConstructor(vec3(-2.0, -1.0, -1.0), vec3(4.0, 0.0, 0.0), vec3(0.0, 2.0, 0.0), vec3(0.0, 0.0, 0.0));

	lambert[0] = LambertianConstructor(vec3(0.7, 0.5, 0.5));
	lambert[1] = LambertianConstructor(vec3(0.5, 0.7, 0.5));
	lambert[2] = LambertianConstructor(vec3(0.5, 0.5, 0.7));

	// metallic[0] = MetallicConstructor(vec3(0.7, 0.5, 0.5), 0.0);
	// metallic[1] = MetallicConstructor(vec3(0.5, 0.7, 0.5), 0.1);
	// metallic[2] = MetallicConstructor(vec3(0.5, 0.5, 0.7), 0.2);

	// dielectric[0] = DielectricConstructor(vec3(1.0, 1.0, 1.0), 0.0, 1.5);
	// dielectric[1] = DielectricConstructor(vec3(1.0, 1.0, 1.0), 0.1, 1.5);
	// dielectric[2] = DielectricConstructor(vec3(1.0, 1.0, 1.0), 0.2, 1.5);
}


bool MaterialScatter(in int materialType, in int material, in Ray incident, in HitRecord hitRecord, out Ray scatter, out vec3 attenuation) {
	if (materialType == MAT_LAMBERTIAN)
		return LambertianScatter(lambert[material], incident, hitRecord, scatter, attenuation);
	else if (materialType == MAT_METALLIC)
		return MetallicScatter(metallic[material], incident, hitRecord, scatter, attenuation);
	else if (materialType == MAT_DIELECTRIC)
		return DielectricScatter(dielectric[material], incident, hitRecord, scatter, attenuation);
	else
		return false;
}


// ================================================

vec3 GetEnvironmentColor(Ray ray) {
    vec3 dir = normalize(ray.direction);
	float theta = acos(dir.y) / PI;
	float phi = (atan(dir.x, dir.z) / (PI) + 1.0) / 2.0;
	return texture(envMap, vec2(phi, theta)).xyz;
}


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


vec3 WorldTrace(World world, Ray ray, int depth) {
    HitRecord hitRecord;

    vec3 frac = vec3(1.0, 1.0, 1.0);
	vec3 bgColor = vec3(0.0, 0.0, 0.0);

    while (depth > 0) {
        --depth;
        if (WorldHit(world, ray, 0.001, RAYCAST_MAX, hitRecord)) {
            Ray scatter;
            vec3 attenuation;
            if (!MaterialScatter(hitRecord.materialType, hitRecord.material, ray, hitRecord, scatter, attenuation)) {
                break;
            }

            frac *= attenuation;
            ray = scatter;
        }
        else {
            bgColor = GetEnvironmentColor(ray);
            break;
        }
    }
    return bgColor * frac;
}


// ================================================

vec3 GammaCorrection(vec3 c) {
	return pow(c, vec3(1.0 / 2.2));
}


vec3 InverseGammaCorrection(vec3 c) {
	return pow(c, vec3(2.2));
}


void main() {
    InitScene();
    Camera camera = CameraConstructor(
                        vec3(-2.0, -1.5, -1.0),
                        vec3(4.0, 0.0, 0.0),
                        vec3(0.0, 3.0, 0.0),
                        vec3(0.0, 0.0, 0.0));

    int sampleCount = 1000;
	vec3 col = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < sampleCount; i++) {
		// Ray ray = CameraGetRay(camera, screenCoord + (rand2() / screenSize));
        Ray ray = CameraGetRay(camera, screenCoord);
        col += WorldTrace(world, ray, 20);
    }
    col /= sampleCount;
    // col = GammaCorrection(col);
    FragColor.xyz = col;
    FragColor.w = 1.0;
}
