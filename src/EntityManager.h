#pragma once

#include "Entity.h"
#include <algorithm>
#include <vector>
#include <map>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map   <std::string, EntityVec>  EntityMap;

class EntityManager
{
    EntityVec m_entities;
    EntityVec m_entitiesToAdd;
    EntityMap m_entityMap;
    size_t    m_totalEntities = 0;

    // helper function to avoid repeated code
    void removeDeadEntities(EntityVec& vec);

public:

    EntityManager();
    void update(); // called at beginning of each frame by the game engine
    std::shared_ptr<Entity> addEntity(const std::string& tag);
    const EntityVec& getEntities();
    const EntityVec& getEntities(const std::string& tag);
    const EntityMap& getEntityMap();
};
