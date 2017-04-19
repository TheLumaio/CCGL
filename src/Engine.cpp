#include "Engine.h"



Engine::Engine(std::string title, int width, int height) :
	title(title),
	width(width),
	height(height)
{
	glfwInit();
	createWindow();
	
	// I shouldn't need to do this
	for (int i=0;i<16;i++)
		mouse[i] = false;
	for (int i=0;i<1024;i++)
		keys[i] = false;
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
	
	// glViewport(0, 0, width, height);
	// glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	lights.init();
	lightShader.load("data/gbuffer_vert.glsl", "data/gbuffer_frag.glsl");
	lightShader.use();
	glUniform1i(glGetUniformLocation(lightShader.getProgram(), "gPosition"), 0);
    glUniform1i(glGetUniformLocation(lightShader.getProgram(), "gNormal"), 1);
    glUniform1i(glGetUniformLocation(lightShader.getProgram(), "gAlbedoSpec"), 2);
	
	glClearColor(0,0,0,0);

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
	
	glfwSetCursorPosCallback(this->window, [](GLFWwindow* window, double x, double y)
	{ static_cast<Engine*>(glfwGetWindowUserPointer(window))->movedEvent(window, x, y); });
	
}

/// events

void Engine::movedEvent(GLFWwindow* window, double x, double y)
{
	currentState()->mouseMoved(x,y);
}

void Engine::mouseEvent(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		mouse[button] = true;
		currentState()->mousePressed(button, mods);
		break;
	case GLFW_RELEASE:
		mouse[button] = false;
		currentState()->mouseReleased(button, mods);
		break;
	default:
		mouse[button] = false;
		printf("Unhandled case in %s:%s at %d.\n", __FILE__, __func__, __LINE__);
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
		keys[key] = true;
		currentState()->keyPressed(key,mods);
		break;
	case GLFW_RELEASE:
		keys[key] = false;
		currentState()->keyReleased(key,mods);
		break;
	case GLFW_REPEAT:
		break;
	default:
		printf("Unhandled case in %s:%s at %d.\n", __FILE__, __func__, __LINE__);
		break;
	}
}

void Engine::textEvent(GLFWwindow* window, unsigned int text)
{
	currentState()->textEntered(text);
}

/// other

void Engine::renderLights()
{
	if (!lightVAO)
	{
		GLfloat lightVertices[] = {
            // Positions        // Texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &lightVAO);
        glGenBuffers(1, &lightVBO);
        glBindVertexArray(lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), &lightVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		
		printf("initialize quad\n");
	}
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

/// public

void Engine::start()
{
	//createWindow();
	initializeEvents();
	
	glfwMakeContextCurrent(this->window);
	while (!glfwWindowShouldClose(this->window))
	{
		glfwPollEvents();
		
		currentState()->update(0);
		
		
		lights.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currentState()->render();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		lightShader.use();
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, lights.gposition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, lights.gnormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, lights.gspec);
		
		glm::vec3 pos = camera->getPosition();
		for (int i = 0; i < 32; i++)
		{
			glUniform3fv(glGetUniformLocation(lightShader.getProgram(), std::string(std::string("lights[") + std::to_string(i) + std::string("].Position")).c_str()), 1, &pos[0]);
			glUniform3fv(glGetUniformLocation(lightShader.getProgram(), std::string(std::string("lights[") + std::to_string(i) + std::string("].Color")).c_str()), 1, &glm::vec3((rand()%100)/100, (rand()%100)/100, (rand()%100)/100)[0]);
			const GLfloat constant = 1.0;
			const GLfloat linear = 0.7;
			const GLfloat quadratic = 1.8;
			glUniform1f(glGetUniformLocation(lightShader.getProgram(), std::string(std::string("lights[") + std::to_string(i) + std::string("].Linear")).c_str()), linear);
			glUniform1f(glGetUniformLocation(lightShader.getProgram(), std::string(std::string("lights[") + std::to_string(i) + std::string("].Quadratic")).c_str()), quadratic);
		}
		glUniform3fv(glGetUniformLocation(lightShader.getProgram(), "viewPos"), 1, &pos[0]);
		renderLights();
		
		glfwSwapBuffers(this->window);
		
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

bool Engine::isKeyDown(int key)
{
	return keys[key];
}

bool Engine::isMousePressed(int button)
{
	return mouse[button];
}

void Engine::setCamera(Camera* cam)
{
	camera = cam;
}

Camera* Engine::getCamera()
{
	return camera;
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
