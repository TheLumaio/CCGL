
#ifndef ENGINE_H
#define ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stack>

class Engine;

class GameState
{
protected:
	Engine* gameref;
public:
	GameState(Engine* ref) : gameref(ref) {}
	virtual ~GameState() {}

	virtual void enter(GameState*) {}
	virtual void leave(GameState*) {}

	virtual void mousePressed  (int, int) {}
	virtual void mouseReleased (int, int) {}
	virtual void mouseScrolled (double,double) {}
	virtual void keyPressed    (int,int) {}
	virtual void keyReleased   (int,int) {}
	virtual void textEntered   (unsigned int) {}

	virtual void update(float) {}
	virtual void render() {}
};

class Engine
{
private:
	GLFWwindow* window;
	std::string title;
	int width;
	int height;

	void createWindow();
	void initializeEvents();
	
	/// events
	void mouseEvent(GLFWwindow*, int, int, int);
	void scrollEvent(GLFWwindow*, double, double);
	void keyEvent(GLFWwindow*, int, int, int, int);
	void textEvent(GLFWwindow*, unsigned int);

	std::stack<GameState*> states; // maybe use a state handler?
	
public:
	Engine(std::string, int, int);
	~Engine()=default;
	
	void start();

	std::string getTitle();
	int getWidth();
	int getHeight();
	GLFWwindow* getWindow();

	void changeState(GameState*);
	GameState* currentState();
	
};

#endif
