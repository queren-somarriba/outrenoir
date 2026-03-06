#pragma once

#include <GL/glew.h>
#include <vector>

#define f_PI		3.14159265f
#define WIDTH		720
#define HEIGHT		1080
#define PSD_R		10.0f
#define LINE_PADDING	1.0f
#define LINE_LENGTH	10000

struct vec2
{
	float	x;
	float	y;
};

struct meridianData
{
	float	scale;
	float	stepSize;
};

struct renderContext
{
	GLuint	VAO;
	GLuint	VBO;
	GLuint	shaderProgram;
	int	vertexCount;
	int	currentDrawCount;
	int	DrawSpeed;
};

struct pdsContext
{
	int			cols;
	int			rows;
	float			cellSize;
	std::vector<int>	grid;
	std::vector<int>	activeList;
};

struct collisionContext
{
	int			cols;
	int			rows;
	float			cellSize;
	std::vector<int>	grid;
};