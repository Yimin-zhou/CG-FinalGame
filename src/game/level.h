#pragma once

#include "render/environment.h"

#include <memory>
#include <vector>

class Level
{
public:
	void Render();

	Level(std::vector<std::shared_ptr<Environment>> environments);
	Level();
	~Level();

private:
	std::vector<std::shared_ptr<Environment>> m_environments;
	uint32_t m_numEnvirons= 1;
};