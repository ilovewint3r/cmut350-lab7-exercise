#ifndef REGISTRY_H
#define REGISTRY_H

#include <cstddef>
#include <vector>
#include <cassert>
#include <utility>

template <typename T>
class Registry {
public:
    template <typename U>
    size_t store(U&& value) {
        size_t idx;
        if (!freeSlots.empty()) {
            idx = freeSlots.back();
            freeSlots.pop_back();
            components[idx] = std::forward<U>(value);
            isAlive[idx] = true;
        }
        else {
            idx = components.size();
            components.push_back(std::forward<U>(value));
            isAlive.push_back(true);
        }
        return idx;
    }

    void remove(size_t idx) {
        assert(valid(idx));
        components[idx] = T{};
        isAlive[idx] = false;
        freeSlots.push_back(idx);
    }

    T& operator[](size_t idx) {
        assert(valid(idx));
        return components[idx];
    }

    const T& operator[](size_t idx) const {
        assert(valid(idx));
        return components[idx];
    }

    bool valid(size_t idx) const {
        return idx < components.size() - freeSlots.size() && isAlive[idx];
    }

    void reserve(size_t capacity) {
        components.reserve(capacity);
        isAlive.reserve(capacity);
    }

private:
    std::vector<T> components;
    std::vector<bool> isAlive;
    std::vector<size_t> freeSlots;
};

#endif  // REGISTRY_H
