#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <assert.h>
#include <iostream>
#include <vector>

#include "camera.hpp"
#include "color.hpp"
#include "filesystem.hpp"
#include "line.hpp"
#include "map.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "raycast.hpp"
#include "rectangle.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "transform.hpp"

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;
constexpr double ASPECT_RATIO = static_cast<double>(SCREEN_WIDTH) / SCREEN_HEIGHT;
constexpr double X_MIN = -ASPECT_RATIO;
constexpr double X_MAX = ASPECT_RATIO;
constexpr double Y_MIN = -1.0;
constexpr double Y_MAX = 1.0;

constexpr glm::vec3 world_forward(0.0f, 0.0f, -1.0f);
constexpr glm::vec3 world_right(1.0f, 0.0f, 0.0f);
constexpr glm::vec3 world_up(0.0f, 1.0f, 0.0f);

bool wireframe = false;

Camera camera(glm::vec3(0.0f, 0.0f, -1.0f), 
              world_up, 
              glm::radians(90.0f), 
              Transform(glm::vec3(0.0f, 0.0f, 3.0f)));
float CAMERA_VELOCITY = 0.01f;

constexpr glm::vec3 COLORS[6]{
    { 0.231f, 0.254f, 0.321f},
    { 0.309f, 0.352f, 0.392f},
    { 0.466f, 0.533f, 0.549f},
    { 0.082f, 0.054f, 0.062f},
    { 0.152f, 0.152f, 0.223f},
    { 0.223f, 0.219f, 0.286f}
};

GLFWwindow* initialize_window();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
std::vector<Line> get_lines(const std::vector<Rectangle>& rectangles);
std::vector<Rectangle> get_rectangles(const Map& map);

int main() {
    GLFWwindow* window = initialize_window();
    Renderer::initialize();

    // -----------------------------------------------------------------

    Shader default_shader(FileSystem::get_path("/src/shaders/default.vert"), 
                          FileSystem::get_path("/src/shaders/default.frag"));

    // Map
    Map map(FileSystem::get_path("/data/maps/map1.txt"));
    assert(map.width * map.height == map.data[0].size() * map.data.size());
    std::vector<Rectangle> rectangles = get_rectangles(map);
    std::vector<Line> lines = get_lines(rectangles);
    std::cout << "Rectangles: " << rectangles.size() 
              << " Lines: " << lines.size() << std::endl;

    // Player
    Rectangle player(0.01f, 
                     Material(Color::Blue), 
                     Transform(glm::vec3(-0.3f, 0.5f, 0.0f)));
    Camera player_camera(glm::vec3(0.0f, -1.0f, 0.0f),
                         world_up,
                         glm::radians(90.0f),
                         Transform(player.transform.position));
    Ray player_direction(player_camera.transform.position, 
                         player_camera.front, 
                         Material(Color::Red));

    Ray* player_fov = new Ray[SCREEN_WIDTH];
    float rotation_amount = player_camera.fov_radians / SCREEN_WIDTH;
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        player_fov[i].origin = player_camera.transform.position;
        
        glm::vec3 front = glm::rotate(player_camera.front, 
                                      -player_camera.fov_radians / 2.0f, 
                                      glm::vec3(0.0f, 0.0f, 1.0f));
        player_fov[i].direction = glm::rotate(front, 
                                              rotation_amount * i, 
                                              glm::vec3(0.0f, 0.0f, 1.0f));
        player_fov[i].material.color = Color::White;
    }

    // -----------------------------------------------------------------

    glm::mat4 view;
    glm::mat4 projection = glm::ortho(-ASPECT_RATIO, 
                                      ASPECT_RATIO, 
                                      -1.0, 
                                      1.0, 
                                      0.01, 
                                      100.0);
    default_shader.set_uniform("projection", projection);
    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = glm::lookAt(
            camera.transform.position, 
            camera.transform.position + camera.front, world_up);
        default_shader.set_uniform("view", view);

        Renderer::draw(player, default_shader);
        Renderer::draw(player_direction, default_shader);

        for (int i = 0; i < SCREEN_WIDTH; i++) {
            float distance = RAY_MAX_DISTANCE;
            for (const Line& line : lines) {
                float d = Raycast::raycast_line(player_fov[i], line);
                if (d < distance && d > 0.0f) { distance = d; }
            }
            Renderer::draw(player_fov[i], default_shader);
        }

        for (const Rectangle& rectangle : rectangles) {
            Renderer::draw(rectangle, default_shader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

std::vector<Line> get_lines(const std::vector<Rectangle>& rectangles) {
    std::vector<Line> lines;
    lines.reserve(rectangles.size() * 4);
    for (const Rectangle& rectangle : rectangles) {
        std::array<float, 12> v = rectangle.get_vertices();
        lines.push_back(Line(glm::vec3(v[0], v[1], v[2]), glm::vec3(v[3], v[4], v[5])));
        lines.push_back(Line(glm::vec3(v[3], v[4], v[5]), glm::vec3(v[9], v[10], v[11])));
        lines.push_back(Line(glm::vec3(v[9], v[10], v[11]), glm::vec3(v[6], v[7], v[8])));
        lines.push_back(Line(glm::vec3(v[0], v[1], v[2]), glm::vec3(v[6], v[7], v[8])));
    }
    return lines;
}

std::vector<Rectangle> get_rectangles(const Map& map) {
    std::vector<Rectangle> rectangles;
    for (int y = 0; y < map.data.size(); y++) {
        for (int x = 0; x < map.data[0].size(); x++) {
            int index = map.data[y][x] - '0';
            float size = (X_MAX - X_MIN) / map.width;
            Material material(COLORS[index]);
            Rectangle rectangle(
                size,
                material,
                Transform(glm::vec3(X_MIN + size / 2.0f + size * x, 
                                    Y_MAX - size / 2.0f - size * y, 
                                    0.0f)));
            rectangles.push_back(rectangle);
        }
    }

    return rectangles;
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        wireframe = !wireframe;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.transform.position += glm::vec3(1.0f, 0.0f, 0.0f) * CAMERA_VELOCITY;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.transform.position += -glm::vec3(1.0f, 0.0f, 0.0f) * CAMERA_VELOCITY;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.transform.position += glm::vec3(0.0f, 1.0f, 0.0f) * CAMERA_VELOCITY;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.transform.position += -glm::vec3(0.0f, 1.0f, 0.0f) * CAMERA_VELOCITY;
    }

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

GLFWwindow* initialize_window() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, 
                                          SCREEN_HEIGHT, 
                                          "FPS Engine", 
                                          NULL, 
                                          NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glEnable(GL_DEPTH_TEST);

    return window;
}