#include "map.hpp"

void Map::load(const std::string& path) {
	std::ifstream file(path);

	std::string line;
	bool found_width = false;
	bool found_height = false;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		if (!found_width) {
			found_width = true;
			width = std::stoi(line);
		}
		else if (found_width && !found_height) {
			found_height = true;
			height = std::stoi(line);
		}
		else {
			data.push_back(line);
		}
	}
}