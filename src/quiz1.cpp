#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\constants.hpp>
#include <cstdlib>
#include <vector>
#include <iostream>

using namespace glm;
using namespace std;


// These consts/variables are for later use
// I disabled the depth test and the cull face in order to show objects that are blocked by others
// mainly the rackets and court that are being enclosed in the blue cube

GLuint VAO, VBO;

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const int GRID_SIZE = 100;
const float GRID_SPACING = 1.0f;


mat4 worldMatrix = mat4(1.0f);
mat4 partMatrix = mat4(1.0f);
mat4 groupMatrix = mat4(1.0f);


int selectedRacket = 0;

vector <mat4> racket1;
mat4 groupRacket1 = mat4(1.0f);

vector <mat4> racket2;
mat4 groupRacket2 = mat4(1.0f);

vector <mat4> racket3;
mat4 groupRacket3 = mat4(1.0f);

vector <mat4> racket4;
mat4 groupRacket4 = mat4(1.0f);

vector <mat4> letterA;
mat4 groupLetterA = mat4(1.0f);

vector <mat4> letterT;
mat4 groupLetterT = mat4(1.0f);

vector <mat4> letterL;
mat4 groupLetterL = mat4(1.0f);

vector <mat4> letterI;
mat4 groupLetterI = mat4(1.0f);


// more camera settings
float cameraSpeed = 1.0f;
float cameraFastSpeed = 2 * cameraSpeed;
float cameraHorizontalAngle = 90.0f;
float cameraVerticalAngle = 0.0f;
float rotationSpeed = 10.0f;




vec3 cameraPosition = vec3(0.5f, 0.5f, 1.3f);
vec3 cameraLookAt = vec3(0.0f, -0.5f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

// functions that deal with inputs/ window resizing

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInputs(GLFWwindow* window, int shaderProgram, float cameraSpeed, float dt);





const char* vertexShaderSource{
	"#version 330 core\n"
	"layout (location = 0) in vec3 pos;"
	"layout (location = 1) in vec3 color;"
	"uniform mat4 viewMatrix;"
	"uniform mat4 worldMatrix= mat4(1.0f);"
	"uniform mat4 projectionMatrix;"
	"out vec3 vertexColor;"
	"void main() {"
	"vertexColor = color;"
	"    gl_Position = projectionMatrix * viewMatrix* worldMatrix* vec4(pos.x, pos.y, pos.z, 1.0f);"
	"}"
};



// I included an if statement to check whether the user has passed in a custom color.
// The if statement checks whether the color boolean has been set to true, if so, it uses the color the user has specified 
// If not, the fragment shader will use the colors found in location 1 - the colored vertices specified in the vec3 arrays

const char* fragmentShaderSource{

	"#version 330 core\n"
	"in vec3 vertexColor;"
	"out vec4 fragColor;"
	"uniform bool objColor;"
	"uniform vec3 objectColor;"
	"void main() {"
	"if (objColor) {"
	"fragColor= vec4(objectColor.r, objectColor.g, objectColor.b, 1.0f);"
	"} else {"
	"fragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
	"}"
	"}"
};



int createCube()
{
	// Cube model

	vec3 vertexArray[] = {
		vec3(-0.5f, -0.5f, -0.5f), vec3(0.48f, 0.83f, 0.97f),
		vec3(-0.5f, -0.5f, 0.5f),  vec3(0.48f, 0.83f, 0.97f),
		vec3(-0.5f, 0.5f, 0.5f),   vec3(0.48f, 0.83f, 0.97f),

		vec3(-0.5f, -0.5f, -0.5f), vec3(0.48f, 0.83f, 0.97f),
		vec3(-0.5f, 0.5f, 0.5f),   vec3(0.48f, 0.83f, 0.97f),
		vec3(-0.5f, 0.5f, -0.5f),  vec3(0.48f, 0.83f, 0.97f),

		vec3(0.5f, 0.5f, -0.5f),   vec3(0.48f, 0.83f, 0.97f),
		vec3(-0.5f, -0.5f, -0.5f), vec3(0.48f, 0.83f, 0.97f),
		vec3(-0.5f, 0.5f, -0.5f),  vec3(0.48f, 0.83f, 0.97f),

		vec3(0.5f, 0.5f, -0.5f),   vec3(0.48f, 0.83f, 0.97f),
		vec3(0.5f, -0.5f, -0.5f),  vec3(0.48f, 0.83f, 0.97f),
		vec3(-0.5f, -0.5f, -0.5f), vec3(0.48f, 0.83f, 0.97f),

		vec3(0.5f, -0.5f, 0.5f),   vec3(0.48f, 0.83f, 0.97f),
		vec3(-0.5f, -0.5f, -0.5f), vec3(0.48f, 0.83f, 0.97f),
		vec3(0.5f, -0.5f, -0.5f),  vec3(0.48f, 0.83f, 0.97f),

		vec3(0.5f, -0.5f, 0.5f),   vec3(0.48f, 0.83f, 0.97f),
		vec3(-0.5f, -0.5f, 0.5f),  vec3(0.48f, 0.83f, 0.97f),
		vec3(-0.5f, -0.5f, -0.5f), vec3(0.48f, 0.83f, 0.97f),

		vec3(-0.5f, 0.5f, 0.5f),   vec3(0.48f, 0.83f, 0.97f),
		vec3(-0.5f, -0.5f, 0.5f),  vec3(0.48f, 0.83f, 0.97f),
		vec3(0.5f, -0.5f, 0.5f),   vec3(0.48f, 0.83f, 0.97f),

		vec3(0.5f, 0.5f, 0.5f),    vec3(0.48f, 0.83f, 0.97f),
		vec3(-0.5f, 0.5f, 0.5f),   vec3(0.48f, 0.83f, 0.97f),
		vec3(0.5f, -0.5f, 0.5f),   vec3(0.48f, 0.83f, 0.97f),

		vec3(0.5f, 0.5f, 0.5f),    vec3(0.48f, 0.83f, 0.97f),
		vec3(0.5f, -0.5f, -0.5f),  vec3(0.48f, 0.83f, 0.97f),
		vec3(0.5f, 0.5f, -0.5f),   vec3(0.48f, 0.83f, 0.97f),

		vec3(0.5f, -0.5f, -0.5f),  vec3(0.48f, 0.83f, 0.97f),
		vec3(0.5f, 0.5f, 0.5f),    vec3(0.48f, 0.83f, 0.97f),
		vec3(0.5f, -0.5f, 0.5f),   vec3(0.48f, 0.83f, 0.97f),

		vec3(0.5f, 0.5f, 0.5f),    vec3(0.48f, 0.83f, 0.97f),
		vec3(0.5f, 0.5f, -0.5f),   vec3(0.48f, 0.83f, 0.97f),
		vec3(-0.5f, 0.5f, -0.5f),  vec3(0.48f, 0.83f, 0.97f),

		vec3(0.5f, 0.5f, 0.5f),    vec3(0.48f, 0.83f, 0.97f),
		vec3(-0.5f, 0.5f, -0.5f),  vec3(0.48f, 0.83f, 0.97f),
		vec3(-0.5f, 0.5f, 0.5f),   vec3(0.48f, 0.83f, 0.97f)

	};


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)sizeof(vec3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	return VAO;
}




int createVertexGrid() {


	vec3 gridVertices[]{
	vec3(0.0f, 0.0f, 0.0f), // Start point
	vec3(0.1f, 0.6f, 0.1f), // color

	vec3(1.0f, 0.0f,  0.0f), // End point
	vec3(0.1f, 0.6f, 0.1f)  // color

	};




	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices), gridVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return VAO;




}


