#version 430 core
layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D img;

struct Ray {
	vec3 pos;
	vec3 dir;
};

struct RayCollision {
	bool hit;
	float distance;
	vec3 point;
	vec3 normal;
};

struct Camera {
	vec3 pos;
	vec3 target;
	float fov;
};

uniform float time;
uniform Camera cam;
uniform samplerCube skybox;


ivec2 size() {
    return ivec2(gl_NumWorkGroups.xy * gl_WorkGroupSize.xy);
}

Ray ray(ivec2 id) {
    const vec3 eye = cam.pos;
    const vec3 target = cam.target;
    const float fov = radians(cam.fov);
    const vec3 worldUp = vec3(0, 1, 0);

    vec3 forward = normalize(target - eye);

    vec3 right = normalize(cross(forward, worldUp));
    vec3 up = normalize(cross(right, forward));

    float width  = float(size().x);
    float height = float(size().y);
    float aspect = width / height;

    float gx = tan(fov * 0.5);
    float gy = gx / aspect;

    vec3 qx = 2.0 * gx / (width  - 1.0) * right;
    vec3 qy = 2.0 * gy / (height - 1.0) * up;

    vec3 p = forward - gx * right - gy * up;
    vec3 pxy = p + qx * float(id.x) + qy * float(id.y);

    return Ray(eye, normalize(pxy));
}

vec3 reflect(vec3 dir, vec3 normal) {
	return dir - 2 * dot(dir, normal) * normal;
}

RayCollision miss() {
	return RayCollision(false, 0, vec3(0), vec3(0));
}

RayCollision sphere(vec3 pos, float radius, Ray ray) {
	vec3 raySpherePos = pos - ray.pos;
    float vector = dot(raySpherePos, ray.dir);
    float distance = length(raySpherePos);
    float d = radius*radius - (distance*distance - vector*vector);

	RayCollision col = miss();

	// Make sure that its infront of the ray
	if (vector < 0) return col;

	col.hit = d >= 0;

	if (distance < radius) {
        col.distance = vector + sqrt(d);
        col.point = ray.pos + ray.dir * col.distance;
        col.normal = -normalize(col.point - pos);
    }
    else {
		col.distance = vector - sqrt(d);
        col.point = ray.pos + ray.dir * col.distance;
        col.normal = normalize(col.point - pos);
    }

	return col;
}

RayCollision raycast(Ray ray) {
	RayCollision col1 = sphere(vec3(0, 0, 5), 1.0, ray);
	RayCollision col2 = sphere(vec3(3, 2, 1), 2, ray);

	if (col1.hit && col2.hit) return col1.distance < col2.distance ? col1 : col2;
	else if (col1.hit) return col1;
	else if (col2.hit) return col2;
	return miss();
}

vec3 raytrace(Ray ray) {
    int bounces = 0;

    while (bounces < 5) {
        RayCollision col = raycast(ray);

        if (!col.hit)
            return texture(skybox, ray.dir).rgb;

        ray.dir = reflect(ray.dir, col.normal);
        ray.pos = col.point + col.normal * 0.001;

        bounces++;
    }

    return vec3(0.0);
}

void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec2 uv = vec2(id) / size();

	

	Ray ray = ray(id);
	vec4 col = vec4(raytrace(ray), 1);
		
    imageStore(img, id, col);
}
