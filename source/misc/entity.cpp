#include "entity.h"

Entity::Entity()
{

}

void Entity::Render(QMatrix4x4 proj) {

    proj = proj*getMV();

    for (auto c:m_children) {
        c->Render(proj);
    }
}
