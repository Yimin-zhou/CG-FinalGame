#include "environment.h"

Environment::~Environment()
{

}

void Environment::Render()
{
	for (auto m : models)
	{
		m->Render();
	}
}

void Environment::AddModel(std::shared_ptr<Model> model)
{
	models.push_back(model);
}

Environment::Environment() :
	models()
{
	for (uint32_t i = 0; i < m_numModels; i++)
	{
		models.push_back(std::make_shared<Model>());
	}
}

Environment::Environment(std::vector<std::shared_ptr<Model>> ms) :
	models(ms)
{
	m_numModels = models.size();
}

