#include <iostream>

#include "entity_manager.h"
#include <cassert>

int main() {
    // test here...
    EntityManager<int, float> manager;
    auto entity = manager.createEntity();
    assert(!manager.hasComponent<int>(entity));
    assert(!manager.hasComponent<float>(entity));

    manager.addComponent(entity, 35.2f);
    assert(manager.hasComponent<float>(entity));
    assert(manager.getComponent<float>(entity) == 35.2f);
    manager.addComponent(entity, 123);
    assert(manager.hasComponent<int>(entity));
    assert(manager.getComponent<int>(entity) == 123);

    int x = 90;
    auto entity2 = manager.createEntity();
    manager.addComponent(entity2, x);
    manager.addComponent(entity2, 22.0f);
    assert(manager.getComponent<int>(entity2) == 90);
    assert(manager.getComponent<float>(entity2) == 22.0f);
    return 0;
}
