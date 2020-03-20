#ifndef FIGURAS_HPP
#define FIGURAS_HPP
#include <vector>
#include <string>
#include <GLFW/glfw3.h>

using namespace std;

class Vertice
{
private:
	float x, y, z;
public:
	Vertice();
	Vertice(float x, float y, float z);
	void SetVertice(float x, float y, float z);
	float GetX();
	float GetY();
	float GetZ();
};

class Cara {
private:
	vector<string> Vertices;
public:
	Cara();
	Cara(string v1, string v2, string v3);
	void AgregaVertice(string v);
	string GetVertice(int i);
};

class Figuras {
private:
	int numCaras;
	GLfloat* Vertices;
	vector<Cara> Caras;
	vector<Vertice> Normales;
public:
	Figuras();
	Figuras(int numCar, GLfloat* car, vector<Cara> c, vector<Vertice> nom);
	void AgregaCara(Cara c);
	const GLfloat* GetVertices();
	int GetCaras();
	vector<Vertice> GetNormales();
};

#endif



