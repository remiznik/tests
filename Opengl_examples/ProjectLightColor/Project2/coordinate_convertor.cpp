#include "coordinate_convertor.h"

CoordinateConvertor::CoordinateConvertor(int maxWidth, int height, float maxHeight)
	: _height(height)
	, _dw((((OPENGL_X_MAX - OPENGL_X_MIN) - ((OPENGL_X_MAX - OPENGL_X_MIN) * 0.05f)) / maxWidth))
	, _dh((((OPENGL_Y_MIN - OPENGL_Y_MAX) - ((OPENGL_Y_MIN - OPENGL_Y_MAX) * 0.05f)) / maxHeight))
	, _biasW(OPENGL_X_MIN * (1 - 0.05f))
	, _biasH(OPENGL_Y_MAX - (OPENGL_Y_MAX* 0.05f))
{
}

Point CoordinateConvertor::point(const Point& p) const
{
	return{ _biasW + _dw * p.x,	_biasH + _dh * p.y };
}

float CoordinateConvertor::height(float height) const
{
	return ((OPENGL_Y_MAX - OPENGL_Y_MIN) / _height) * height;
}
