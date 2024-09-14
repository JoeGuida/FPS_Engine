#include "minimap.hpp"

struct RectData {
    glm::ivec2 position;
    glm::ivec2 size;
    char key;

    RectData(const glm::ivec2& position, const glm::ivec2& size, char key) :
        position(position), size(size), key(key) {}
    RectData(const RectData&) = default;
    RectData(RectData&&) = default;
    virtual ~RectData() = default;
};

std::vector<Line> MiniMap::get_lines() {
    std::vector<Rectangle> rectangles = get_rectangles();
    std::vector<Line> lines;
    lines.reserve(rectangles.size() * 4);

    for (const Rectangle& rectangle : rectangles) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, rectangle.transform.position);
        std::array<float, 12> v = rectangle.get_vertices();

        glm::vec3 v0 = model * glm::vec4(v[0], v[1], v[2], 1.0f);
        glm::vec3 v1 = model * glm::vec4(v[3], v[4], v[5], 1.0f);
        glm::vec3 v2 = model * glm::vec4(v[6], v[7], v[8], 1.0f);
        glm::vec3 v3 = model * glm::vec4(v[9], v[10], v[11], 1.0f);

        lines.push_back(Line(glm::vec3(v0.x, v0.y, v0.z), glm::vec3(v1.x, v1.y, v1.z)));
        lines.push_back(Line(glm::vec3(v1.x, v1.y, v1.z), glm::vec3(v3.x, v3.y, v3.z)));
        lines.push_back(Line(glm::vec3(v3.x, v3.y, v3.z), glm::vec3(v2.x, v2.y, v2.z)));
        lines.push_back(Line(glm::vec3(v0.x, v0.y, v0.z), glm::vec3(v2.x, v2.y, v2.z)));
    }
    return lines;
}

std::vector<Rectangle> MiniMap::get_rectangles() {
    return optimize_rectangles();
}

std::vector<Rectangle> MiniMap::optimize_rectangles() {
    std::vector<Rectangle> rectangles;
    std::vector<RectData> rect_data_horizontal;
    std::vector<RectData> rect_data_final;
    float size = width / map.width;

    // Fill rect_data with horizontally optimized rectangles
    for (int row = 0; row < map.data.size(); row++) {
        for (int col = 0; col < map.data[0].size(); col++) {
            if (map.data[row][col] == ' ') {
                continue;
            }

            int width = 1;
            int height = 1;
            char key = map.data[row][col];

            for (int i = 1; i < map.data[0].size() - col; i++) {
                if (map.data[row][col + i] != key) {
                    break;
                }
                width++;
            }

            RectData data(glm::ivec2(col, row), glm::ivec2(width, height), key);
            rect_data_horizontal.push_back(data);
            col += width - 1;
        }
    }

    // Combine rect_data horizontally
    for (int i = 0; i < rect_data_horizontal.size(); i++) {
        if (rect_data_final.size() == 0) {
            rect_data_final.push_back(rect_data_horizontal[i]);
            continue;
        }

        bool has_height_changed = false;
        for (int j = 0; j < rect_data_final.size(); j++) {
            int diff = std::abs(rect_data_horizontal[i].position.y - rect_data_final[j].position.y);
            int xy = 0;
            if (rect_data_horizontal[i].position.x == rect_data_final[j].position.x &&
                rect_data_horizontal[i].key == rect_data_final[j].key &&
                rect_data_horizontal[i].size.x == rect_data_final[j].size.x &&
                diff <= rect_data_final[j].size.y) {

                rect_data_final[j].size.y++;
                has_height_changed = true;
            }
        }

        if (!has_height_changed) {
            rect_data_final.push_back(rect_data_horizontal[i]);
        }
    }

    for (const RectData& data : rect_data_final) {
        float x_pos = bounds.x + (size * data.position.x) + (size * data.size.x) / 2.0f;
        float y_pos = bounds.w - (size * data.position.y) - (size * data.size.y) / 2.0f;
        Rectangle rectangle(size * data.size.x,
            size * data.size.y,
            Material(colormap[data.key - '0']),
            Transform(glm::vec3(x_pos, y_pos, 0.0f)));
        rectangles.push_back(rectangle);
    }

    return rectangles;
}