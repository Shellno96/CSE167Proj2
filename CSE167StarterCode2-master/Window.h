#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "OBJObject.h"
#include "shader.h"

class Window
{
private:
	static OBJObject * object1;
	static OBJObject * object2;
	static OBJObject * object3;
	static int object_num;

public:
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void resizeObject(float);
	static void translateObject(glm::vec3);
	static void scaleObject(glm::vec3);
	static void orbitObject(float);
	static void resetObject();
	static OBJObject * getObject();
	static int getHeight();
	static int getWidth();
};

#endif
