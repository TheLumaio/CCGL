#include <iostream>

#include "Engine.h"
#include "test.h"
#include "Camera.h"
#include "Model.h"

class MenuState : public GameState
{
private:
	Shader shader;
	Camera camera;
	Model model;
	Model barrel;
	Test bounds;
	
	bool moving;
public:
	MenuState(Engine* ref) :
		GameState(ref),
		shader("data/vertex.glsl", "data/fragment.glsl"),
		model("data/tex/nanosuit2.obj"),
		barrel("data/barrel.fbx"),
		camera(&shader),
		moving(false)
	{
		core->setCamera(&camera);
	}
	~MenuState()=default;
	
	void mouseMoved(double x, double y) override
	{
		if (core->isMousePressed(1)) {
			if (!moving) {
				camera.lastx = x;
				camera.lasty = y;
				moving = true;
			}
			camera.mouseMove(x,y);
		} else {
			moving = false;
		}
	}
	
	void enter(GameState* prev) override
	{
		std::cout << "entered state " << this << " from " << prev << std::endl;
		glfwSetInputMode(core->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	
	void update(float dt) override
	{
		
		// model.setPosition(glm::vec3(cos(glfwGetTime()*15), 0, 0));
		
		if (core->isKeyDown(GLFW_KEY_W)) {
			camera.setPosition(camera.getPosition() + camera.getFront());
		}
		if (core->isKeyDown(GLFW_KEY_S)) {
			camera.setPosition(camera.getPosition() - camera.getFront());
		}
		if (core->isKeyDown(GLFW_KEY_A)) {
			camera.setPosition(camera.getPosition() - glm::normalize(glm::cross(camera.getFront(), camera.getUp())));
		}
		if (core->isKeyDown(GLFW_KEY_D)) {
			camera.setPosition(camera.getPosition() + glm::normalize(glm::cross(camera.getFront(), camera.getUp())));
		}
		
		if (core->isMousePressed(1)) {
			glfwSetInputMode(core->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		} else {
			glfwSetInputMode(core->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	
	void render() override
	{
		// glClearColor(0.1, 0.1, 0.1, 1);
		shader.use();
		
		camera.update(0.1f);
		
		model.render(&shader);
		
		barrel.setScale(glm::vec3(5, 5, 5));
		barrel.setPosition(glm::vec3(16, 0, 5));
		barrel.render(&shader);
		bounds.render(&shader);
	}
	
};

int main(int argc, char** argv)
{
	std::cout << "Test" << std::endl;
	
	Engine engine("StandigeEngine", 1280, 720);
	engine.changeState(new MenuState(&engine));
	engine.start();
	
	return 0;
}
