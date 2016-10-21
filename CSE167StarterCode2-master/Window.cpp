#include "window.h"

const char* window_title = "GLFW Starter Project";
GLint shaderProgram;

OBJObject * Window::object1;
OBJObject * Window::object2;
OBJObject * Window::object3;

int Window::object_num;
glm::vec2 Window::mouse_point;
glm::vec3 Window::lastPoint;

int Movement;
const int NONE = 0;
const int ROTATE = 1;
const int TRANSLATE = 2;
const float m_ROTSCALE = 1.0f;
const float m_ZOOMSCALE = 1.0f;
const float m_TRANSSCALE = 0.1f;

int Mode;
const int OBJECT = 0;
const int DIRLIGHT = 1;
const int POINTLIGHT = 2;
const int SPOTLIGHT = 3;
const int NORMALS = 4;

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
	const vector<glm::vec3> bunnyMaterial = 
	{
		{0.7f, 0.28f, 0.49f},
		{0.727811f,	0.326959f, 0.326959f},
		{0.1745f, 0.04175f, 0.05175f}
	};
	float bunnyShininess = 0.2f;

	const vector<glm::vec3> bearMaterial =
	{
		{ 0.2f, 0.4f, 0.5f},
		{ 0.03f, 0.6f, 0.7f},
		{ 0.03f, 0.04f, 0.5f}
	};
	float bearShininess = .038125f;

	const vector<glm::vec3> dragonMaterial =
	{
	{ 0.05164f, 0.50648f, 0.22648f },
	{ 0.028281f, 0.655802f, 0.366065f },
	{ 0.04725f, 0.3995f, 0.2745f }
	};
	float dragonShininess = 0.85f;

	object1 = new OBJObject("C:\\Users\\Ty\\Documents\\School\\FA 16\\CSE 167\\CSE167StarterCode-master\\MyResources\\bunny.obj", bunnyMaterial, bunnyShininess);
	object2 = new OBJObject("C:\\Users\\Ty\\Documents\\School\\FA 16\\CSE 167\\CSE167StarterCode-master\\MyResources\\bear.obj", bearMaterial, bearShininess);
	object3 = new OBJObject("C:\\Users\\Ty\\Documents\\School\\FA 16\\CSE 167\\CSE167StarterCode-master\\MyResources\\dragon.obj", dragonMaterial, dragonShininess);

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(object1);
	delete(object2);
	delete(object3);
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

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Set the last know mouse_point
	mouse_point = { xpos, ypos };

	glm::vec3 direction;
	float rot_angle;
	glm::vec3 curPoint;

	// Handle any necessary mouse movements
	switch (Movement)
	{

		// Left-mouse button is being held down
		case ROTATE: 
		{
			curPoint = trackBallMapping(mouse_point); 
			direction = curPoint - lastPoint;
			float velocity = glm::length(direction);
				
			if (velocity > 0.0001) // If little movement - do nothing.
			{
				// Rotate about the axis that is perpendicular to the great circle connecting the mouse movements.
				glm::vec3 rotAxis;
				rotAxis = glm::cross(lastPoint, curPoint);
				rot_angle = velocity * m_ROTSCALE;
					
				//Apply the rotation
				switch (Mode)
				{
					case OBJECT:
						mouseRotateObject(rot_angle, rotAxis);
						break;
					case DIRLIGHT:
						mouseRotateDirLight(-rot_angle, rotAxis);
						break;
					case POINTLIGHT:
						mouseRotatePointLight(rot_angle, rotAxis);
						break;
					case SPOTLIGHT:
						mouseRotateSpotLight(rot_angle, rotAxis);
						break;
				}
			}
			break;
		}

		//Right-mouse button is being held down
		case TRANSLATE:
		{
			curPoint = trackBallMapping(mouse_point);
				
			//Get the translation vector
			direction = curPoint - lastPoint;
			float velocity = glm::length(direction);
			if (velocity > 0.0001) // If little movement - do nothing.
			{
				//Apply the translation
				switch (Mode)
				{
					case OBJECT:
						translateObject({ direction.x * m_TRANSSCALE, direction.y * m_TRANSSCALE, 0 });
						break;
					case DIRLIGHT:
						break;
					case POINTLIGHT:
						break;
					case SPOTLIGHT:
						changeSpotLightCutOff(direction.y * m_TRANSSCALE);
						break;
				}
			}
			break;
		}

	// Save the location of the current point for the next movement.
	lastPoint = curPoint;
	}
}

void Window::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		glm::vec2 point = mouse_point;
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			//Rotate mode
			Movement = ROTATE;
			lastPoint = trackBallMapping(point);
		}

		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			//Translate mode
			Movement = TRANSLATE;
			lastPoint = trackBallMapping(point);
		}
	}

	else if (action == GLFW_RELEASE)
	{
		Movement = NONE;
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	switch (Mode)
	{
		case OBJECT:
			translateObject({ 0,0, -1 * yoffset * m_ZOOMSCALE });
			break;
		case DIRLIGHT:
			break;
		case POINTLIGHT:
			scrollTranslatePointLight({ 0,0, -1 * yoffset * m_ZOOMSCALE});
			break;
		case SPOTLIGHT:
			scrollTranslateSpotLight({ 0,0, -1 * yoffset * m_ZOOMSCALE});
			break;
	}
}

