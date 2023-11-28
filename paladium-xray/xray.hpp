#pragma once

#include <wtypes.h>
#include <Windows.h>
#include <gl/GL.h>
#include <vector>

namespace xray {

	void render();
	void render_gui();
	void initialize(HMODULE handle);

	extern bool gui_open;
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

class block
{
public:
	vec3i position;
	int block_id;

	block(vec3i position, int block_id) : position(position)
	{
		this->position = position;
		this->block_id = block_id;
	}
};

struct chunk {
	int x, z;
	std::vector<block>* blocks;
};