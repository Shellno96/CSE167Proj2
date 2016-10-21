#ifndef _CUBE_H_
#define _CUBE_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
//from proj1
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

//from proj1
#include <vector>
#include <stdio.h>
#include <iostream>

//new
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

using namespace std;

class OBJObject
{
private:

	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::mat4 toWorld;
	glm::mat4 dirLightToWorld;
	glm::mat4 pointLightToWorld;
	glm::mat4 spotLightToWorld;
	glm::mat4 spotLightDirectionToWorld;
	glm::mat4 normalToWorld;

	FILE * fp;     // file pointer
	float x, y, z;  // vertex coordinates
	float r, g, b;  // vertex color
	int c1, c2, c3;    // characters read from file
	int size;
	float maxX, maxY, maxZ, minX, minY, minZ;
	float avgCenterX, avgCenterY, avgCenterZ, avgScaleX, avgScaleY, avgScaleZ;
	float objectSize;

	int lightType = 0;
	
	glm::vec3 dirLightDirection;
	glm::vec3 pointLight;
	glm::vec3 spotLight;
	glm::vec3 spotLightDirection;
	float spotLightCutOff;
	float spotExponent;

	glm::vec3 outDirLightDirection;
	glm::vec3 outPointLight;
	glm::vec3 outSpotLight;
	glm::vec3 outSpotLightDirection;


	glm::vec3 diffuseMaterial;
	glm::vec3 specularMaterial;
	glm::vec3 ambientMaterial;
	float shininessMaterial;

public:
	OBJObject(const char *, vector<glm::vec3>, float);
	~OBJObject();

	void draw(GLuint);
	void update();
	void spin(float);
	void parse1(const char *);
	void parse2(const char *);

	void translate(glm::vec3);
	void resize(float);
	void scale(glm::vec3);
	void orbit(float);
	void reset();
	void mouse_rotate(float deg, glm::vec3 axis);
	void dirLight_rotate(float deg, glm::vec3 axis);
	void pointLight_rotate(float deg, glm::vec3 axis);
	void pointLight_translate(glm::vec3 transVec);
	void spotLight_rotate(float deg, glm::vec3 axis);
	void spotLight_changeCutOff(float change);
	void spotLight_changeSpotExponent(float change);
	void spotLight_translate(glm::vec3 transVec);

	void setLightType(int);

	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec3> getNormals();
	glm::mat4 getToWorld();
	float getPointSize();

	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview, uModelColor, uNormalModelView;
};

#endif

