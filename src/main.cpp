#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/rotate_vector.hpp>
#include<glm/vec2.hpp>

#include <assert.h>
#include <iostream>
#include <vector>

#include "camera.hpp"
#include "color.hpp"
#include "filesystem.hpp"
#include "line.hpp"
#include "map.hpp"
#include "minimap.hpp"
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
constexpr float CAMERA_VELOCITY = 0.01f;

const std::vector<Color> COLORS {
    Color(0.231f, 0.254f, 0.321f),
    Color(0.309f, 0.352f, 0.392f),
    Color(0.466f, 0.533f, 0.549f),
    Color(0.082f, 0.054f, 0.062f),
    Color(0.152f, 0.152f, 0.223f),
    Color(0.223f, 0.219f, 0.286f)
};

Rectangle player(0.01f,
    Material(Color::Blue),
    Transform(glm::vec3(-1.5f, 0.85f, 0.0f)));

Camera player_camera(glm::vec3(0.0f, -1.0f, 0.0f),
    world_up,
    glm::radians(90.0f),
    Transform(player.transform.position));

GLFWwindow* initialize_window();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void update_rays(const Camera& camera, int screen_width, Ray* player_fov);

int main() {
    GLFWwindow* window = initialize_window();
    Renderer::initialize();

    // -----------------------------------------------------------------

    Shader default_shader(FileSystem::get_path("/src/shaders/default.vert"), 
                          FileSystem::get_path("/src/shaders/default.frag"));

    // Map
    Map map(FileSystem::get_path("/data/maps/map1.txt"));
    assert(map.width * map.height == map.data[0].size() * map.data.size());
    
    // MiniMap
    MiniMap mini_map(map, glm::vec4(X_MIN, X_MAX, Y_MIN, Y_MAX), ASPECT_RATIO, COLORS);
    mini_map.optimize_rectangles();

    std::vector<Rectangle> rectangles = mini_map.get_rectangles();
    std::vector<Line> lines = mini_map.get_lines();
    std::cout << "Rectangles: " << rectangles.size() << " Lines: " << lines.size() << std::endl;


    // Player
    Ray player_direction(player_camera.transform.position, 
                         player_camera.front, 
                         Material(Color::Red),
                         mini_map.height * 0.05f);

    Ray* player_fov = new Ray[SCREEN_WIDTH];
    update_rays(player_camera, SCREEN_WIDTH, player_fov);

    // -----------------------------------------------------------------

    glm::mat4 view;
    glm::mat4 projection = glm::ortho(-ASPECT_RATIO, ASPECT_RATIO, -1.0, 1.0, 0.01, 100.0);
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
        player_direction.origin = player_camera.transform.position;
        Renderer::draw(player_direction, default_shader);
        update_rays(player_camera, SCREEN_WIDTH, player_fov);
        for (int i = 0; i < SCREEN_WIDTH; i++) {
            float distance = RAY_MAX_DISTANCE;
            for (const Line& line : lines) {
                float d = Raycast::raycast_line(player_fov[i], line);
                if (d < distance && d > 0.0f) { distance = d; }
            }
            
            if (distance > 0.0f && distance < RAY_MAX_DISTANCE) {
                player_fov[i].distance = distance;
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



void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        if (wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            wireframe = !wireframe;
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            wireframe = !wireframe;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        player.transform.position += glm::vec3(1.0f, 0.0f, 0.0f) * CAMERA_VELOCITY;
        player_camera.transform.position += glm::vec3(1.0f, 0.0f, 0.0f) * CAMERA_VELOCITY;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        player.transform.position += -glm::vec3(1.0f, 0.0f, 0.0f) * CAMERA_VELOCITY;
        player_camera.transform.position += -glm::vec3(1.0f, 0.0f, 0.0f) * CAMERA_VELOCITY;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        player.transform.position -= glm::vec3(0.0f, 1.0f, 0.0f) * CAMERA_VELOCITY;
        player_camera.transform.position -= glm::vec3(0.0f, 1.0f, 0.0f) * CAMERA_VELOCITY;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        player.transform.position += glm::vec3(0.0f, 1.0f, 0.0f) * CAMERA_VELOCITY;
        player_camera.transform.position += glm::vec3(0.0f, 1.0f, 0.0f) * CAMERA_VELOCITY;
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

void update_rays(const Camera& camera, int screen_width, Ray* player_fov) {
    float rotation_amount = camera.fov_radians / screen_width;
    glm::vec3 front = glm::rotate(camera.front,
        -camera.fov_radians / 2.0f,
        glm::vec3(0.0f, 0.0f, 1.0f));
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        player_fov[i].origin = camera.transform.position;
        player_fov[i].direction = glm::rotate(front,
            rotation_amount * i,
            glm::vec3(0.0f, 0.0f, 1.0f));
        player_fov[i].material.color = Color::White;
    }
}