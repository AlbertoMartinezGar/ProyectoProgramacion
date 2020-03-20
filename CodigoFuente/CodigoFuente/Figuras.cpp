#include "Figuras.h"

Vertice::Vertice() {
	x = y = z = 0.0;
}

Vertice::Vertice(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vertice::SetVertice(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vertice::GetX() {
	return x;
}

float Vertice::GetY() {
	return y;
}

float Vertice::GetZ() {
	return z;
}

Cara::Cara() {
}

Cara::Cara(string v1, string v2, string v3) {
	Vertices.push_back(v1);
	Vertices.push_back(v2);
	Vertices.push_back(v3);
}

void Cara::AgregaVertice(string v) {
	Vertices.push_back(v);
}

string Cara::GetVertice(int i) {
	return Vertices[i];
}

Figuras::Figuras() {
	numCaras = 0;
	Vertices = 0;
}

Figuras::Figuras(int numCar, GLfloat *vert, vector<Cara> c, vector<Vertice> nom) {
	this->numCaras = numCar;
	Vertices = vert;
	Caras = c;
	Normales = nom;
}

void Figuras::AgregaCara(Cara c) {
	Caras.push_back(c);
}

const GLfloat* Figuras::GetVertices(){
	return (Vertices);
}

int Figuras::GetCaras() {
	return numCaras;
}

vector<Vertice> Figuras::GetNormales() {
	return Normales;
}