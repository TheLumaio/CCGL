#include <iostream>

#include "Engine.h"
#include "test.h"
#include "Camera.h"

class MenuState : public GameState
{
private:
	Test test;
	Shader shader;
	Camera camera;
public:
	MenuState(Engine* ref) : GameState(ref),
	shader("data/vertex.glsl", "data/fragment.glsl"),
	camera(&shader) {}
	~MenuState()=default;
	
	void enter(GameState* prev) override
	{
		std::cout << "entered state " << this << " from " << prev << std::endl;
	}
	
	void update(float dt) override
	{
		
	}
	
	void render() override
	{
		// glClearColor(0, 1, 0, 1);
		shader.use();
		camera.attach();
		
		test.render();
	}
	
};

int main(int argc, char** argv)
{
	std::cout << "Test" << std::endl;
	
	Engine engine("OpenGL test", 1280, 720);
	engine.changeState(new MenuState(&engine));
	engine.start();
	
	return 0;
}
