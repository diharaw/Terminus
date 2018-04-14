#pragma once

#include <string>
#include <GLFW/glfw3.h>
#include "macros.hpp"

#define PROTOGFX_DECLARE_MAIN(x)		  \
int main()								  \
{										  \
	protogfx::Application* app = new x(); \
	app->run();							  \
	delete app;							  \
	return 0;							  \
}										  \

PROTOGFX_BEGIN_NAMESPACE

class Application
{
public:
	Application();
	virtual ~Application();
	void run();

private:
	bool init_internal();
	void shutdown_internal();
	void update();
	bool read_config();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	virtual bool init() = 0;
	virtual void render() = 0;
	virtual void frame(double dt) = 0;
	virtual void shutdown() = 0;

public:
	static bool keys[1024];
	static bool	mouse[5];

private:
	GLFWwindow* _window;
	uint16_t	_width;
	uint16_t	_height;
	std::string _title;
	std::string _script;
	uint16_t	_refresh_rate;
	int			_major_version;
	int			_minor_version;
	bool		_vsync;
	bool		_resizable;
	bool		_maximized;
	double		_dt;
};

PROTOGFX_END_NAMESPACE