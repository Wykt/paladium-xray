#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <vector>

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

class whitelisted_block
{
public:
	int block_id;
	vec3f color;

	whitelisted_block(int block_id, vec3f color) : color(color)
	{
		this->block_id = block_id;
		this->color = color;
	}
};

namespace blocks
{
	void initialize();
	bool is_block_whitelisted(int block_id);
	vec3f* get_block_color(int block_id);
	std::vector<whitelisted_block>* get_whitelisted_blocks();
}