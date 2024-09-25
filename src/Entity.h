#pragma once

#include "Components.h"
#include <memory>

class Entity
{
    // constructor
    Entity() {}
    Entity(const size_t id, const std::string& tag) : m_id(id), m_tag(tag) {}

    bool              m_active = true;
    const size_t      m_id     = 0;
    const std::string m_tag    = "Default";
    

public:

    friend class EntityManager;

    // component pointer
    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CShape>     cShape;
    std::shared_ptr<CCollision> cCollision;
    std::shared_ptr<CInput>     cInput;
    std::shared_ptr<CScore>     cScore;
    std::shared_ptr<CLifeSpan>  cLifeSpan;
    
    // private member access functioms
    const size_t             id() const { return m_id;      }
    const std::string&      tag() const { return m_tag;     }
    bool               isActive() const { return m_active;  }
    void              setActive()       { m_active = true;  }
    void                destroy()       { m_active = false; }

    bool operator == (Entity cmp) { return m_id == cmp.m_id; }
    bool operator != (Entity cmp) { return !(*this == cmp); }
};
