#include "window.h"

const char* window_title = "GLFW Starter Project";
GLint shaderProgram;

OBJObject * Window::object1;
OBJObject * Window::object2;
OBJObject * Window::object3;

int Window::object_num;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{
	object1 = new OBJObject("C:\\Users\\Ty\\Documents\\School\\FA 16\\CSE 167\\CSE167StarterCode-master\\MyResources\\bunny.obj");
	object2 = new OBJObject("C:\\Users\\Ty\\Documents\\School\\FA 16\\CSE 167\\CSE167StarterCode-master\\MyResources\\bear.obj");
	object3 = new OBJObject("C:\\Users\\Ty\\Documents\\School\\FA 16\\CSE 167\\CSE167StarterCode-master\\MyResources\\dragon.obj");

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(object1);
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	/*
	// Call the update function the OBJObject
	if (object_num == 0)
		object1->update();
	else if (object_num == 1)
		object2->update();
	else if (object_num == 2)
		object3->update();
	*/
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	
	// Render objects
	if (object_num == 0)
		object1->draw(shaderProgram);
	else if (object_num == 1)
		object2->draw(shaderProgram);
	else if (object_num == 2)
		object3->draw(shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{

		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		else if (key == GLFW_KEY_F1)
		{
			object_num = 0;
			//cout << "F1" << endl;
		}

		else if (key == GLFW_KEY_F2)
		{
			object_num = 1;
			//cout << "F2" << endl;
		}

		else if (key == GLFW_KEY_F3)
		{
			object_num = 2;
			//cout << "F3" << endl;
		}

		else if (key == GLFW_KEY_P)
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				resizeObject(1.0f);
			}
			else
			{
				resizeObject(-1.0f);
			}
		}

		else if (key == GLFW_KEY_X)
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				translateObject({ 1.0f,0.0f,0.0f });
				//cout << "X" << mods << endl;
			}
			else
			{
				translateObject({ -1.0f,0.0f,0.0f });
				//cout << "x" << endl;
			}
		}

		else if (key == GLFW_KEY_Y)
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				translateObject({ 0.0f,1.0f,0.0f });
				//cout << "Y" << mods << endl;
			}
			else
			{
				translateObject({ 0.0f,-1.0f,0.0f });
				//cout << "y" << endl;
			}
		}

		else if (key == GLFW_KEY_Z)
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				translateObject({ 0.0f,0.0f,1.0f });
				//cout << "Z" << mods << endl;
			}
			else
			{
				translateObject({ 0.0f,0.0f,-1.0f });
				//cout << "z" << endl;
			}
		}

		else if (key == GLFW_KEY_S)
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				scaleObject({ 2.0f,2.0f,2.0f });
				//cout << "S" << mods << endl;
			}
			else
			{
				scaleObject({ 0.5f,0.5f,0.5f });
				//cout << "s" << endl;
			}
		}

		else if (key == GLFW_KEY_O)
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				orbitObject(-10.0f);
				//cout << "O" << mods << endl;
			}
			else
			{
				orbitObject(10.0f);
				//cout << "o" << endl;
			}
		}

		else if (key == GLFW_KEY_R)
		{
			resetObject();
			//cout << "r" << endl;
		}
	}
}

void Window::resizeObject(float change)
{
	if (object_num == 0)
		object1->resize(change);
	else if (object_num == 1)
		object2->resize(change);
	else if (object_num == 2)
		object3->resize(change);
}

void Window::translateObject(glm::vec3 transVec)
{
	if (object_num == 0)
		object1->translate(transVec);
	else if (object_num == 1)
		object2->translate(transVec);
	else if (object_num == 2)
		object3->translate(transVec);
}

void Window::scaleObject(glm::vec3 transVec)
{
	if (object_num == 0)
		object1->scale(transVec);
	else if (object_num == 1)
		object2->scale(transVec);
	else if (object_num == 2)
		object3->scale(transVec);
}

void Window::orbitObject(float deg)
{
	if (object_num == 0)
		object1->orbit(deg);
	else if (object_num == 1)
		object2->orbit(deg);
	else if (object_num == 2)
		object3->orbit(deg);
}

void Window::resetObject()
{
	if (object_num == 0)
		object1->reset();
	else if (object_num == 1)
		object2->reset();
	else if (object_num == 2)
		object3->reset();
}

OBJObject * Window::getObject() {
	if (object_num == 0)
		return object1;
	else if (object_num == 1)
		return object2;
	else if (object_num == 2)
		return object3;
}

int Window::getHeight() {
	return height;
}

int Window::getWidth() {
	return width;
}