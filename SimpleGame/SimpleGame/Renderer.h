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
	void Class0310_Rendering();
	void DrawParticle();
	void DrawFragmentSandBox();
	void DrawAlphaClear();
	void DrawVertexSandBox();
	void DrawTextureSandBox();
	void CreateTextures();
	GLuint CreatePngTexture(char* filePath, GLuint samplingMethod);

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float *newX, float *newY);
	void Class0310();//데이터를 준비하고 올리는 작업을 할것이다.
	void CreateParticleVBO(int numParticleCount);
	GLuint m_ParticleShader = -1;
	GLuint m_ParticleVBO = -1;
	GLuint m_ParticleVertexCount = -1;
	GLuint m_ParticleVelVBO = -1;
	GLuint m_ParticleEmitTimeVBO = -1;
	GLuint m_ParticleLifeTimeVBO = -1;

	GLuint m_ParticlePeriodVBO = -1;
	GLuint m_ParticleAmpVBO = -1;
	GLuint m_ParticleValueVBO = -1;

	GLuint m_ParticleColorVBO = -1;

	GLuint m_ParticlePosColUVVBO = -1;

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_SolidRectShader = 0; //우리가 사용할 셰이더 프로그램의 ID가 저장되는 멤버변수이다.

	GLuint m_testVBO = 0;
	GLuint m_testVBO1 = 0;
	GLuint m_testVBOColor = 0;
	GLuint m_FragmentSandboxShader = 0;
	GLuint m_FragmentSandboxVBO = 0;

	GLuint m_AlphaClearShader = 0;
	GLuint m_AlphaClearVBO = 0;

	GLuint m_VertexSandboxShader = 0;
	GLuint m_HorizontalLineVBO = 0;
	GLuint m_HorizontalLineVertexCount = 0;

	GLuint m_TextureSandboxShader = 0;
	GLuint m_TextureSandboxVBO = 0;

	GLuint m_CheckerBoardTexture = 0;
	GLuint m_RGBTexture = 0;

	GLuint m_0Texture = 0;
	GLuint m_1Texture = 0;
	GLuint m_2Texture = 0;
	GLuint m_3Texture = 0;
	GLuint m_4Texture = 0;
	GLuint m_5Texture = 0;
	GLuint m_MergedTexture = 0;

	GLuint m_ParticleTexture = 0;

	int frame = 0;
	bool point = 0;
	float g_time = 0;
};

