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
	static glm::vec2 mouse_point;
	static glm::vec3 lastPoint;

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
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void resizeObject(float);
	static void translateObject(glm::vec3);
	static void scaleObject(glm::vec3);
	static void orbitObject(float);
	static void resetObject();
	static void resetLight();

	static void mouseRotateObject(float, glm::vec3);
	static void mouseRotateDirLight(float rot_angle, glm::vec3 rotAxis);
	static void mouseRotatePointLight(float rot_angle, glm::vec3 rotAxis);
	static void scrollTranslatePointLight(glm::vec3 transVec);
	static void mouseRotateSpotLight(float rot_angle, glm::vec3 rotAxis);
	static void changeSpotLightCutOff(float change);
	static void changeSpotLightSpotExponent(float change);
	static void scrollTranslateSpotLight(glm::vec3 transVec);

	static OBJObject * getObject();
	static glm::vec3 trackBallMapping(glm::vec2 point);
	static int getHeight();
	static int getWidth();
	static glm::vec3 getCamPos();
};

#endif
