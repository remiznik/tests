#ifndef __INCLUDEGEOMETRY
#define __INCLUDEGEOMETRY

#include <vector>
#include <memory>

#include <gl/glew.h>

#include "primitives.h"
#include "coordinate_convertor.h"

class Program;

// ����� Line, ������������ ��� ����������� ������ ����� � ���� ����� � OpenGl.
// ������ ��� ����� ����� ������������� � ������������� � ������� (height) �������� ��� ��� ������������.
class Line
{
public:
	Line();
	~Line();

	void setData(const std::vector<Point>& points, float height, CoordinateConvertor convertor, std::shared_ptr<Program> program);
	void setColor(Color firstColor, Color secondColor);
	void setColor(Color color);

	void draw();
	void reset();

	size_t countVertices() const { return _countVertices; }

private:
	void calculateRectangle(Point a, Point b, float h, GLfloat* v);

private:
	Color _firstColor;
	Color _secondColor;

	GLuint _vao;
	GLuint _vbo;
	GLuint _ebo;

	size_t _countIndices;
	size_t _countVertices;

	std::shared_ptr<Program> _program;
};

#endif