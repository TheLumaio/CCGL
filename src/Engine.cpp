#include "Engine.h"



Engine::Engine(std::string title, int width, int height) :
	title(title),
	width(width),
	height(height)
{
	glfwInit();
	createWindow();
}

void Engine::createWindow()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	this->window = glfwCreateWindow(this->width, this->height, this->title.c_str(), nullptr, nullptr);

	glfwSetWindowUserPointer(this->window, this);
	
	glfwMakeContextCurrent(this->window);
	glewExperimental=true;
	
	glewInit();

}

void Engine::initializeEvents()
{
	glfwSetMouseButtonCallback(this->window, [](GLFWwindow* window, int button, int action, int mods)
	{ static_cast<Engine*>(glfwGetWindowUserPointer(window))->mouseEvent(window, button, action, mods); });

	glfwSetScrollCallback(this->window, [](GLFWwindow* window, double dx, double dy)
	{ static_cast<Engine*>(glfwGetWindowUserPointer(window))->scrollEvent(window, dx, dy); });

	glfwSetKeyCallback(this->window, [](GLFWwindow* window, int key, int scan, int action, int mods)
	{ static_cast<Engine*>(glfwGetWindowUserPointer(window))->keyEvent(window, key, scan, action, mods); });

	glfwSetCharCallback(this->window, [](GLFWwindow* window, unsigned int text)
	{ static_cast<Engine*>(glfwGetWindowUserPointer(window))->textEvent(window, text); });
}

/// events

void Engine::mouseEvent(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		currentState()->mousePressed(button, mods);
		break;
	case GLFW_RELEASE:
		currentState()->mouseReleased(button, mods);
		break;
	default:
		printf("Unhandled case in %s:%s at %d.", __FILE__, __func__, __LINE__);
		break;
	}
}

void Engine::scrollEvent(GLFWwindow* window, double dx, double dy)
{
	currentState()->mouseScrolled(dx,dy);
}

void Engine::keyEvent(GLFWwindow* window, int key, int scan, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		currentState()->keyPressed(key,mods);
		break;
	case GLFW_RELEASE:
		currentState()->keyReleased(key,mods);
		break;
	default:
		printf("Unhandled case in %s:%s at %d.", __FILE__, __func__, __LINE__);
		break;
	}
}

void Engine::textEvent(GLFWwindow* window, unsigned int text)
{
	currentState()->textEntered(text);
}

/// public

void Engine::start()
{
	//createWindow();
	initializeEvents();
	
	glfwMakeContextCurrent(this->window);
	while (!glfwWindowShouldClose(this->window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		currentState()->render();
		
		glfwSwapBuffers(this->window);
		glfwPollEvents();
		
	}

	glfwTerminate();

}

std::string Engine::getTitle() {
	return this->title;
}

int Engine::getWidth() {
	return this->width;
}

int Engine::getHeight() {
	return this->height;
}

GLFWwindow* Engine::getWindow()
{
	return this->window;
}

void Engine::changeState(GameState* state)
{
	if (!this->states.empty()) {
		this->states.top()->leave(state);
		this->states.pop();
	}
	auto prev = !this->states.empty() ? this->states.top() : nullptr;
	state->enter(prev);
	this->states.push(state);
}

GameState* Engine::currentState()
{
	return this->states.top();
}
