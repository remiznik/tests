#define _CRT_SECURE_NO_WARNINGS
#include "application.h"

#include <iostream>

#include "program.h"

application* application::instance = nullptr;

namespace
{
	int factorial(int i)
	{
		return (i == 1) ? 1 : i * factorial(i - 1);
	}
}

application::application()
{
	glfwInit();

	//std::cout << " version : " << glGetString(GL_VERSION) << std::endl;

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	_window = glfwCreateWindow(_windowWidth, _windowHeight, "LearnOpenGL", nullptr, nullptr);
	if (_window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		throw;
	}
	glfwMakeContextCurrent(_window);
	// Set the required callback functions
	glfwSetKeyCallback(_window, application::keyboardCallback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		throw;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(_window, &width, &height);
	glViewport(0, 0, width, height);

	instance = this;
}

void application::initialize()
{
	_programDrawLine = std::make_shared<Program>("shaders/frag.glsl", "shaders/vert.glsl");
	overlay.init(100, 200);

	createAxis();
}

void application::createAxis()
{
	std::vector<Point> pointsXLine{
		{ (float)_windowWidth, (float)_windowHeight },
		{ .0f, (float)_windowHeight }
	};
	_xAxisLine.setData(pointsXLine, 1.f, CoordinateConvertor(_windowWidth, _windowHeight, (float)_windowHeight), _programDrawLine);
	_xAxisLine.setColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	std::vector<Point> pointsYLine{
		{ .0f, (float)_windowHeight },
		{ .0f, .0f }
	};

	_yAxisLine.setData(pointsYLine, 1.f, CoordinateConvertor(_windowWidth, _windowHeight, (float)_windowHeight), _programDrawLine);
	_yAxisLine.setColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	_dataLine.setColor({ .4f, .0f, .8f, 1.0f }, { 0.502f, .0f, .0f, 1.0f });
	_smaLine.setColor({ 1.f, 1.0f, .0f, 1.0f });
	_emaLine.setColor({ .0f, 1.0f, .0f, 1.0f });
}

void application::run()
{
	initialize();

	double lastTime = 0.0;
	int frames = 0;

	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON_MODE);
	

	while (!glfwWindowShouldClose(_window))
	{
		double nowTime = glfwGetTime();
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(.0f, .0f, .0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		dataGenerate(nowTime);

		calulatePointsFromData();
		_xAxisLine.draw();
		_yAxisLine.draw();
		_dataLine.draw();
		_emaLine.draw();
		_smaLine.draw();


		if (nowTime > (lastTime + 0.25f))
		{
			_fps = float(frames) / ((float)nowTime - (float)lastTime);
			frames = 0;
			lastTime = nowTime;
		}

		updateOverlay();
		glfwSwapBuffers(_window);

		frames++;
	}
}

void application::calulatePointsFromData()
{
	std::vector<Point> data;
	data.reserve(_dataSize);

	std::vector<Point> smaData;
	smaData.reserve((int)ceil(_dataSize / _averageInterval));

	std::vector<Point> emaData;
	emaData.reserve((int)ceil(_dataSize / _averageInterval));

	int index = 0;
	float smaSum = .0f, emaSum = .0f;
	int emaInd = 1;
	int emaFactorial = factorial(_averageInterval);
	_dataAverage = .0f;

	float _maxValue = .0f;

	for (auto it = _data.begin(); it != _data.end(); ++it, ++index)
	{
		auto value = (float)(*it);
		data.push_back({ (float)index, value });

		smaSum += value;
		emaSum += emaInd * value;
		++emaInd;

		// нулеой индекс надо пропустить.
		if (index && !(index % _averageInterval))
		{
			// c = (x_0 + x_1 + ... + x_n) / n;
			//  n - количество значений в интервале
			//  x_n - значение функции в интервале с индексом n
			smaData.push_back({ (float)index, (float)(smaSum / _averageInterval) });

			// c = (P_0 * x_0 + P__1 * x_1 + ... + P_n * x_n) / !n;
			//	n - количество значений на интервале
			//  x_n - значение функции в интервале с индексом n
			//  P_n - весовой коэффициент для значения, в данной реализации  P изменяестя от (1 до (n +1).
			emaData.push_back({ (float)index, (float)(emaSum * 1.0f / emaFactorial) });

			smaSum = emaSum = 0;
			emaInd = 1;
		}

		if (value > _maxValue)
			_maxValue = value;

		_dataAverage += value;
	}

	_dataAverage /= _dataSize ? _dataSize : 1;

	_dataLine.setData(data, 1.f, CoordinateConvertor(_dataSize, _windowHeight, _maxValue), _programDrawLine);
	_smaLine.setData(smaData, 2.0f, CoordinateConvertor(_dataSize, _windowHeight, _maxValue), _programDrawLine);
	_emaLine.setData(emaData, 2.0f, CoordinateConvertor(_dataSize, _windowHeight, _maxValue), _programDrawLine);
}


void application::updateOverlay()
{
	char buffer[256];

	overlay.clear();

	sprintf(buffer, "time  %2.2fms ", 1000.0f / _fps);
	overlay.drawText(buffer, 0, 0);

	sprintf(buffer, "frame %4.2ffps", _fps);
	overlay.drawText(buffer, 0, 1);
	
	sprintf(buffer, "fixed points : %s ", _fixedDataPoints ? "On" : "Off");
	overlay.drawText(buffer, 0, 2);

	sprintf(buffer, "points    : %d ", _dataSize);
	overlay.drawText(buffer, 0, 3);

	sprintf(buffer, "average   : %2.2f", _dataAverage);
	overlay.drawText(buffer, 0, 4);

	auto vertices = _xAxisLine.countVertices()
		+ _yAxisLine.countVertices()
		+ _dataLine.countVertices()
		+ _emaLine.countVertices()
		+ _smaLine.countVertices();
		;

	sprintf(buffer, "triangles : %d ", vertices / 4);
	overlay.drawText(buffer, 0, 5);

	sprintf(buffer, "vertex    : %d ", vertices);
	overlay.drawText(buffer, 0, 6);

	overlay.draw();
}

void application::keyboard(int key, int scancode, int action)
{
	std::cout << key << std::endl;

	if (action == GLFW_PRESS)
	{
		switch (key) {
			case GLFW_KEY_ESCAPE:
			{
				glfwSetWindowShouldClose(_window, GL_TRUE);
				break;
			}
			case GLFW_KEY_N:
			{
				_data.push_back(rand() % _generatorMaxValue);
				++_dataSize;
				break;
			}
			case GLFW_KEY_R:
			{
				_generatorRunning = true;			
				break;
			}
			case GLFW_KEY_F:
			{
				_fixedDataPoints = !_fixedDataPoints;			
				break;
			}
			default:
				break;
		}
	}
	/*case 27:
	{
		tearDown();
		break;
	}
	case 'h':
	{
		printHelp();
		break;
	}
	case 'r':
	{
		_generatorRunning = true;
		runDataGenetate();
		break;
	}
	case 'f':
	{
		_fixedDataPoints = !_fixedDataPoints;
		glutPostRedisplay();
		break;
	}
	case 's':
	{
		_generatorRunning = false;
		break;
	}
	default:
		break;
	}*/
}

void application::dataGenerate(double time)
{
	static double lastTime = 0.0f;
	lastTime += time - lastTime;

	if (!_generatorRunning)
		return;

	if (lastTime > 10.5f)
	{
		lastTime = time;
		_data.push_back(rand() % _generatorMaxValue);
		++_dataSize;

		while (_fixedDataPoints && (int)_dataSize > _fixePoints)
		{
			_data.pop_front();
			--_dataSize;
		}
	}
}

application::~application()
{
}
