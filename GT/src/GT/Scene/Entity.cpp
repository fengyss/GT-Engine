#include "gtpch.h"
#include "Entity.h"

namespace GT
{
	Entity::Entity(entt::entity entity, Scene* scene)
		:m_EntityHandle(entity),m_Scene(scene)
	{
	}
}

