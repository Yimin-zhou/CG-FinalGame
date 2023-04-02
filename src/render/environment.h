#pragma once

#include "model.h"

#include <memory>
#include <vector>

class Environment
{
public:
	std::vector<std::shared_ptr<Model>> models;

	void AddModel(std::shared_ptr<Model> model);

	Environment(std::vector<std::shared_ptr<Model>> models);
	Environment();
	~Environment();

private:
	uint32_t m_numModels = 1;
};