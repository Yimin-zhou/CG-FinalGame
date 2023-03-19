#include "level.h"

Level::~Level()
{

}

void Level::Render()
{
	for (auto& e : m_environments)
	{
		e->Render();
	}
}

Level::Level() :
	m_environments()
{
	for (uint32_t i = 0; i < m_numEnvirons; i++)
	{
		m_environments.push_back(std::make_shared<Environment>());
	}
}

Level::Level(std::vector<std::shared_ptr<Environment>> environments) :
	m_environments(environments)
{
	m_numEnvirons = environments.size();
}
