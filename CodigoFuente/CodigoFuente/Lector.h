#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <string>
#include "Figuras.h"
#include <iterator>
#include <glm/glm.hpp>

#define NumAtributos 3
#define NumVertices 9

using namespace std; 
using namespace glm;

GLfloat* AbreArchivo(string nom, int*tamCaras, bool*UV_Detector) {
	ifstream archivo = ifstream(nom);

	if (!archivo.is_open()) {
		cout << "No se encontro el archivo: " << nom << endl;
		return NULL;
	}

	vector <Vertice> tem_vertices;
	vector<Vertice> tem_normales;
	vector<glm::vec2> tem_uv;

	vector<unsigned int> verticeIndices, uvIndices, normalIndices;
	int numCaras = 0;

	while (archivo) {
		string linea;
		getline(archivo, linea);

		istringstream str(linea);
		string inicio;
		str >> inicio;

		float v1, v2, v3;

		if (inicio == "vt") {
			str >> v1 >> v2;
			tem_uv.push_back(glm::vec2(v1, v2));
			*UV_Detector = true;
			//cout << "vt " << v1 << " " << v2 << endl;
		}
		else if (inicio == "vn") {
			str >> v1 >> v2 >> v3;
			Vertice vertice_aux = Vertice(v1, v2, v3);
			tem_normales.push_back(vertice_aux);
			//cout << "vn " << v1 << " " << v2 << " " << v3 << endl;
		}
		else if (inicio == "v") {
			str >> v1 >> v2 >> v3;
			Vertice vertice_aux = Vertice(v1, v2, v3);
			tem_vertices.push_back(vertice_aux);
			//cout << "v " << v1 << " " << v2 << " " << v3 << endl;
		}
		else if (inicio == "f") {
			unsigned int v1, v2, v3, n1, n2, n3, uv1, uv2, uv3;

			std::replace_if(std::begin(linea), std::end(linea), [](const char& ch) {return ch == '/'; }, ' ');

			istringstream face_str(linea);
			face_str.ignore(linea.length(), ' ');

			if ((*UV_Detector) == false) 
				face_str >> v1 >> n1 >> v2 >> n2 >> v3 >> n3;
			else
				face_str >> v1 >> uv1 >> n1 >> v2 >> uv2 >> n2 >> v3 >> uv3 >> n3;

			//cout << "f " << v1 << "/" << uv1 << "/" << n1 << " /" << v2 << "/" << uv2 << "/" << n2 << "/ " << v3 << "/" << uv3 << "/" << n3 << endl;

			verticeIndices.push_back(v1);
			verticeIndices.push_back(v2);
			verticeIndices.push_back(v3);

			normalIndices.push_back(n1);
			normalIndices.push_back(n2);
			normalIndices.push_back(n3);

			if ((*UV_Detector) != false) {
				uvIndices.push_back(uv1);
				uvIndices.push_back(uv2);
				uvIndices.push_back(uv3);
			}

			numCaras++;
		}
	}

	*tamCaras = numCaras;
	int w = 0, x = 0, y = 0;
	if ((*UV_Detector) == true) {
		//int tam = (numCaras * NumVertices * NumAtributos * sizeof(GLfloat)) + (9 * numCaras);
		int tam = (numCaras * 11 * 3) * sizeof(GLfloat); 
		GLfloat* buffer = new GLfloat[tam];
		for (unsigned int i = 0; i < verticeIndices.size(); i++) {
			unsigned int vIndice = verticeIndices[i];
			unsigned int uvIndice = uvIndices[i];
			unsigned int nIndice = normalIndices[i];

			buffer[w++] = tem_vertices[vIndice - 1].GetX();
			buffer[w++] = tem_vertices[vIndice - 1].GetY();
			buffer[w++] = tem_vertices[vIndice - 1].GetZ();

			buffer[w++] = tem_normales[nIndice - 1].GetX();
			buffer[w++] = tem_normales[nIndice - 1].GetY();
			buffer[w++] = tem_normales[nIndice - 1].GetZ();

			buffer[w++] = 1.0f;
			buffer[w++] = 0.0f;
			buffer[w++] = 0.0f;

			buffer[w++] = x++;
			buffer[w++] = y;

			if (x == 2) {
				y++;
				x = 0;
			}

			//buffer[w++] = tem_uv[uvIndice - 1].x;
			//buffer[w++] = tem_uv[uvIndice - 1].y;
		}
		return buffer;
	}
	else {
		int tam = numCaras * 9 * NumAtributos * sizeof(GLfloat);
		GLfloat* buffer = new GLfloat[tam];
		for (unsigned int i = 0; i < verticeIndices.size(); i++) {
			unsigned int vIndice = verticeIndices[i];
			unsigned int nIndice = normalIndices[i];

			buffer[w++] = tem_vertices[vIndice - 1].GetX();
			buffer[w++] = tem_vertices[vIndice - 1].GetY();
			buffer[w++] = tem_vertices[vIndice - 1].GetZ();

			buffer[w++] = tem_normales[nIndice - 1].GetX();
			buffer[w++] = tem_normales[nIndice - 1].GetY();
			buffer[w++] = tem_normales[nIndice - 1].GetZ();

			buffer[w++] = 1.0f;
			buffer[w++] = 0.0f;
			buffer[w++] = 0.0f;
		}
		return buffer;
	}
}


