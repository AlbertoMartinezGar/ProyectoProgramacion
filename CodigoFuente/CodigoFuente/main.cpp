#include <stdio.h>
#include <stdlib.h>
#include <glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "Lector.h"
#include <ctime>

#define NumAtributos 3
#define NumVertices 9
#define Altura 900
#define Ancho 1080
#define Velocidad 0.001f

using namespace glm;

GLFWwindow* window;

mat4x4 mv_matrix;
mat4x4 view_matrix;
mat4x4 proj_matrix;

int InitWindow();
void Matrices(vec3 PosActual, float pitch, float yaw);
void CalculaTamaño(bool UV, int numCaras, int*tam, int*avanza);
void PresionTecla(vec3*PosActual, float*pitch, float*yaw, vec3 VectorDir, vec3 VectorDirLados);
mat4 MatrizVista(vec3 eye, float pitch, float yaw);
void Mouse(float* pitch, float* yaw, double xpos, double ypos, double* xpos2, double* ypos2, vec3*VectorDir, vec3*VectorDirLados);

int main(void)
{
	float yaw = 0.0, pitch = 0.0;
	double xpos = 0, ypos = 0, xpos2 = 0, ypos2 = 0;
	vec3 VectorDireccion, VectorDireccionLados;
	vec3 PosActual = { 0.0, 0.0, 0.0 };

	if (InitWindow() < 0) return -1;

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID =
		LoadShaders("C:\\Users\\erick\\Desktop\\Sem VI\\Programacion de Videojuegos\\Shaders\\shaders\\VS2.glsl",
			"C:\\Users\\erick\\Desktop\\Sem VI\\Programacion de Videojuegos\\Shaders\\shaders\\FS2.glsl");

	GLuint programID2 =
		LoadShaders("C:\\Users\\erick\\Desktop\\Sem VI\\Programacion de Videojuegos\\Shaders\\shaders\\VS2.glsl",
			"C:\\Users\\erick\\Desktop\\Sem VI\\Programacion de Videojuegos\\Shaders\\shaders\\FSS.glsl");

	int numCaras, tam, avance, numCaras2, tam2, avance2;
	bool UV_Detector = false, UV_Detector2 = false;

	const GLfloat* g_vertex_buffer_data = AbreArchivo("C:\\Users\\erick\\Desktop\\Sem VI\\Programacion de Videojuegos\\ObjetosShaders\\cube.obj", &numCaras, &UV_Detector);
	const GLfloat* g_vertex_buffer_data2 = AbreArchivo("C:\\Users\\erick\\Desktop\\Sem VI\\Programacion de Videojuegos\\ObjetosShaders\\roca2.obj", &numCaras2, &UV_Detector2);

	// Se calcula el tamaño y avanza dependiendo de si se encontraron uv
	CalculaTamaño(UV_Detector, numCaras, &tam, &avance);
	CalculaTamaño(UV_Detector2, numCaras2, &tam2, &avance2);

	//Genereacion de buffers
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);

	GLuint vertexbuffer2;
	glGenBuffers(1, &vertexbuffer2);

	//buffer de vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, tam, g_vertex_buffer_data, GL_STATIC_DRAW);

	//Buffer figura 2
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glBufferData(GL_ARRAY_BUFFER, tam2, g_vertex_buffer_data2, GL_STATIC_DRAW);

	clock_t inicio, final;
	inicio = clock();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, (Ancho / 2), (Altura / 2));

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		//uniforms
		int uniform_WindowSize = glGetUniformLocation(programID, "ws");
		glUniform2f(uniform_WindowSize, 800, 600);


		/*** FIGURA 1 ***/

		Mouse(&pitch, &yaw, xpos, ypos, &xpos2, &ypos2,&VectorDireccion, &VectorDireccionLados);
		PresionTecla(&PosActual, &pitch, &yaw, VectorDireccion, VectorDireccionLados);
		Matrices(PosActual, pitch, yaw);

		//Multiplicacion de matrices 
		int modelLoc = glGetUniformLocation(programID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mv_matrix));
		modelLoc = glGetUniformLocation(programID, "view");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		modelLoc = glGetUniformLocation(programID, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(proj_matrix));

		//Atributo 0 = posicion
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

		// Atributo, elementos que contiene en este caso (x, y, z), cuanto debe moverse, donde se empieza a leer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, avance, (void*)0);

		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, avance, (void*)36);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, avance, (void*)12);
		
		//Coordenadas x-y
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, avance, (void*)36);

		glDrawArrays(GL_TRIANGLES, 0, tam); //Como lo va a dibujar, inicio de arreglo, fin de arreglo

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		/*** FIGURA 2 ***/

		glUseProgram(programID2);

		//Multiplicacion de matrices 
		modelLoc = glGetUniformLocation(programID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mv_matrix));
		modelLoc = glGetUniformLocation(programID, "view");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		modelLoc = glGetUniformLocation(programID, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(proj_matrix));

		//Atributo 0 = posicion
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);

		// Atributo, elementos que contiene en este caso (x, y, z), cuanto debe moverse, donde se empieza a leer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, avance2, (void*)0);

		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, avance, (void*)36);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, avance2, (void*)12);

		//Coordenadas x-y
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, avance2, (void*)36);

		glDrawArrays(GL_TRIANGLES, 0, tam2); //Como lo va a dibujar, inicio de arreglo, fin de arreglo

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);


	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &vertexbuffer2);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
	glDeleteProgram(programID2);

	glfwTerminate();

	return 0;
}

