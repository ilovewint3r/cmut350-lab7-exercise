#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <cstddef>  // size_t
#include <type_traits>
#include <utility>
#include <limits>
#include <array>
#include <vector>
#include <tuple>
#include "pack.h"  // Pack from prep
#include "registry.h"

using EntityID = size_t;
template <typename... ComponentTypes>
class EntityManager {
public:
    EntityID createEntity() {
        EntityID id;
        if (!freeSlots.empty()) {
            id = freeSlots.back();
            freeSlots.pop_back();
            isAlive[id] = true;
            for (auto& table : components) {
                table[id] = invalid;
            }
        }
        else {
            id = isAlive.size();
            isAlive.push_back(true);
            for (auto& table : components) {
                table.push_back(invalid);
            }
        }
        return id;
    }

    bool validEntity(EntityID id) const {
        return id < isAlive.size() && isAlive[id];
    }

    template <typename ComponentType>
    bool hasComponent(EntityID id) const {
        static_assert(Pack<ComponentTypes...>::template contains<ComponentType>);
        assert(validEntity(id));
        constexpr size_t idx = Pack<ComponentTypes...>::template index<ComponentType>;
        return components[idx][id] != invalid;
    }

    template <typename ComponentType>
    ComponentType& getComponent(EntityID id) {
        static_assert(Pack<ComponentTypes...>::template contains<ComponentType>);
        assert(hasComponent<ComponentType>(id));
        constexpr size_t idx = Pack<ComponentTypes...>::template index<ComponentType>;
        return std::get<idx>(registries)[components[idx][id]];
    }

    template <typename ComponentType>
    void addComponent(EntityID id, ComponentType&& component) {
        using T = std::remove_reference_t<ComponentType>;
        static_assert(Pack<ComponentTypes...>::template contains<T>);
        assert(validEntity(id));
        assert(!hasComponent<T>(id)); // cant have 2 of the same type
        constexpr size_t idx = Pack<ComponentTypes...>::template index<T>;
        components[idx][id] = std::get<idx>(registries).store(std::forward<ComponentType>(component));
    }


private:
    static constexpr size_t invalid = std::numeric_limits<size_t>::max();
    std::tuple<Registry<ComponentTypes>...> registries;
    std::array<std::vector<size_t>, sizeof...(ComponentTypes)> components;
    std::vector<bool> isAlive;
    std::vector<EntityID> freeSlots;
};

#endif  // ENTITY_MANAGER_H
