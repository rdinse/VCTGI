#ifndef __CONTEXT_H
#define __CONTEXT_H

#include "Singleton.h"
#include "Common.h"

#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>

#define DEFAULT_CONTEXT_WIDTH 800
#define DEFAULT_CONTEXT_HEIGHT 600

// Forward declaration of GLFW and AntTweakBar callback functions.
void resizeCallback(GLFWwindow *window, int w, int h);
void pollMouseAndKeyboardEvents(GLFWwindow* window);
void TW_CALL reloadShadersButtonCallback(void *clientData);

class Context : public Singleton <Context>
{
friend class Singleton <Context>;
public:
	~Context();
	void init(void);
	void initAntTweakBarAndProfiler(void);
	void afterDrawUpdate(void);
	void printOpenGLInfo(void);

	std::string* getTitle();
	TwBar* getAntTweakBar();
	int getWidth();
	int getHeight();
	GLFWwindow* getWindow();

	void setSize(int width, int height);
protected:
	Context();
private:
	GLenum m_lastErr;
	GLFWwindow* m_window;
	std::string m_title;
	int m_width;
	int m_height;
	TwBar* tw_tweakBar;
};

#endif /* __CONTEXT_H */
