#include "line.h"

#include "program.h"

Line::Line()
	: _firstColor({ 1.0f, 1.0f, 1.0f, 1.0f })
	, _secondColor({ 1.0f, 1.0f, 1.0f, 1.0f })
	, _countIndices(0)
	, _countVertices(0)
	, _program(nullptr)
{}

Line::~Line()
{
	reset();
}

void Line::setData(
	const std::vector<Point>& points,
	float height,
	CoordinateConvertor convertor,
	std::shared_ptr<Program> program)
{
	reset();

	if (points.size() == 0)
		return;

	if (program.get() == nullptr)
		return;

	_program = program;

	_countVertices = (points.size() - 1) * 8;
	GLfloat* vertex = new GLfloat[_countVertices];

	for (size_t i = 1; i < points.size(); ++i)
		calculateRectangle(
			convertor.point(points[i - 1]),
			convertor.point(points[i]),
			convertor.height(height),
			vertex + (i - 1) * 8
		);

	// пор€док растановки вершин дл€ пр€моугольника
	const int ind[] = { 0, 1, 2, 0, 2, 3 };
	//const int ind[] = { 1, 0, 2, 3 };
	const auto countInd = sizeof(ind) / sizeof(int);

	_countIndices = (points.size() - 1) * countInd;
	GLuint* indices = new GLuint[_countIndices];

	for (size_t i = 0, k = 0; i < _countIndices; ++k, i += countInd)
		for (auto j = 0; j < countInd; ++j)
			indices[k * countInd + j] = i + ind[j] - (k * 2);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _countVertices, vertex, GL_STATIC_DRAW);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		

		glGenBuffers(1, &_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _countIndices, indices, GL_STATIC_DRAW);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	delete[] vertex;
	delete[] indices;
}

void Line::setColor(Color firstColor, Color secondColor)
{
	_firstColor = firstColor;
	_secondColor = secondColor;
}

void Line::setColor(Color color)
{
	setColor(color, color);
}

void Line::draw()
{
	if (_countIndices == 0)
		return;

	
	GLint vertexFirstColorLocation = glGetUniformLocation(_program->id(), "firstColor");
	glUniform4f(vertexFirstColorLocation, _firstColor.r, _firstColor.g, _firstColor.b, _firstColor.a);
	GLint vertexSecondColorLocation = glGetUniformLocation(_program->id(), "secondColor");
	glUniform4f(vertexSecondColorLocation, _secondColor.r, _secondColor.g, _secondColor.b, _secondColor.a);

	_program->use();
	glBindVertexArray(_vao);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glDrawElements(GL_TRIANGLES, _countIndices, GL_UNSIGNED_INT, 0);


	glBindVertexArray(0);	
}

void Line::calculateRectangle(Point a, Point b, float h, GLfloat * v)
{
	auto calculateVertex = [](auto a, auto b, auto h, auto result)
	{
		if (a.x == b.x)
		{
			result[0] = (a.x + h);
			result[1] = (a.y);
		}
		else
		{
			const auto alfa = atan((b.y - a.y) / (b.x - a.x));
			result[0] = (a.x + h * sin(alfa));
			result[1] = (a.y - h * cos(alfa));
		}
	};

	const auto halfH = h / 2.0f;
	calculateVertex(a, b, halfH, v);
	calculateVertex(a, b, -halfH, v + 2);
	calculateVertex(b, a, -halfH, v + 4);
	calculateVertex(b, a, halfH, v + 6);
}

void Line::reset()
{
	if (_countIndices)
	{
		glDeleteVertexArrays(1, &_vao);
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ebo);
	}

	_countIndices = 0;
	_countVertices = 0;

	_program.reset();
}