glm::vec3 Window::trackBallMapping(glm::vec2 point)
{
	glm::vec3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	float d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
	v.x = (2.0*point.x - Window::width) / Window::width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v.y = (Window::height - 2.0*point.y) / Window::height;   // this does the equivalent to the above for the mouse Y position
	v.z = 0.0;   // initially the mouse z position is set to zero, but this will change below
	d = glm::length(v);    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d<1.0) ? d : 1.0;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	v.z = sqrtf(1.001 - d*d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	glm::normalize(v); // Still need to normalize, since we only capped d, not v.
	return v;  // return the mouse location on the surface of the trackball
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

		else if (key == GLFW_KEY_E)
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				changeSpotLightSpotExponent(1.0f);
				//cout << "E" << mods << endl;
			}
			else
			{
				changeSpotLightSpotExponent(-1.0f);
				//cout << "e" << endl;
			}
		}

		else if (key == GLFW_KEY_R)
		{
			resetObject();
			//cout << "r" << endl;
		}

		else if (key == GLFW_KEY_L)
		{
			resetLight();
		}

		else if (key == GLFW_KEY_0)
		{
			Mode = OBJECT;
		}

		else if (key == GLFW_KEY_1)
		{
			Mode = DIRLIGHT;
			if (object_num == 0)
				object1->setLightType(0);
			else if (object_num == 1)
				object2->setLightType(0);
			else if (object_num == 2)
				object3->setLightType(0);
		}

		else if (key == GLFW_KEY_2)
		{
			Mode = POINTLIGHT;
			if (object_num == 0)
				object1->setLightType(1);
			else if (object_num == 1)
				object2->setLightType(1);
			else if (object_num == 2)
				object3->setLightType(1);
		}

		else if (key == GLFW_KEY_3)
		{
			Mode = SPOTLIGHT;
			if (object_num == 0)
				object1->setLightType(2);
			else if (object_num == 1)
				object2->setLightType(2);
			else if (object_num == 2)
				object3->setLightType(2);
		}

		else if (key == GLFW_KEY_4)
		{
			Mode = NORMALS;
			if (object_num == 0)
				object1->setLightType(3);
			else if (object_num == 1)
				object2->setLightType(3);
			else if (object_num == 2)
				object3->setLightType(3);
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

void Window::resetLight()
{
	if (object_num == 0)
		object1->resetLight();
	else if (object_num == 1)
		object2->resetLight();
	else if (object_num == 2)
		object3->resetLight();
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

void Window::mouseRotateObject(float rot_angle, glm::vec3 rotAxis) {
	if (object_num == 0)
		object1->mouse_rotate(rot_angle, rotAxis);
	else if (object_num == 1)
		object2->mouse_rotate(rot_angle, rotAxis);
	else if (object_num == 2)
		object3->mouse_rotate(rot_angle, rotAxis);
}

void Window::mouseRotateDirLight(float rot_angle, glm::vec3 rotAxis) {
	if (object_num == 0)
		object1->dirLight_rotate(rot_angle, rotAxis);
	else if (object_num == 1)
		object2->dirLight_rotate(rot_angle, rotAxis);
	else if (object_num == 2)
		object3->dirLight_rotate(rot_angle, rotAxis);
}

void Window::mouseRotatePointLight(float rot_angle, glm::vec3 rotAxis) {
	if (object_num == 0)
		object1->pointLight_rotate(rot_angle, rotAxis);
	else if (object_num == 1)
		object2->pointLight_rotate(rot_angle, rotAxis);
	else if (object_num == 2)
		object3->pointLight_rotate(rot_angle, rotAxis);
}

void Window::scrollTranslatePointLight(glm::vec3 transVec) {
	if (object_num == 0)
		object1->pointLight_translate(transVec);
	else if (object_num == 1)
		object2->pointLight_translate(transVec);
	else if (object_num == 2)
		object3->pointLight_translate(transVec);
}

void Window::scrollTranslateSpotLight(glm::vec3 transVec) {
	if (object_num == 0)
		object1->spotLight_translate(transVec);
	else if (object_num == 1)
		object2->spotLight_translate(transVec);
	else if (object_num == 2)
		object3->spotLight_translate(transVec);
}

void Window::mouseRotateSpotLight(float rot_angle, glm::vec3 rotAxis) {
	if (object_num == 0)
		object1->spotLight_rotate(rot_angle, rotAxis);
	else if (object_num == 1)
		object2->spotLight_rotate(rot_angle, rotAxis);
	else if (object_num == 2)
		object3->spotLight_rotate(rot_angle, rotAxis);
}

void Window::changeSpotLightCutOff(float change){
	if (object_num == 0)
		object1->spotLight_changeCutOff(change);
	else if (object_num == 1)
		object2->spotLight_changeCutOff(change);
	else if (object_num == 2)
		object3->spotLight_changeCutOff(change);
}

void Window::changeSpotLightSpotExponent(float change) {
	if (object_num == 0)
		object1->spotLight_changeSpotExponent(change);
	else if (object_num == 1)
		object2->spotLight_changeSpotExponent(change);
	else if (object_num == 2)
		object3->spotLight_changeSpotExponent(change);
}

glm::vec3 Window::getCamPos() {
	return cam_pos;
}