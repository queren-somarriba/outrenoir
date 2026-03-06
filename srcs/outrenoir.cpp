#include "outrenoir.hpp"

bool fileExist(const std::string& name)
{
	std::ifstream f(name.c_str());
	return f.good();
}

int main(int argc, char** argv)
{
	renderContext	context;
	otData	data;

	if (argc < 5)
	{
		std::cerr << "Usage: ./outrenoir <scale> <setpSize> <speed> <record>" << std::endl;//stepSize, nombre de lignes, seed
		return 1;
	}

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	data.scale = std::stof(argv[1]);//scale between 0.0f and 1.0f
	data.stepSize = std::stof(argv[2]);
	data.record = std::stof(argv[4]);

	GLFWwindow* window = initWindow();
	if (!window)
		return -1;

	std::vector<vec2> grid(WIDTH * HEIGHT);
	grid = makeGrid(data);

	std::vector<vec2> outputList;
	poissonDiskSampling(outputList);

	std::vector<vec2> allSegments;
	allSegments.reserve(2000 * 500 * 6);

	collisionContext col_ctx;
	col_ctx.cellSize = LINE_PADDING / sqrtf(2.0f);
	col_ctx.cols = std::ceil(WIDTH / col_ctx.cellSize);
	col_ctx.rows = std::ceil(HEIGHT / col_ctx.cellSize);
	col_ctx.grid = std::vector<int>(col_ctx.cols * col_ctx.rows, -1);


	for (int i = 0; i < static_cast<int>(outputList.size()); ++i)
		makeSegments({outputList[i].x, outputList[i].y}, data, grid, allSegments, i, col_ctx);

	context.vertexCount = allSegments.size();
	SetupBuffers(allSegments, context.VAO, context.VBO);
	context.shaderProgram = CompileShaders();

	context.DrawSpeed = DRAW_SPEED * std::stof(argv[3]);
	context.currentDrawCount = 0;

	if (data.record)
	{
		int fbWidth, fbHeight;
		std::string	name = "outrenoir_";
		std::string	extName = ".mp4";
		int nb = 0;
		std::string	fileName = name + std::to_string(nb) + extName;
		
		while (fileExist(fileName))
		{
			++nb;
			fileName = name + std::to_string(nb) + extName;
		}	
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		std::string cmd = "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s " + std::to_string(fbWidth) + "x" + std::to_string(fbHeight) + " -r 60 -i - -vf vflip -c:v libx264 -pix_fmt yuv420p " + fileName;
		context.ffmpegPipe = popen(cmd.c_str(), "w");
		context.isRecording = true;
	

		if (!context.ffmpegPipe)
			std::cerr << "Erreur : Impossible d'ouvrir le pipe FFmpeg. Est-il installé ?" << std::endl;
	}

	while (!glfwWindowShouldClose(window))
		renderScene(window, context);

	glDeleteBuffers(1, &context.VBO);
	glfwTerminate();

	return 0;
}