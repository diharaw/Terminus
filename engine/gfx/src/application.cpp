#include <gfx/include/application.hpp>
#include <gfx/include/filesystem.hpp>
#include <json.hpp>

PROTOGFX_BEGIN_NAMESPACE

bool Application::keys[1024];
bool Application::mouse[5];

Application::Application()
{
	_dt = 0;
}

Application::~Application()
{

}

bool Application::init_internal()
{
	glfwInit();

	if (!read_config())
		return false;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_RESIZABLE, _resizable);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _major_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _minor_version);
	glfwWindowHint(GLFW_MAXIMIZED, _maximized);
	glfwWindowHint(GLFW_REFRESH_RATE, _refresh_rate);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);

	if (!_window)
		return false;

	glfwSetKeyCallback(_window, key_callback);
	glfwSetCursorPosCallback(_window, mouse_callback);
	glfwSetScrollCallback(_window, scroll_callback);

	glfwMakeContextCurrent(_window);

	return true;
}

bool Application::read_config()
{
	OsFile* file = filesystem::open_file("res/config.json", FileMode::READ_TEXT);

	if (!file)
		return false;

	size_t file_size = file->size();
	char* buffer = (char*)malloc(file_size + 1);
	file->read(file_size, buffer);
	buffer[file_size] = '\0';
	nlohmann::json json = nlohmann::json::parse((const char*)buffer);

	_width = json["width"];
	_height = json["height"];
	std::string title = json["title"];
	_title = title;
	std::string script = json["script"];
	_script = script;
	_vsync = json["vsync"];
	_resizable = json["resizable"];
	_maximized = json["maximized"];
	_refresh_rate = json["refresh_rate"];
	_major_version = json["major_version"];
	_minor_version = json["minor_version"];

	free(buffer);

	filesystem::close_file(file);

	return true;
}

void Application::shutdown_internal()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Application::update()
{
	glfwPollEvents();
}

void Application::run()
{
	if (!init_internal())
		return;

	if (!init())
		return;

	while (!glfwWindowShouldClose(_window))
	{
		double start = glfwGetTime();
		update();
		frame(_dt);
		render();
		glfwSwapBuffers(_window);
		_dt = glfwGetTime() - start;
	}

	shutdown();
	shutdown_internal();
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

}

void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

PROTOGFX_END_NAMESPACE