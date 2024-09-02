#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <assert.h>
#include <iostream>
#include <vector>

#include "camera.hpp"
#include "color.hpp"
#include "filesystem.hpp"
#include "map.hpp"
#include "material.hpp"
#include "rectangle.hpp"
#include "renderer.hpp"
#include "shader.hpp"

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

Camera camera(glm::vec3(0.0f, 0.0f, -1.0f), world_up, Transform(glm::vec3(0.0f, 0.0f, 3.0f)));
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
std::vector<Rectangle> get_rectangles(const Map& map);

int main() {
	GLFWwindow* window = initialize_window();
	Renderer::initialize();

	// -----------------------------------------------------------------
	Shader default_shader(
		FileSystem::get_path("/src/shaders/default.vert"), 
		FileSystem::get_path("/src/shaders/default.frag"));

	Map map(FileSystem::get_path("/data/maps/map.txt"));
	assert(map.width * map.height == map.data[0].size() * map.data.size());
	std::vector<Rectangle> rectangles = get_rectangles(map);
	Rectangle player(0.01f, Material(Color::Blue), Transform(glm::vec3(0.0f, -0.3f, 0.0f)));

	// -----------------------------------------------------------------

	glm::mat4 view;
 	glm::mat4 projection = glm::ortho(-ASPECT_RATIO, ASPECT_RATIO, -1.0, 1.0, 0.01, 100.0);
	default_shader.set_uniform("projection", projection);
	while (!glfwWindowShouldClose(window)) {
		process_input(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = glm::lookAt(camera.transform.position, camera.transform.position + camera.front, world_up);
		default_shader.set_uniform("view", view);

		Renderer::draw(player, default_shader);

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

std::vector<Rectangle> get_rectangles(const Map& map) {
	std::vector<Rectangle> rectangles;
	for (int y = 0; y < map.data.size(); y++) {
		for (int x = 0; x < map.data[0].size(); x++) {
			int index = map.data[y][x] - '0';

			if (index == 2) { index = 1; } // remove when doors are implemented

			float size = (X_MAX - X_MIN) / map.width;
			Material material(COLORS[index]);
			Rectangle rectangle(
				size,
				material,
				Transform(glm::vec3(X_MIN + size / 2.0f + size * x, Y_MAX - size / 2.0f - size * y, 0.0f)));
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

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "FPS Engine", NULL, NULL);
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