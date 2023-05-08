#pragma once

#include <map>
#include <vector>

template <typename T>
class TimedDispatcher {
public:
    void setDispatchDelay(double delay) { this->delay = delay; }
    void add(double time, T element) { this->elements.insert({ time, element }); }
    
    std::vector<T> get(double time) {
        std::vector<T> result;
        for (auto it = elements.begin(); it != elements.end();) {
            if (it->first < time - delay) {
                result.push_back(it->second);
                it = elements.erase(it);
            }
            else {
                ++it;
            }
        }
        return result;
    }

    bool isEmpty() {
        return this->elements.empty();
    }

    int size() {
        return this->elements.size();
    }

private:
    double delay = 0;
    std::multimap<double, T> elements;
};