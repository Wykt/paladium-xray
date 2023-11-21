#pragma once

#include <wtypes.h>
#include <Windows.h>
#include <gl/GL.h>
#include <vector>

namespace xray {

	void render();
	void initialize(HMODULE handle);
}

extern GLfloat projection[16];
extern GLfloat modelview[16];

class vec3i {
public:
	int x, y, z;

	vec3i(int x, int y, int z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class vec3d {
public:
	double x, y, z;

	vec3d(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct chunk {
	int x, z;
	std::vector<vec3i> blocks;
};