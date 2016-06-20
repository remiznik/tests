#ifndef __INCLUDECONVERTOR
#define __INCLUDECONVERTOR

#include "primitives.h"

//  ласс CpprdinateConvertor, преобразует вхд€шие значени€ в координаты Opengl 
// преусмотрено небольшое смешение от краЄв экрана дл€ красивого оборожени€ графиков.
class CoordinateConvertor
{
public:
	CoordinateConvertor(int maxWidth, int height, float maxHeight);

	Point point(const Point& p) const;

	float height(float height) const;

private:
	const float OPENGL_X_MIN = -1.0;
	const float OPENGL_X_MAX = 1.0;
	const float OPENGL_Y_MIN = -1.0;
	const float OPENGL_Y_MAX = 1.0;

	int _height;

	float _dw;
	float _dh;
	float _biasW;
	float _biasH;

};

#endif

