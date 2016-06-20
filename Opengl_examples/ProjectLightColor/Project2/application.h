#ifndef __INCLUDEAPPLICATION
#define __INCLUDEAPPLICATION

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "line.h"
#include "text_overlay.h"

#include <list>

class Program;

class application
{
public:
	application();
	~application();

	void run();

private:
	void initialize();
	void updateOverlay();
	void createAxis();
	void calulatePointsFromData();
	void dataGenerate(double time);
	void keyboard(int key, int scancode, int action);

	static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{ instance->keyboard(key, scancode, action); }

private:
	static application* instance;
	GLFWwindow* _window;
	std::shared_ptr<Program> _programDrawLine;

	Line _xAxisLine;
	Line _yAxisLine;

	int _windowWidth = 700;
	int _windowHeight = 600;
	float _fps = 0.0;

	Line _dataLine;
	Line _smaLine;
	Line _emaLine;

	std::list<int> _data;
	size_t _dataSize = 0;
	float _dataAverage = .0f;
	bool _generatorRunning = false;
	bool _fixedDataPoints = false;
	const int _fixePoints = 100;
	const int _averageInterval = 4;
	const int _generatorMaxValue = 100;	
	

	text_overlay   overlay;

};

#endif