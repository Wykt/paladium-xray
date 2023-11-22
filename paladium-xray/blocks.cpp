#include "blocks.hpp"
#include <vector>

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

std::vector<whitelisted_block> whitelisted_blocks;

void blocks::initialize()
{
	//whitelisted_blocks.push_back(whitelisted_block(546, vec3f(0.85F, 0.F, 1.F)));
	//whitelisted_blocks.push_back(whitelisted_block(547, vec3f(0.F, 0.F, 0.F)));
	whitelisted_blocks.push_back(whitelisted_block(548, vec3f(0.95F, 0.3F, 0.025F)));
	whitelisted_blocks.push_back(whitelisted_block(549, vec3f(0.F, 1.F, 0.F)));
	whitelisted_blocks.push_back(whitelisted_block(550, vec3f(0.95F, 0.8F, 0.05F)));
	whitelisted_blocks.push_back(whitelisted_block(551, vec3f(0.F, 1.F, 0.6F)));
}

bool blocks::is_block_whitelisted(int block_id)
{
	for (whitelisted_block& b : whitelisted_blocks)
	{
		if (b.block_id == block_id)
		{
			return true;
		}
	}

	return false;
}

vec3f* blocks::get_block_color(int block_id)
{
	for (whitelisted_block& b : whitelisted_blocks)
	{
		if (b.block_id == block_id)
		{
			return &b.color;
		}
	}

	return new vec3f(0.F,0.F,0.F);
}