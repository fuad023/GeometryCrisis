#include "EntityManager.h"


EntityManager::EntityManager() {}


void EntityManager::removeDeadEntities(EntityVec& vec) // or you could create another vector of m_alive = true and erase the original vector, this would also solve the iterator invalidation
{
    // use std::remove_if to remove dead entities
    // probably not the fastest solution, but it is safe                      // assuming here it would be best to pass as reference
    vec.erase( std::remove_if( vec.begin(), vec.end(), [](std::shared_ptr<Entity> entity) { return !entity->isActive(); } ), vec.end() );
}


void EntityManager::update()
{
    for (auto e : m_entitiesToAdd) // we did not take the reference of the entity pointer
    {                              // because later the elements in this vector wiil be deleted
        m_entities.push_back(e);
        m_entityMap[e->tag()].push_back(e);
    }
    m_entitiesToAdd.clear();

    // clean up dead entities in all vectors
    removeDeadEntities(m_entities);
    for (auto& kv : m_entityMap) {
        removeDeadEntities(kv.second);
    }
}


std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag) 
{
    auto entity = std::shared_ptr<Entity> ( new Entity( m_totalEntities++, tag ) );
    m_entitiesToAdd.push_back(entity);
    return entity;
}


const EntityVec& EntityManager::getEntities() {
    return m_entities; 
}


const EntityVec& EntityManager::getEntities(const std::string& tag) { 
    // return the vector in the map where all the entities with the same tag live
    return m_entityMap[tag]; 
} 


const EntityMap& EntityManager::getEntityMap() {
    return m_entityMap;
} 