int createNet() {


	vec3 gridVertices[]{
	 vec3(0.0f, 0.0f, 0.0f), // Start point
	 vec3(0.0f, 0.0f, 0.0f), // color

	 vec3(0.0f, 0.5f,  0.0f), // End point
	 vec3(0.0f, 0.0f, 0.0f) // color

	};




	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices), gridVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return VAO;




}


// Here is where the compilation of the shaders happens 

int createShaders() {

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "Vertex shader compilation did not work" << infoLog << endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "Fragment shader compilation did not work" << infoLog << endl;
	}

	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "Shader program compilation did not work" << infoLog << endl;
	}

	glValidateProgram(shaderProgram);
	glGetShaderiv(shaderProgram, GL_VALIDATE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "invalid shader program" << infoLog << endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;


}

// Function for resizing the window 

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

	glViewport(0, 0, width, height);
}



//							START OF CONSTRUCTION 


float rotateBy = 0.05f;
void processInputs(GLFWwindow* window, int shaderProgram, float currentCameraSpeed, float dt) {
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}


	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
	{
		cameraPosition.x -= currentCameraSpeed * dt;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
	{
		cameraPosition.x += currentCameraSpeed * dt;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move camera up
	{
		cameraPosition.y -= currentCameraSpeed * dt;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move camera down
	{
		cameraPosition.y += currentCameraSpeed * dt;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // move camera up
	{
		cameraPosition.z -= currentCameraSpeed * dt;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // move camera down
	{
		cameraPosition.z += currentCameraSpeed * dt;
	}



	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		selectedRacket = 1;
		cameraPosition = vec3(0.3f, 0.5f, 0.3f);
		cameraLookAt = vec3(0.0f, -0.5f, -1.0f);
		cameraUp = vec3(0.0f, 1.0f, 0.0f);
		viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

		// Set the view matrix 
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		selectedRacket = 2;
		cameraPosition = vec3(0.8f, 0.5f, 0.3f);
		cameraLookAt = vec3(0.0f, -0.5f, -1.0f);
		cameraUp = vec3(0.0f, 1.0f, 0.0f);
		viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

		// Set the view matrix 
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		selectedRacket = 3;
		cameraPosition = vec3(0.1f, 0.5f, 1.5f);
		cameraLookAt = vec3(0.0f, -0.5f, -1.0f);
		cameraUp = vec3(0.0f, 1.0f, 0.0f);
		viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

		// Set the view matrix 
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
	}

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		selectedRacket = 4;
		cameraPosition = vec3(0.8f, 0.5f, 2.2f);
		cameraLookAt = vec3(0.0f, -0.5f, -1.0f);
		cameraUp = vec3(0.0f, 1.0f, 0.0f);
		viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

		// Set the view matrix 
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
	}



	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		//GLuint worldLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		if (selectedRacket == 1) {
			rotateBy = 0.3f;
			//	groupRacket1 = translate(groupRacket1, vec3(0.3f, 0.0f, -0.5f));
			//	groupRacket1 = translate(groupRacket1, vec3(0.3f, 0.0f, -0.5f));

				//worked code
			//	groupRacket1 = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));
			//	groupRacket1 = translate(groupRacket1, -vec3(0.3f, 0.0f, -0.5f));


			//	groupRacket1 = translate(groupRacket1, -vec3(0.01f, 0.01f, -0.5f)); // Translate back to the origin
			vec3 handleCenter = vec3(0.3f, 0.09f, -0.5f);
			// 2. Define a translation matrix to move the handle's center point to the origin
			mat4 translateToOrigin = translate(mat4(1.0f), -handleCenter);

			// 3. Perform the rotation around the origin
			mat4 rotationMatrix = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			mat4 translateBack = translate(mat4(1.0f), handleCenter);

			// Combine the transformations in the correct order
			groupRacket1 = translateBack * rotationMatrix * translateToOrigin * groupRacket1;



			vec3 handleCenterLetter = vec3(0.3f, 0.09f, -0.5f);
			// 2. Define a translation matrix to move the handle's center point to the origin
			mat4 translateToOriginLetter = translate(mat4(1.0f), -handleCenterLetter);

			// 3. Perform the rotation around the origin
			rotationMatrix = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			translateBack = translate(mat4(1.0f), handleCenterLetter);

			// Combine the transformations in the correct order
			groupLetterA = translateBack * rotationMatrix * translateToOrigin * groupLetterA;

			//	groupLetterA = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));

			rotateBy += 0.3f;
		}

		else if (selectedRacket == 2) {
			rotateBy = 0.3f;
			vec3 handleCenter = vec3(0.8f, 0.09f, -0.5f);


			mat4 translateToOrigin = translate(mat4(1.0f), -handleCenter);

			// 3. Perform the rotation around the origin
			mat4 rotationMatrix = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			mat4 translateBack = translate(mat4(1.0f), handleCenter);

			// Combine the transformations in the correct order
			groupRacket2 = translateBack * rotationMatrix * translateToOrigin * groupRacket2;


			//	groupRacket2 = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));
			//	groupLetterT = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));



			vec3 handleCenterLetter = vec3(0.8f, 0.09f, -0.5f);
			// 2. Define a translation matrix to move the handle's center point to the origin
			mat4 translateToOriginLetter = translate(mat4(1.0f), -handleCenterLetter);

			// 3. Perform the rotation around the origin
			rotationMatrix = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			translateBack = translate(mat4(1.0f), handleCenterLetter);

			// Combine the transformations in the correct order
			groupLetterT = translateBack * rotationMatrix * translateToOrigin * groupLetterT;



			rotateBy += 0.3f;
		}

		else if (selectedRacket == 3) {
			rotateBy = 0.3f;
			vec3 handleCenter = vec3(0.1f, 0.09f, 0.8f);
			//	groupRacket3 = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));



			mat4 translateToOrigin = translate(mat4(1.0f), -handleCenter);

			// 3. Perform the rotation around the origin
			mat4 rotationMatrix = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			mat4 translateBack = translate(mat4(1.0f), handleCenter);

			// Combine the transformations in the correct order
			groupRacket3 = translateBack * rotationMatrix * translateToOrigin * groupRacket3;


			//	groupLetterL = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));




			vec3 handleCenterLetter = vec3(0.1f, 0.09f, 0.8f);
			// 2. Define a translation matrix to move the handle's center point to the origin
			mat4 translateToOriginLetter = translate(mat4(1.0f), -handleCenterLetter);

			// 3. Perform the rotation around the origin
			rotationMatrix = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			translateBack = translate(mat4(1.0f), handleCenterLetter);

			// Combine the transformations in the correct order
			groupLetterL = translateBack * rotationMatrix * translateToOrigin * groupLetterL;

			rotateBy += 0.3f;
		}

		else if (selectedRacket == 4) {
			rotateBy = 0.3f;
			vec3 handleCenter = vec3(0.8f, 0.09f, 1.5f);
			//	groupRacket4 = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));


			mat4 translateToOrigin = translate(mat4(1.0f), -handleCenter);

			// 3. Perform the rotation around the origin
			mat4 rotationMatrix = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			mat4 translateBack = translate(mat4(1.0f), handleCenter);

			// Combine the transformations in the correct order
			groupRacket4 = translateBack * rotationMatrix * translateToOrigin * groupRacket4;


			//groupLetterI = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));

			vec3 handleCenterLetter = vec3(0.8f, 0.09f, 1.5f);
			// 2. Define a translation matrix to move the handle's center point to the origin
			mat4 translateToOriginLetter = translate(mat4(1.0f), -handleCenterLetter);

			// 3. Perform the rotation around the origin
			rotationMatrix = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			translateBack = translate(mat4(1.0f), handleCenterLetter);

			// Combine the transformations in the correct order
			groupLetterI = translateBack * rotationMatrix * translateToOrigin * groupLetterI;


			rotateBy += 0.3f;
		}

		else {
			cout << "No tennis racket is selected" << endl;
		}
	}


	// The same issue I discussed above happens here
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	{
		//GLuint worldLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		if (selectedRacket == 1) {
			rotateBy = 0.3f;
			//	groupRacket1 = translate(groupRacket1, vec3(0.3f, 0.0f, -0.5f));
			//	groupRacket1 = translate(groupRacket1, vec3(0.3f, 0.0f, -0.5f));

				//worked code
			//	groupRacket1 = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));
			//	groupRacket1 = translate(groupRacket1, -vec3(0.3f, 0.0f, -0.5f));


			//	groupRacket1 = translate(groupRacket1, -vec3(0.01f, 0.01f, -0.5f)); // Translate back to the origin
			vec3 handleCenter = vec3(0.3f, 0.09f, -0.5f);
			// 2. Define a translation matrix to move the handle's center point to the origin
			mat4 translateToOrigin = translate(mat4(1.0f), -handleCenter);

			// 3. Perform the rotation around the origin
			mat4 rotationMatrix = rotate(mat4(1.0f), radians(rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			mat4 translateBack = translate(mat4(1.0f), handleCenter);

			// Combine the transformations in the correct order
			groupRacket1 = translateBack * rotationMatrix * translateToOrigin * groupRacket1;



			vec3 handleCenterLetter = vec3(0.3f, 0.09f, -0.5f);
			// 2. Define a translation matrix to move the handle's center point to the origin
			mat4 translateToOriginLetter = translate(mat4(1.0f), -handleCenterLetter);

			// 3. Perform the rotation around the origin
			rotationMatrix = rotate(mat4(1.0f), radians(rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			translateBack = translate(mat4(1.0f), handleCenterLetter);

			// Combine the transformations in the correct order
			groupLetterA = translateBack * rotationMatrix * translateToOrigin * groupLetterA;

			//	groupLetterA = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));

			rotateBy += 0.3f;
		}

		else if (selectedRacket == 2) {
			rotateBy = 0.3f;
			vec3 handleCenter = vec3(0.8f, 0.09f, -0.5f);


			mat4 translateToOrigin = translate(mat4(1.0f), -handleCenter);

			// 3. Perform the rotation around the origin
			mat4 rotationMatrix = rotate(mat4(1.0f), radians(rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			mat4 translateBack = translate(mat4(1.0f), handleCenter);

			// Combine the transformations in the correct order
			groupRacket2 = translateBack * rotationMatrix * translateToOrigin * groupRacket2;


			//	groupRacket2 = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));
			//	groupLetterT = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));



			vec3 handleCenterLetter = vec3(0.8f, 0.09f, -0.5f);
			// 2. Define a translation matrix to move the handle's center point to the origin
			mat4 translateToOriginLetter = translate(mat4(1.0f), -handleCenterLetter);

			// 3. Perform the rotation around the origin
			rotationMatrix = rotate(mat4(1.0f), radians(rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			translateBack = translate(mat4(1.0f), handleCenterLetter);

			// Combine the transformations in the correct order
			groupLetterT = translateBack * rotationMatrix * translateToOrigin * groupLetterT;



			rotateBy += 0.3f;
		}

		else if (selectedRacket == 3) {
			rotateBy = 0.3f;
			vec3 handleCenter = vec3(0.1f, 0.09f, 0.8f);
			//	groupRacket3 = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));



			mat4 translateToOrigin = translate(mat4(1.0f), -handleCenter);

			// 3. Perform the rotation around the origin
			mat4 rotationMatrix = rotate(mat4(1.0f), radians(rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			mat4 translateBack = translate(mat4(1.0f), handleCenter);

			// Combine the transformations in the correct order
			groupRacket3 = translateBack * rotationMatrix * translateToOrigin * groupRacket3;


			//	groupLetterL = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));




			vec3 handleCenterLetter = vec3(0.1f, 0.09f, 0.8f);
			// 2. Define a translation matrix to move the handle's center point to the origin
			mat4 translateToOriginLetter = translate(mat4(1.0f), -handleCenterLetter);

			// 3. Perform the rotation around the origin
			rotationMatrix = rotate(mat4(1.0f), radians(rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			translateBack = translate(mat4(1.0f), handleCenterLetter);

			// Combine the transformations in the correct order
			groupLetterL = translateBack * rotationMatrix * translateToOrigin * groupLetterL;

			rotateBy += 0.3f;
		}

		else if (selectedRacket == 4) {
			rotateBy = 0.3f;
			vec3 handleCenter = vec3(0.8f, 0.09f, 1.5f);
			//	groupRacket4 = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));


			mat4 translateToOrigin = translate(mat4(1.0f), -handleCenter);

			// 3. Perform the rotation around the origin
			mat4 rotationMatrix = rotate(mat4(1.0f), radians(rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			mat4 translateBack = translate(mat4(1.0f), handleCenter);

			// Combine the transformations in the correct order
			groupRacket4 = translateBack * rotationMatrix * translateToOrigin * groupRacket4;


			//groupLetterI = rotate(mat4(1.0f), radians(-rotateBy), vec3(0.0f, 1.0f, 0.0f));

			vec3 handleCenterLetter = vec3(0.8f, 0.09f, 1.5f);
			// 2. Define a translation matrix to move the handle's center point to the origin
			mat4 translateToOriginLetter = translate(mat4(1.0f), -handleCenterLetter);

			// 3. Perform the rotation around the origin
			rotationMatrix = rotate(mat4(1.0f), radians(rotateBy), vec3(0.0f, 1.0f, 0.0f));

			// 4. Apply the inverse translation matrix
			translateBack = translate(mat4(1.0f), handleCenterLetter);

			// Combine the transformations in the correct order
			groupLetterI = translateBack * rotationMatrix * translateToOrigin * groupLetterI;


			rotateBy += 0.3f;
		}

		else {
			cout << "No tennis racket is selected" << endl;
		}
	}


	mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

	// Set the view matrix 

	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));

	mat4 projectionMatrix = glm::perspective(70.0f, 1024.0f / 768.0f, 0.1f, 100.0f);

	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(projectionMatrix));

}






//					END OF CONSTRUCTION 


int main() {

	// Here is where we initialize GLFW and its versions 

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	// Here is where we initialize the window object and set the properties

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Quiz 1", NULL, NULL);

	if (window == NULL) {
		cout << "Window creation has failed." << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);



	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		cout << "Error. Glew could not be initialized." << endl;
		glfwTerminate();
		return -1;
	}


	// Setting the color to clear the screen 
	glClearColor(0.13f, 0.23f, 0.24f, 0.50f);



	int shaderProgram = createShaders();
	glUseProgram(shaderProgram);



	// Setting the camera and view / projection matrices

	vec3 cameraPosition = vec3(0.5f, 0.5f, 1.3f);
	vec3 cameraLookAt = vec3(0.0f, -0.5f, -1.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
	mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

	// Setting the view matrix
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));

	float aspectRatio = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);

	mat4 projectionMatrix = glm::perspective(70.0f, 1024.0f / 768.0f, 0.1f, 100.0f);


	float lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);



	// Setting the projection matrix
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(projectionMatrix));


	GLuint modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	mat4 modelMatrix = mat4(1.0f);

	GLuint worldLocation = glGetUniformLocation(shaderProgram, "worldMatrix");


	// This location is for the color uniform mentioned in the vertex shader
	// As I said before, the user can specify what colors they want themselves using this color location 
	GLuint colorlLocation = glGetUniformLocation(shaderProgram, "objectColor");




	// more camera settings
	float cameraSpeed = 1.0f;
	float cameraFastSpeed = 2 * cameraSpeed;
	float cameraHorizontalAngle = 90.0f;
	float cameraVerticalAngle = 0.0f;
	float angle = 0;
	float rotationSpeed = 10.0f;


	// creating the VAOs for the separate object models

	int cubeVAO = createCube();
	int GridVAO = createVertexGrid();
	int netVAO = createNet();


	// setting the default value for the bool color in the vertex shader. 
	// by making it false, we make sure that the default setting is that the 
	// vertices found in location 1 will be used for coloring
	bool colorBool = false;

	// getting location of the bool color in order to set it later 
	GLuint boolColorLocation = glGetUniformLocation(shaderProgram, "objColor");

	//setting it to false
	glUniform1i(boolColorLocation, colorBool);




	// Start of render loop
	while (!glfwWindowShouldClose(window)) {
		//worldMatrix = mat4(1.0f);
		glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
		racket1.clear();
		racket2.clear();
		racket3.clear();
		racket4.clear();

		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;
		angle = (angle + rotationSpeed * dt);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);





		// This is the construction of the sky - blue cube model, which will be enclosing our scene
		// when transforming objects, I am updating the transformation info by setting the result
		// in the worldMatrix in the vertex shader

		glBindVertexArray(cubeVAO);

		mat4 cubePartMatrix = translate(mat4(1.0f), vec3(0.4f, 0.0f, -1.95f)) * scale(mat4(1.0f), vec3(2.2f, 2.f, 7.85f));
		worldMatrix = groupMatrix * cubePartMatrix;
		glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
		glDrawArrays(GL_TRIANGLES, 0, 36);





		//	Here is the creation of the grid 

		// we first unbind then bind the needed VAO to create the grid 
		glBindVertexArray(0);
		glBindVertexArray(GridVAO);

		for (int i = -78 / 2; i <= 78 / 2; i++) {
			//horizontal lines 
			mat4 gridPartMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, i * 0.05f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.5f));
			worldMatrix = groupMatrix * gridPartMatrix;
			glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
			glDrawArrays(GL_LINES, 0, 2);


		}

		for (int i = -36 / 2; i <= 36 / 2; i++) {
			//vertical lines
			mat4 gridPartMatrix = translate(mat4(1.0f), vec3(i * 0.028f + 0.5f, 0.0f, -1.95f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(-3.9f, 1.0f, 1.5f));
			worldMatrix = groupMatrix * gridPartMatrix;
			glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
			glDrawArrays(GL_LINES, 0, 2);
		}

		glBindVertexArray(0);




		// This section takes care of the tennis court: the court itself and net
		// This is the tennis court floor 

		glBindVertexArray(cubeVAO);
		mat4 partMatrix = translate(mat4(1.0f), vec3(0.5f, -0.05f, -0.36f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(4.5f, 0.005f, 1.099f));
		worldMatrix = groupMatrix * partMatrix;
		glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(worldMatrix));

		// As you can see, we are binding the true value to the boolean in order to let the vertex shader
		// to take the user specified color 
		glUniform1i(boolColorLocation, 1);

		// this is the user specified color (white)
		glUniform3fv(colorlLocation, 1, value_ptr(vec3(0.0f, 0.38f, 0.0f)));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// These two objects below are the white stripes in the tennis court 

		cubePartMatrix = translate(mat4(1.0f), vec3(0.5f, 0.0f, -1.0f)) * scale(mat4(1.0f), vec3(0.8f, 0.01f, 0.01f));
		worldMatrix = groupMatrix * cubePartMatrix;
		glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
		glUniform3fv(colorlLocation, 1, value_ptr(vec3(1.0f, 1.f, 1.0f)));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		cubePartMatrix = translate(mat4(1.0f), vec3(0.5f, 0.0f, 1.3f)) * scale(mat4(1.0f), vec3(0.8f, 0.01f, 0.01f));
		worldMatrix = groupMatrix * cubePartMatrix;
		glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
		glUniform3fv(colorlLocation, 1, value_ptr(vec3(1.0f, 1.f, 1.0f)));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// unbind and set the bool color back to the default (false)
		// until we set it to true again when we specify a color 
		// otherwise, the vertex shader will just use the colored vertices in the VAO arrays
		glBindVertexArray(0);
		glUniform1i(boolColorLocation, 0);






		// Here is the construction of the tennis court net 
		glBindVertexArray(netVAO);

		for (int i = -6 / 2; i <= 6 / 2; i++) {

			mat4 netPartMatrix = translate(mat4(1.0f), vec3(0.0f, -i * 0.0249f + 0.074f, 0.6f)) * rotate(mat4(1.0f), radians(-90.0f), vec3(.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(4.0f, 2.009f, 3.0f));
			worldMatrix = groupMatrix * netPartMatrix;
			glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
			glDrawArrays(GL_LINES, 0, 2);

		}

		for (int i = -36 / 2; i <= 36 / 2; i++) {

			mat4 netPartMatrix = translate(mat4(1.0f), vec3(i * 0.028f + 0.5f, 0.0f, 0.6f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(-3.9f, 0.3f, 2.5f));
			worldMatrix = groupMatrix * netPartMatrix;
			glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
			glDrawArrays(GL_LINES, 0, 2);
		}

		glBindVertexArray(0);






		// This is where it starts to get a bit messy
		// Below, I start to construct the tennis rackets 
		// Instead of a for loop, I decided to manually construct all 4 
		// I understand it is not the best choice but I felt limited when passing the coordinates, which is why
		bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
		float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;

		processInputs(window, shaderProgram, currentCameraSpeed, dt);


		//	RACKET 1:
		//  Everything below relates to the different parts of the racket: 
		// handle, net, corners....

		// The racket stick model:
		glBindVertexArray(cubeVAO);
		mat4 T1PartMatrix = translate(mat4(1.0f), vec3(0.3f, 0.0f, -0.5f)) * scale(mat4(1.0f), vec3(0.01f, 0.13f, 0.02f));
		racket1.push_back(T1PartMatrix);

		// the small white corners:
		mat4 RightC1PartMatrix = translate(mat4(1.0f), vec3(0.32f, 0.08f, -0.5f)) * rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.06f, 0.02f));
		racket1.push_back(RightC1PartMatrix);


		mat4 LeftC1PartMatrix = translate(mat4(1.0f), vec3(0.28f, 0.08f, -0.5f)) * rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.06f, 0.02f));
		racket1.push_back(LeftC1PartMatrix);

		//  the red corners:
		mat4 RightR1PartMatrix = translate(mat4(1.0f), vec3(0.34f, 0.13f, -0.5f)) * scale(mat4(1.0f), vec3(0.01f, 0.07f, 0.02f));
		racket1.push_back(RightR1PartMatrix);

		mat4 LeftR1PartMatrix = translate(mat4(1.0f), vec3(0.26f, 0.13f, -0.5f)) * scale(mat4(1.0f), vec3(0.01f, 0.07f, 0.02f));
		racket1.push_back(LeftR1PartMatrix);

		// the small upper white corners:
		mat4 RightC2PartMatrix = translate(mat4(1.0f), vec3(0.331f, 0.179f, -0.5f)) * rotate(mat4(1.0f), radians(40.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.03f, 0.02f));
		racket1.push_back(RightC2PartMatrix);

		mat4 LeftC2PartMatrix = translate(mat4(1.0f), vec3(0.269f, 0.179f, -0.5f)) * rotate(mat4(1.0f), radians(-40.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.03f, 0.02f));
		racket1.push_back(LeftC2PartMatrix);

		// middle red tube at the top:
		mat4 Middle1PartMatrix = translate(mat4(1.0f), vec3(0.30f, 0.19f, -0.5f)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.045f, 0.02f));
		racket1.push_back(Middle1PartMatrix);


		// The racket net:
		mat4 Net1PartMatrix = translate(mat4(1.0f), vec3(0.30f, 0.14f, -0.5f)) * scale(mat4(1.0f), vec3(0.0645f, 0.075f, 0.02f));
		racket1.push_back(Net1PartMatrix);



		// Drawing each object of raket1
		// The indexes are used to identify which object we are using
		// from the index, we can know which color to pass on to the shader
		// since the objects are stored in a pre-defined way (handle, lower corners, middle corners, upper corners, middle tube, net)
		// I know which specific indexes should have which color 

		GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
		glUniform1i(boolColorLocation, 1);
		int index = 0;

		for (mat4& element : racket1) {
			//	for (int i = 0; i < 4; ++i) {
				//	for (int j = 0; j < 4; ++j) {
						// Use std::setw to set the field width for each element
					//	std::cout << "output: " << groupMatrix[i][j] << " " << endl;
				//	}
				//	std::cout << std::endl;
				//}
			worldMatrix = groupRacket1 * element;
			
			glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
			if (index == 0 || index == 3 || index == 4 || index == 7) {
				glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 0.0f, 0.0f)));
			}

			if (index == 1 || index == 2 || index == 5 || index == 6) {
				glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
			}

			if (index == 8) {
				glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 1.0f, 0.0f)));
			}

			glDrawArrays(GL_TRIANGLES, 0, 36);
			index++;
		}






		// Below will be the same code as before since I am manually constructing all 4 rackets

		//			RACKET 2:

		//	racket handle:
		glBindVertexArray(cubeVAO);
		mat4 T2PartMatrix = translate(mat4(1.0f), vec3(0.8f, 0.0f, -0.5f)) * scale(mat4(1.0f), vec3(0.01f, 0.13f, 0.02f));
		racket2.push_back(T2PartMatrix);


		// small white corners:
		mat4 RightC21PartMatrix = translate(mat4(1.0f), vec3(0.82f, 0.08f, -0.5f)) * rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.06f, 0.02f));
		racket2.push_back(RightC21PartMatrix);

		mat4 LeftC21PartMatrix = translate(mat4(1.0f), vec3(0.78f, 0.08f, -0.5f)) * rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.06f, 0.02f));
		racket2.push_back(LeftC21PartMatrix);


		// red corners: 
		mat4 RightR2PartMatrix = translate(mat4(1.0f), vec3(0.84f, 0.13f, -0.5f)) * scale(mat4(1.0f), vec3(0.01f, 0.07f, 0.02f));
		racket2.push_back(RightR2PartMatrix);


		mat4 LeftR2PartMatrix = translate(mat4(1.0f), vec3(0.76f, 0.13f, -0.5f)) * scale(mat4(1.0f), vec3(0.01f, 0.07f, 0.02f));
		racket2.push_back(LeftR2PartMatrix);



		// upper white corners:
		mat4 RightC22PartMatrix = translate(mat4(1.0f), vec3(0.831f, 0.179f, -0.5f)) * rotate(mat4(1.0f), radians(40.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.03f, 0.02f));
		racket2.push_back(RightC22PartMatrix);

		mat4 LeftC22PartMatrix = translate(mat4(1.0f), vec3(0.769f, 0.179f, -0.5f)) * rotate(mat4(1.0f), radians(-40.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.03f, 0.02f));
		racket2.push_back(LeftC22PartMatrix);



		// upper red tube:
		mat4 Middle2PartMatrix = translate(mat4(1.0f), vec3(0.80f, 0.19f, -0.5f)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.045f, 0.02f));
		racket2.push_back(Middle2PartMatrix);



		// racket net:
		mat4 Net2PartMatrix = translate(mat4(1.0f), vec3(0.80f, 0.14f, -0.5f)) * scale(mat4(1.0f), vec3(0.0645f, 0.075f, 0.02f));
		racket2.push_back(Net2PartMatrix);


		index = 0;
		for (mat4& element : racket2) {

			worldMatrix = groupRacket2 * element;

			glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
			if (index == 0 || index == 3 || index == 4 || index == 7) {
				glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 0.0f, 0.0f)));
			}

			if (index == 1 || index == 2 || index == 5 || index == 6) {
				glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
			}

			if (index == 8) {
				glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 1.0f, 0.0f)));
			}

			glDrawArrays(GL_TRIANGLES, 0, 36);
			index++;
		}




		//			RACKET 3:
		//	racket handle:
		glBindVertexArray(cubeVAO);
		mat4 T3PartMatrix = translate(mat4(1.0f), vec3(0.1f, 0.0f, 0.8f)) * scale(mat4(1.0f), vec3(0.01f, 0.13f, 0.02f));
		racket3.push_back(T3PartMatrix);


		// small white corners:
		mat4 RightC31PartMatrix = translate(mat4(1.0f), vec3(0.12f, 0.08f, 0.8f)) * rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.06f, 0.02f));
		racket3.push_back(RightC31PartMatrix);


		mat4 leftC31PartMatrix = translate(mat4(1.0f), vec3(0.08f, 0.08f, 0.8f)) * rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.06f, 0.02f));
		racket3.push_back(leftC31PartMatrix);

		// red corners: 
		mat4 RightR31PartMatrix = translate(mat4(1.0f), vec3(0.14f, 0.13f, 0.8f)) * scale(mat4(1.0f), vec3(0.01f, 0.07f, 0.02f));
		racket3.push_back(RightR31PartMatrix);

		mat4 LeftR31PartMatrix = translate(mat4(1.0f), vec3(0.06f, 0.13f, 0.8f)) * scale(mat4(1.0f), vec3(0.01f, 0.07f, 0.02f));
		racket3.push_back(LeftR31PartMatrix);



		// small upper white corners:
		mat4 RightC32PartMatrix = translate(mat4(1.0f), vec3(0.131f, 0.179f, 0.8f)) * rotate(mat4(1.0f), radians(40.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.03f, 0.02f));
		racket3.push_back(RightC32PartMatrix);

		mat4 LeftC32PartMatrix = translate(mat4(1.0f), vec3(0.069f, 0.179f, 0.8f)) * rotate(mat4(1.0f), radians(-40.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.03f, 0.02f));
		racket3.push_back(LeftC32PartMatrix);


		// upper red tube: 
		mat4 Middle3PartMatrix = translate(mat4(1.0f), vec3(0.10f, 0.19f, 0.8f)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.045f, 0.02f));
		racket3.push_back(Middle3PartMatrix);



		// racket net:
		mat4 Net3PartMatrix = translate(mat4(1.0f), vec3(0.10f, 0.14f, 0.8f)) * scale(mat4(1.0f), vec3(0.0645f, 0.075f, 0.02f));
		racket3.push_back(Net3PartMatrix);


		index = 0;
		for (mat4& element : racket3) {

			worldMatrix = groupRacket3 * element;

			glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
			if (index == 0 || index == 3 || index == 4 || index == 7) {
				glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 0.0f, 0.0f)));
			}

			if (index == 1 || index == 2 || index == 5 || index == 6) {
				glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
			}

			if (index == 8) {
				glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 1.0f, 0.0f)));
			}

			glDrawArrays(GL_TRIANGLES, 0, 36);
			index++;
		}



		//			RACKET 4:
		//	 tennis handle:
		glBindVertexArray(cubeVAO);
		mat4 T4PartMatrix = translate(mat4(1.0f), vec3(0.8f, 0.0f, 1.5f)) * scale(mat4(1.0f), vec3(0.01f, 0.13f, 0.02f));
		racket4.push_back(T4PartMatrix);

		// small white corners:
		mat4 RightC41PartMatrix = translate(mat4(1.0f), vec3(0.82f, 0.08f, 1.5f)) * rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.06f, 0.02f));
		racket4.push_back(RightC41PartMatrix);

		mat4 LeftC41PartMatrix = translate(mat4(1.0f), vec3(0.78f, 0.08f, 1.5f)) * rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.06f, 0.02f));
		racket4.push_back(LeftC41PartMatrix);

		// red corners:
		mat4 RightR41PartMatrix = translate(mat4(1.0f), vec3(0.84f, 0.13f, 1.5f)) * scale(mat4(1.0f), vec3(0.01f, 0.07f, 0.02f));
		racket4.push_back(RightR41PartMatrix);

		mat4 LeftR41PartMatrix = translate(mat4(1.0f), vec3(0.76f, 0.13f, 1.5f)) * scale(mat4(1.0f), vec3(0.01f, 0.07f, 0.02f));
		racket4.push_back(LeftR41PartMatrix);


		// small upper white corners:
		mat4 RightC42PartMatrix = translate(mat4(1.0f), vec3(0.831f, 0.179f, 1.5f)) * rotate(mat4(1.0f), radians(40.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.03f, 0.02f));
		racket4.push_back(RightC42PartMatrix);

		mat4 LeftC42PartMatrix = translate(mat4(1.0f), vec3(0.769f, 0.179f, 1.5f)) * rotate(mat4(1.0f), radians(-40.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.03f, 0.02f));
		racket4.push_back(LeftC42PartMatrix);


		// upper red tube:
		mat4 Middle4PartMatrix = translate(mat4(1.0f), vec3(0.80f, 0.19f, 1.5f)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.045f, 0.02f));
		racket4.push_back(Middle4PartMatrix);

		// racket net:
		mat4 Net4PartMatrix = translate(mat4(1.0f), vec3(0.80f, 0.14f, 1.5f)) * scale(mat4(1.0f), vec3(0.0645f, 0.075f, 0.02f));
		racket4.push_back(Net4PartMatrix);


		index = 0;
		for (mat4& element : racket4) {

			worldMatrix = groupRacket4 * element;

			glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
			if (index == 0 || index == 3 || index == 4 || index == 7) {
				glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 0.0f, 0.0f)));
			}

			if (index == 1 || index == 2 || index == 5 || index == 6) {
				glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
			}

			if (index == 8) {
				glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 1.0f, 0.0f)));
			}

			glDrawArrays(GL_TRIANGLES, 0, 36);
			index++;
		}

		// Here is where the characters start to get rendered
		// I have letters A, T, L and I

		// LETTER A 

		// The for loop is to construct the same letter 3 times, but with different z values and different color values 
		// space is for incrementing the coordinates 
		// color is for changing the colors of the same letter 

		float space = 0.01f;
		float color = 0.2f;

		for (int i = 0; i <= 3; i++) {

			letterA.clear();
			letterT.clear();
			letterL.clear();
			letterI.clear();

			glBindVertexArray(cubeVAO);

			// left part of A
			mat4 LeftAPartMatrix = translate(mat4(1.0f), vec3(0.27f, 0.4f, -0.5f + space)) * rotate(mat4(1.0f), radians(-24.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.11f, 0.02f));
			letterA.push_back(LeftAPartMatrix);

			// right part of A
			mat4 RightAPartMatrix = translate(mat4(1.0f), vec3(0.312f, 0.4f, -0.5f + space)) * rotate(mat4(1.0f), radians(24.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.11f, 0.02f));
			letterA.push_back(RightAPartMatrix);


			// middle line of A
			mat4 MiddleAPartMatrix = translate(mat4(1.0f), vec3(0.29f, 0.4f, -0.5f + space)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.049f, 0.02f));
			letterA.push_back(MiddleAPartMatrix);

			for (mat4& element : letterA) {
				worldMatrix = groupLetterA * element;
				glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
				glUniform3fv(colorLocation, 1, value_ptr(vec3(0.8f, 0.44f + (color * i), 0.69f)));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}


			//	LETTER T 
			// upright line of T: 
			mat4 TPartMatrix = translate(mat4(1.0f), vec3(0.8f, 0.4f, -0.5f + space)) * scale(mat4(1.0f), vec3(0.01f, 0.11f, 0.02f));
			letterT.push_back(TPartMatrix);

			// the horizontal line of T:
			mat4 MiddleTPartMatrix = translate(mat4(1.0f), vec3(0.8f, 0.4f + 0.05f, -0.5f + space)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.11f, 0.02f));
			letterT.push_back(MiddleTPartMatrix);

			for (mat4& element : letterT) {
				worldMatrix = groupLetterT * element;
				glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
				glUniform3fv(colorLocation, 1, value_ptr(vec3(0.19f + 0.5 + (color * i), 0.6f, 0.3f)));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			// LETTER L 
			// vertical line of L: 
			mat4 LPartMatrix = translate(mat4(1.0f), vec3(0.1f, 0.4f, 0.8f + space)) * scale(mat4(1.0f), vec3(0.01f, 0.11f, 0.02f));
			letterL.push_back(LPartMatrix);

			// horizontal line of L: 
			mat4 HoriLPartMatrix = translate(mat4(1.0f), vec3(0.1f + 0.04f, 0.35f, 0.8f + space)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.087f, 0.02f));
			letterL.push_back(HoriLPartMatrix);

			for (mat4& element : letterL) {
				worldMatrix = groupLetterL * element;
				glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
				glUniform3fv(colorLocation, 1, value_ptr(vec3(0.54f, 0.01f + (color * i), 0.65f)));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			//	LETTER I
			// vertical part of I:
			mat4 IPartMatrix = translate(mat4(1.0f), vec3(0.8f, 0.4f, 1.5f + space)) * scale(mat4(1.0f), vec3(0.01f, 0.11f, 0.02f));
			letterI.push_back(IPartMatrix);

			// upper part of I:
			mat4 UpIPartMatrix = translate(mat4(1.0f), vec3(0.8f, 0.4f + 0.05f, 1.5f + space)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.08f, 0.02f));
			letterI.push_back(UpIPartMatrix);

			// lower part of I:
			mat4 BelowIPartMatrix = translate(mat4(1.0f), vec3(0.8f, 0.35f, 1.5f + space)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.01f, 0.08f, 0.02f));
			letterI.push_back(BelowIPartMatrix);

			for (mat4& element : letterI) {
				worldMatrix = groupLetterI * element;
				glUniformMatrix4fv(worldLocation, 1, GL_FALSE, value_ptr(worldMatrix));
				glUniform3fv(colorLocation, 1, value_ptr(vec3(0.3f + (color * i), 0.47f, 1.0f)));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			// I increment space each loop
			space += 0.01;
		}


		// setting bool color to false again
		glUniform1i(boolColorLocation, 0);
		glBindVertexArray(0);


		// double buffer
		glfwSwapBuffers(window);
		glfwPollEvents();

		// camera movement
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
		{
			cameraPosition.x -= currentCameraSpeed * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
		{
			cameraPosition.x += currentCameraSpeed * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move camera up
		{
			cameraPosition.y -= currentCameraSpeed * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move camera down
		{
			cameraPosition.y += currentCameraSpeed * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // move camera up
		{
			cameraPosition.z -= currentCameraSpeed * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // move camera down
		{
			cameraPosition.z += currentCameraSpeed * dt;
		}
	}


	glfwTerminate();
	return 0;
}
