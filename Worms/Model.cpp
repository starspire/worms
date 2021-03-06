#include "Model.h"
#include <fstream>
#include <string>
#include "glm/gtc/type_ptr.hpp"

using namespace std;
using namespace glm;

Model::Model()
{
	for (int i = 0; i < 3; i++)
	{
		ambient[i] = 0;
		specular[i] = 0;
		diffuse[i] = 0;
	}
	ambient[3] = 1; specular[3] = 1; diffuse[3] = 1;
}


Model::~Model()
{
}

void Model::SetTexture(GLuint in_handle)
{
	tex_handle = in_handle;
}

void Model::Draw(glm::mat4 &view, glm::mat4 &model)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

	glLoadMatrixf(value_ptr(view*M2));
	glBindTexture(GL_TEXTURE_2D, tex_handle);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &textures[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glDrawArrays(GL_TRIANGLES, 0, vertex_count);


	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

/*	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	float wierzch[12] = {
		boundingBox.bottomLeft.x, boundingBox.bottomLeft.y, boundingBox.bottomLeft.z,
		boundingBox.bottomRight.x, boundingBox.bottomRight.y, boundingBox.bottomRight.z,
		boundingBox.topRight.x, boundingBox.topRight.y, boundingBox.topRight.z,
		boundingBox.topLeft.x, boundingBox.topLeft.y, boundingBox.topLeft.z
	};
	float colors[12] = {
		0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0
	};
	float normalz[12] = {
		0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1
	};
	glVertexPointer(3, GL_FLOAT, 0, wierzch);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glNormalPointer(3, 0, normalz);
	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY); */
}


bool Model::loadObj(string path)
{
	/*vertices.clear();
	textures.clear();
	normals.clear(); */
	vertex_count = 0;
	fstream plik;
	plik.open(path, ios::in);
	if (plik.good() == false)
		return false;

	vector<float> local_normals;
	vector<float> local_vertices;
	vector<float> local_textures;
	vector<int> normalsIndices;
	vector<int> textureIndices;
	vector<int> vertexIndices;
	string buf;
	float fbuf;

	float bottom = 0, top = 0, leftest = 0, rightest = 0; // iksDE // for boundingBox construction
	while (!plik.eof())
	{
		plik >> buf;
		if (buf.compare("v") == 0)
		{
			plik >> fbuf;
			local_vertices.push_back(fbuf);

			if (fbuf > rightest) rightest = fbuf;
			else if (fbuf < leftest) leftest = fbuf;

			plik >> fbuf;
			local_vertices.push_back(fbuf);

			if (fbuf > top) top = fbuf;
			else if (fbuf < bottom) bottom = fbuf;

			plik >> fbuf;
			local_vertices.push_back(fbuf);
		}
		else if (buf.compare("vn") == 0)
		{
			plik >> fbuf;
			local_normals.push_back(fbuf);
			plik >> fbuf;
			local_normals.push_back(fbuf);
			plik >> fbuf;
			local_normals.push_back(fbuf);
		}
		else if (buf.compare("f") == 0)
		{
			int indexa, indexb, indexc;
			for (int i = 0; i < 3; i++)
			{
				plik >> buf;
				sscanf_s(buf.c_str(), "%d/%d/%d", &indexa, &indexc, &indexb);
				vertexIndices.push_back(indexa - 1);
				normalsIndices.push_back(indexb - 1);
				textureIndices.push_back(indexc - 1);
			}
		}
		else if (buf.compare("vt") == 0)
		{
			plik >> fbuf;
			local_textures.push_back(fbuf);
			plik >> fbuf;
			local_textures.push_back(fbuf);
		}
	}
	for (int i = 0; i < normalsIndices.size(); i++)
	{
		int a = normalsIndices[i] * 3;
		int b = vertexIndices[i] * 3;
		int c = textureIndices[i] * 2;
		vertices.push_back(local_vertices[b]);
		vertices.push_back(local_vertices[b + 1]);
		vertices.push_back(local_vertices[b + 2]);
		normals.push_back(local_normals[a]);
		normals.push_back(local_normals[a + 1]);
		normals.push_back(local_normals[a + 2]);
		textures.push_back(local_textures[c]);
		textures.push_back(local_textures[c+1]);
		vertex_count++;
	}
	plik.close();

	boundingBox.bottomLeft = vec4(leftest, bottom, 0, 1);
	boundingBox.bottomRight = vec4(rightest, bottom, 0, 1);
	boundingBox.topLeft = vec4(leftest, top, 0, 1);
	boundingBox.topRight = vec4(rightest, top, 0, 1);

	return true;
}
