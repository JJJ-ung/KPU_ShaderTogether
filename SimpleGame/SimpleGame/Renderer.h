#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float *newX, float *newY);

private:
	bool m_Initialized = false;

	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_SolidRectShader = 0;
		// 사용할 쉐이더 프로그램의 ID

	// //////////////////////////////////////////////////////////////////////////
private:
	void Class0310();
	void CreateParticleVBO();

public:
	void Class0310_Rendering();
	void DrawParticle();

private:
	GLuint m_testVBO = 0;
	float m_fScale = 0.f;
	float m_fAdd = 0.01f;

private:
	GLuint m_ParticleVBO = 0;
	GLuint m_ParticleColorVBO = 0;
	GLuint m_ParticleVertexCount = -1;

private:
	GLuint m_ParticleShader = -1;
	// //////////////////////////////////////////////////////////////////////////
};