int InitWindow()
{
	//GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		//getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(Ancho, Altura, "Lector de Objetos", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window\n");
		//getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	//Glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		//getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//OpenGl
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	return 1;
}

void Matrices(vec3 PosActual, float pitch, float yaw) {
	// Matriz de modelo, se aplica una rotacion sobre el eje Y 
	glm::mat4 Model(1.0f);
	mv_matrix = Model;

	// Matriz de proyeccion y visualizacion
	proj_matrix = glm::perspective(45.0f, (float)( 1080 / 900), 0.1f, 100.0f);

	// Camara, punto a donde ve, vector vista hacia arriba
	view_matrix = MatrizVista(vec3(PosActual.x, 0, PosActual.z), pitch, yaw);
}

mat4 MatrizVista(vec3 eye, float pitch, float yaw) {
	pitch = radians(pitch);
	yaw = radians(yaw);

	float cosPitch = cos(pitch);
	float senPitch = -sin(pitch);
	float cosYaw = cos(yaw);
	float senYaw= -sin(yaw);

	vec3 xaxis = { cosYaw, 0, -senYaw };
	vec3 yaxis = { senYaw * senPitch, cosPitch, cosYaw * senPitch };
	vec3 zaxis = { senYaw * cosPitch, -senPitch, cosPitch * cosYaw };

	mat4 view = {
		vec4(xaxis.x,			yaxis.x,			zaxis.x,	   0),
		vec4(xaxis.y,			yaxis.y,			zaxis.y,	   0),
		vec4(xaxis.z,			yaxis.z,			zaxis.z,	   0),
		vec4(-dot(xaxis, eye), -dot(yaxis, eye), -dot(zaxis, eye), 1)
	};

	return view;
}

void CalculaTamaño(bool UV, int numCaras, int* tam, int* avanza) {
	if (UV == true) {
		*tam = (numCaras * 11 * 3) * sizeof(GLfloat);
		*avanza = (sizeof(GLfloat) * NumVertices) + (sizeof(GLfloat) * 2);
	}
	else {
		*tam = (numCaras * NumVertices * NumAtributos * sizeof(GLfloat));
		*avanza = (sizeof(GLfloat) * NumVertices);
	}
}

void PresionTecla(vec3* PosActual, float* pitch, float* yaw, vec3 VectorDir, vec3 VectorDirLados) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		*PosActual -= VectorDir * Velocidad;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		*PosActual += VectorDir * Velocidad;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		*PosActual += VectorDirLados * Velocidad;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		*PosActual -= VectorDirLados * Velocidad;
	}
}

void Mouse(float* pitch, float* yaw, double xpos, double ypos, double*xpos2, double*ypos2, vec3* VectorDir, vec3* VectorDirLados) {
	glfwGetCursorPos(window, &xpos, &ypos);
	if (xpos != *xpos2) {
		*yaw = ((xpos * 360) / (Ancho));
		*xpos2 = xpos;
		if (*yaw < -360)
			*yaw += 360;
		if (*yaw > 360)
			*yaw -= 360;
	}
	if (ypos != *ypos2 && ypos != (Altura / 2)) {
		*pitch = ((ypos * 360) / (Altura / 2));
		*ypos2 = ypos;
	}

	GLfloat pitch1 = 0.0f;
	GLfloat yaw1 = 0.0f;

	yaw1 = glm::radians(*yaw);
	pitch1 = glm::radians(*pitch);

	*VectorDir = {
		-sin(yaw1) * cos(pitch1),
		0.0f,
		cos(yaw1)
	};

	*VectorDirLados = cross(*VectorDir, vec3(0.0f, 1.0f, 0.0f));

}
