#ifndef FPS_ENGINE_MAP_HPP
#define FPS_ENGINE_MAP_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Map {
private:
    void load(const std::string& path);

public:
    Map(const std::string& path) { load(path); }
    Map(const Map&) = default;
    Map(Map&&) = default;
    virtual ~Map() {}

    int width;
    int height;
    std::vector<std::string> data;
};

#endif