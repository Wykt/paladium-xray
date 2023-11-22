#pragma once

#include <Windows.h>
#include <gl/GL.h>

class vec3f
{
public:
	float r, g, b;

	vec3f(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
};

namespace blocks
{
	void initialize();
	bool is_block_whitelisted(int block_id);
	vec3f* get_block_color(int block_id);
}