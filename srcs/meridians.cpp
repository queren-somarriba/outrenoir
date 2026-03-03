#include "meridians.hpp"

int main(int argc, char** argv)
{
	renderContext	context;
	meridiansData	data;

	if (argc < 5)
	{
		std::cerr << "Usage: ./meridians <width> <height> <scale> <setpSize>" << std::endl;//stepSize, nombre de lignes, seed
		return 1;
	}

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	data.width = std::stoi(argv[1]);
	data.height = std::stoi(argv[2]);
	data.scale = std::stof(argv[3]);//scale between 0.0f and 1.0f
	data.stepSize = std::stof(argv[4]);
	float startX, startY;

	GLFWwindow* window = initWindow(data);
	if (!window)
		return -1;

	std::vector<vec2> grid(data.width * data.height);
	grid = makeGrid(data);
	std::vector<vec2> allSegments;
	allSegments.reserve(2000 * 500 * 6);

	for (int i = 0; i < 2000; ++i)
	{
		startX = (static_cast<float>(std::rand()) / RAND_MAX) * data.width;
		startY = (static_cast<float>(std::rand()) / RAND_MAX) * data.height;
		makeSegments({startX, startY}, data, grid, allSegments);
	}

	context.vertexCount = allSegments.size();
	SetupBuffers(allSegments, context.VAO, context.VBO);
	context.shaderProgram = CompileShaders();

	while (!glfwWindowShouldClose(window))
		renderScene(window, context, data);

	glDeleteBuffers(1, &context.VBO);
	glfwTerminate();

	return 0;
}