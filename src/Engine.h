
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
	Engine* core;
public:
	GameState(Engine* ref) : core(ref) {}
	virtual ~GameState() {}

	virtual void enter(GameState*) {}
	virtual void leave(GameState*) {}

	virtual void mousePressed  (int, int) {}
	virtual void mouseReleased (int, int) {}
	virtual void mouseMoved    (double,double) {}
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
	void movedEvent(GLFWwindow*, double, double);
	void mouseEvent(GLFWwindow*, int, int, int);
	void scrollEvent(GLFWwindow*, double, double);
	void keyEvent(GLFWwindow*, int, int, int, int);
	void textEvent(GLFWwindow*, unsigned int);

	std::stack<GameState*> states; // maybe use a state handler?
	
	bool keys[1024];
	bool mouse[16];
	
public:
	Engine(std::string, int, int);
	~Engine()=default;
	
	void start();

	std::string getTitle();
	int getWidth();
	int getHeight();
	GLFWwindow* getWindow();
	
	bool isKeyDown(int);
	bool isMousePressed(int);

	void changeState(GameState*);
	GameState* currentState();
	
};

#endif
