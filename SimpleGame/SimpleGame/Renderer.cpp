#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include "assert.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
	Class0310();
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_ParticleShader = CompileShaders("./Shaders/particle.vs", "./Shaders/particle.fs");
	m_FragmentSandboxShader = CompileShaders("./Shaders/FragmentSandBox.vs", "./Shaders/FragmentSandBox.fs");
	m_AlphaClearShader = CompileShaders("./Shaders/Alpha.vs", "./Shaders/Alpha.fs");
	m_VertexSandboxShader = CompileShaders("./Shaders/VertexSandbox.vs", "./Shaders/VertexSandbox.fs");
	m_TextureSandboxShader = CompileShaders("./Shaders/TextureSandbox.vs", "./Shaders/TextureSandbox.fs");
	m_GridMeshShader = CompileShaders("./Shaders/GridMesh.vs", "./Shaders/GridMesh.fs");

	//Create VBOs
	CreateVertexBufferObjects();
	CreateParticleVBO(1000);

	CreateTextures();

	m_RGBTexture = CreatePngTexture("./RGB.png", GL_NEAREST);

	m_0Texture = CreatePngTexture("./0512_0.png", GL_NEAREST);
	m_1Texture = CreatePngTexture("./0512_1.png", GL_NEAREST);
	m_2Texture = CreatePngTexture("./0512_2.png", GL_NEAREST);
	m_3Texture = CreatePngTexture("./0512_3.png", GL_NEAREST);
	m_4Texture = CreatePngTexture("./0512_4.png", GL_NEAREST);
	m_5Texture = CreatePngTexture("./0512_5.png", GL_NEAREST);
	m_MergedTexture = CreatePngTexture("./explosion.png", GL_NEAREST);

	m_ParticleTexture = CreatePngTexture("./0517_Particle.png", GL_NEAREST);

	CreateGridMesh();

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::CreateVertexBufferObjects()
{
	float textureRect[]
		=
	{
		-0.5,0.5,0,			0,0,
		-0.5,-0.5,0,		0,1,
		0.5,0.5,0,			1,0,
		0.5,0.5,0,			1,0,
		-0.5,-0.5,0,		0,1,
		0.5,-0.5,0,			1,1
	};
	glGenBuffers(1, &m_TextureSandboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureRect), textureRect, GL_STATIC_DRAW);

	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};
	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float rect1[] =
	{
		-1,-1,0,0,1,
		-1,1,0,0,0,
		1,1,0,1,0,
		-1,-1,0,0,1,
		1,1,0,1,0,
		1,-1,0,1,1
	};
	glGenBuffers(1, &m_FragmentSandboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_FragmentSandboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect1), rect1, GL_STATIC_DRAW);

	float rect2[] =
	{
		-1,-1,0,
		-1,1,0,
		1,1,0,
		-1,-1,0,
		1,1,0,
		1,-1,0
	};
	glGenBuffers(1, &m_AlphaClearVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_AlphaClearVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect2), rect2, GL_STATIC_DRAW);

	m_HorizontalLineVertexCount = 100;
	float* verticesLine = new float[m_HorizontalLineVertexCount * 3];
	float gap = 2.f / ((float)m_HorizontalLineVertexCount -1.f);
	int index = 0;
	for (int i = 0; i < m_HorizontalLineVertexCount; ++i)
	{
		verticesLine[index] = (float)i * gap - 1.f; index++;
		verticesLine[index] = 0.f; index++;
		verticesLine[index] = 0.f; index++;
	}
	glGenBuffers(1, &m_HorizontalLineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_HorizontalLineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_HorizontalLineVertexCount * 3, verticesLine, GL_STATIC_DRAW);
	delete[] verticesLine;
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

void Renderer::CreateParticleVBO(int numParticleCount)
{
	int vertexCount = 6;
	int particleCount = numParticleCount;
	int floatCount = 3;
	int totalFloatCount = particleCount * vertexCount * floatCount;

	int totalFloatCountSingle = particleCount * vertexCount * 1;

	int totalFloatCountFour = particleCount * vertexCount * 4;

	m_ParticleVertexCount = particleCount * vertexCount;

	float* vertices = NULL;
	vertices = new float[totalFloatCount];

	float particleSize = 0.01;

	int index = 0;

	for (int i = 0; i < numParticleCount; ++i)
	{
		float particleCenterX = 0.0f;//2.0f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		float particleCenterY = 0.0f;//2.0f * (((float)rand() / (float)RAND_MAX) - 0.5f);

		vertices[index] = particleCenterX - particleSize; index++;
		vertices[index] = particleCenterY + particleSize; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = particleCenterX - particleSize; index++;
		vertices[index] = particleCenterY - particleSize; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = particleCenterX + particleSize; index++;
		vertices[index] = particleCenterY + particleSize; index++;
		vertices[index] = 0.f; index++;

		vertices[index] = particleCenterX + particleSize; index++;
		vertices[index] = particleCenterY + particleSize; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = particleCenterX - particleSize; index++;
		vertices[index] = particleCenterY - particleSize; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = particleCenterX + particleSize; index++;
		vertices[index] = particleCenterY - particleSize; index++;
		vertices[index] = 0.f; index++;
	}

	glGenBuffers(1, &m_ParticleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*totalFloatCount, vertices, GL_STATIC_DRAW);
	delete[] vertices;

	float* verticesVel = NULL;
	verticesVel = new float[totalFloatCount];
	index = 0;
	for (int i = 0; i < numParticleCount; ++i)
	{
		float velX = 2.0f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		float velY = 2.0f * (((float)rand() / (float)RAND_MAX));//2.0f * (((float)rand() / (float)RAND_MAX) - 0.5f);

		verticesVel[index] = velX; index++;
		verticesVel[index] = velY; index++;
		verticesVel[index] = 0.f; index++;
		verticesVel[index] = velX; index++;
		verticesVel[index] = velY; index++;
		verticesVel[index] = 0.f; index++;
		verticesVel[index] = velX; index++;
		verticesVel[index] = velY; index++;
		verticesVel[index] = 0.f; index++;

		verticesVel[index] = velX; index++;
		verticesVel[index] = velY; index++;
		verticesVel[index] = 0.f; index++;
		verticesVel[index] = velX; index++;
		verticesVel[index] = velY; index++;
		verticesVel[index] = 0.f; index++;
		verticesVel[index] = velX; index++;
		verticesVel[index] = velY; index++;
		verticesVel[index] = 0.f; index++;
	}

	glGenBuffers(1, &m_ParticleVelVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVelVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCount, verticesVel, GL_STATIC_DRAW);
	delete[] verticesVel;

	//emitTime
	float* verticesEmitTime = NULL;
	verticesEmitTime = new float[totalFloatCountSingle];
	index = 0;
	for (int i = 0; i < numParticleCount; ++i)
	{
		float emitTime = 10.0f * (((float)rand() / (float)RAND_MAX));

		verticesEmitTime[index] = emitTime; index++;
		verticesEmitTime[index] = emitTime; index++;
		verticesEmitTime[index] = emitTime; index++;
		verticesEmitTime[index] = emitTime; index++;
		verticesEmitTime[index] = emitTime; index++;
		verticesEmitTime[index] = emitTime; index++;
	}

	glGenBuffers(1, &m_ParticleEmitTimeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleEmitTimeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCountSingle, verticesEmitTime, GL_STATIC_DRAW);
	delete[] verticesEmitTime;

	//LifeTime
	float* verticesLifeTime = NULL;
	verticesLifeTime = new float[totalFloatCountSingle];
	index = 0;
	for (int i = 0; i < numParticleCount; ++i)
	{
		float lifetime =  ((float)rand() / (float)RAND_MAX) *1.f;

		verticesLifeTime[index] = lifetime; index++;
		verticesLifeTime[index] = lifetime; index++;
		verticesLifeTime[index] = lifetime; index++;
		verticesLifeTime[index] = lifetime; index++;
		verticesLifeTime[index] = lifetime; index++;
		verticesLifeTime[index] = lifetime; index++;
	}

	glGenBuffers(1, &m_ParticleLifeTimeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleLifeTimeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCountSingle, verticesLifeTime, GL_STATIC_DRAW);
	delete[] verticesLifeTime;
;
		//Period
	float* verticesPeriodTime = NULL;
	verticesPeriodTime = new float[totalFloatCountSingle];
	index = 0;
	for (int i = 0; i < numParticleCount; ++i)
	{
		float period = 1.0f * (((float)rand() / (float)RAND_MAX));

		verticesPeriodTime[index] = period; index++;
		verticesPeriodTime[index] = period; index++;
		verticesPeriodTime[index] = period; index++;
		verticesPeriodTime[index] = period; index++;
		verticesPeriodTime[index] = period; index++;
		verticesPeriodTime[index] = period; index++;
	}

	glGenBuffers(1, &m_ParticlePeriodVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticlePeriodVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCountSingle, verticesPeriodTime, GL_STATIC_DRAW);
	delete[] verticesPeriodTime;

	//Period
	float* verticesAmpTime = NULL;
	verticesAmpTime = new float[totalFloatCountSingle];
	index = 0;
	for (int i = 0; i < numParticleCount; ++i)
	{
		float amp = 1.f * (2.f * (((float)rand() / (float)RAND_MAX)) - 1.f);

		verticesAmpTime[index] = amp; index++;
		verticesAmpTime[index] = amp; index++;
		verticesAmpTime[index] = amp; index++;
		verticesAmpTime[index] = amp; index++;
		verticesAmpTime[index] = amp; index++;
		verticesAmpTime[index] = amp; index++;
	}

	glGenBuffers(1, &m_ParticleAmpVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleAmpVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCountSingle, verticesAmpTime, GL_STATIC_DRAW);
	delete[] verticesAmpTime;

	//Value
	float* verticesValueTime = NULL;
	verticesValueTime = new float[totalFloatCountSingle];
	index = 0;
	for (int i = 0; i < numParticleCount; ++i)
	{
		float value = 1.f * ((float)rand() / (float)RAND_MAX);
		verticesValueTime[index] = value; index++;
		verticesValueTime[index] = value; index++;
		verticesValueTime[index] = value; index++;
		verticesValueTime[index] = value; index++;
		verticesValueTime[index] = value; index++;
		verticesValueTime[index] = value; index++;
	}

	glGenBuffers(1, &m_ParticleValueVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleValueVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCountSingle, verticesValueTime, GL_STATIC_DRAW);
	delete[] verticesValueTime;

	//Color
	float* verticesColor = NULL;
	verticesColor = new float[totalFloatCountFour];

	index = 0;
	for (int i = 0; i < numParticleCount; i++)
	{
		float r = 1.f * ((float)rand() / (float)RAND_MAX);
		float g = 1.f * ((float)rand() / (float)RAND_MAX);
		float b = 1.f * ((float)rand() / (float)RAND_MAX);
		float a = 1.f; //1.f * ((float)rand() / (float)RAND_MAX);
		verticesColor[index] = r; index++;
		verticesColor[index] = g; index++;
		verticesColor[index] = b; index++;
		verticesColor[index] = a; index++; // v1

		verticesColor[index] = r; index++;
		verticesColor[index] = g; index++;
		verticesColor[index] = b; index++;
		verticesColor[index] = a; index++; // v2

		verticesColor[index] = r; index++;
		verticesColor[index] = g; index++;
		verticesColor[index] = b; index++;
		verticesColor[index] = a; index++; // v3

		verticesColor[index] = r; index++;
		verticesColor[index] = g; index++;
		verticesColor[index] = b; index++;
		verticesColor[index] = a; index++; // v4

		verticesColor[index] = r; index++;
		verticesColor[index] = g; index++;
		verticesColor[index] = b; index++;
		verticesColor[index] = a; index++; // v5

		verticesColor[index] = r; index++;
		verticesColor[index] = g; index++;
		verticesColor[index] = b; index++;
		verticesColor[index] = a; index++; // v6
	}

	glGenBuffers(1, &m_ParticleColorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleColorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCountFour,
		verticesColor, GL_STATIC_DRAW);
	delete[] verticesColor;

	//Position, Color vbo
	int totalFloatCountPosColUV = numParticleCount * 6 * (3 + 4 + 2);
	float* verticesPosColUV = NULL;
	verticesPosColUV = new float[totalFloatCountPosColUV];

	particleSize = 0.1;

	index = 0;

	for (int i = 0; i < numParticleCount; ++i)
	{
		float particleCenterX = 0.0f;//2.0f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		float particleCenterY = 0.0f;//2.0f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		float r = 1.f * ((float)rand() / (float)RAND_MAX);
		float g = 1.f * ((float)rand() / (float)RAND_MAX);
		float b = 1.f * ((float)rand() / (float)RAND_MAX);
		float a = 1.f; //1.f * ((float)rand() / (float)RAND_MAX);

		verticesPosColUV[index] = particleCenterX - particleSize; index++;
		verticesPosColUV[index] = particleCenterY + particleSize; index++;
		verticesPosColUV[index] = 0.f; index++;
		verticesPosColUV[index] = r; index++;
		verticesPosColUV[index] = g; index++;
		verticesPosColUV[index] = b; index++;
		verticesPosColUV[index] = a; index++;

		verticesPosColUV[index] = 0; index++;
		verticesPosColUV[index] = 0; index++; // v1

		verticesPosColUV[index] = particleCenterX - particleSize; index++;
		verticesPosColUV[index] = particleCenterY - particleSize; index++;
		verticesPosColUV[index] = 0.f; index++;
		verticesPosColUV[index] = r; index++;
		verticesPosColUV[index] = g; index++;
		verticesPosColUV[index] = b; index++;
		verticesPosColUV[index] = a; index++;

		verticesPosColUV[index] = 0; index++;
		verticesPosColUV[index] = 1; index++; // v2

		verticesPosColUV[index] = particleCenterX + particleSize; index++;
		verticesPosColUV[index] = particleCenterY + particleSize; index++;
		verticesPosColUV[index] = 0.f; index++;
		verticesPosColUV[index] = r; index++;
		verticesPosColUV[index] = g; index++;
		verticesPosColUV[index] = b; index++;
		verticesPosColUV[index] = a; index++;

		verticesPosColUV[index] = 1; index++;
		verticesPosColUV[index] = 0; index++; // v3

		verticesPosColUV[index] = particleCenterX + particleSize; index++;
		verticesPosColUV[index] = particleCenterY + particleSize; index++;
		verticesPosColUV[index] = 0.f; index++;
		verticesPosColUV[index] = r; index++;
		verticesPosColUV[index] = g; index++;
		verticesPosColUV[index] = b; index++;
		verticesPosColUV[index] = a; index++;

		verticesPosColUV[index] = 1; index++;
		verticesPosColUV[index] = 0; index++; // v4

		verticesPosColUV[index] = particleCenterX - particleSize; index++;
		verticesPosColUV[index] = particleCenterY - particleSize; index++;
		verticesPosColUV[index] = 0.f; index++;
		verticesPosColUV[index] = r; index++;
		verticesPosColUV[index] = g; index++;
		verticesPosColUV[index] = b; index++;
		verticesPosColUV[index] = a; index++;

		verticesPosColUV[index] = 0; index++;
		verticesPosColUV[index] = 1; index++; // v5

		verticesPosColUV[index] = particleCenterX + particleSize; index++;
		verticesPosColUV[index] = particleCenterY - particleSize; index++;
		verticesPosColUV[index] = 0.f; index++;
		verticesPosColUV[index] = r; index++;
		verticesPosColUV[index] = g; index++;
		verticesPosColUV[index] = b; index++;
		verticesPosColUV[index] = a; index++;

		verticesPosColUV[index] = 1; index++;
		verticesPosColUV[index] = 1; index++; // v6
	}

	glGenBuffers(1, &m_ParticlePosColUVVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticlePosColUVVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCountPosColUV, verticesPosColUV, GL_STATIC_DRAW);
	delete[] verticesPosColUV;


}

void Renderer::CreateGridMesh()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 8;
	int pointCountY = 8;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX * pointCountY * 2];
	float* vertices = new float[(pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3];
	gDummyVertexCount = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y * pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y * pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &VBO_DummyMesh);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_DummyMesh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);
}

void Renderer::Class0310()
{
	float vertices[] = {0,0,0, 1,0,0, 1,1,0};

	glGenBuffers(1, &m_testVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);//데이터를 올림 STATIC_DRAW를 쓰면 이 정보는 앞으로 변하지 않는다 라는 뜻을 가진다. STATIC이 있으면 DYNAMIC도 있다. STATIC의 경우는 앞으로 cpu가 데이터를 가지고 있을 필요가 없으므로 cpu메모리를 사용하지 않는다.
	//하지만 DYNAMIC의 경우에는 올리는 데이터 만큼의 데이터 공간을 cpu메모리에 만든다. 둘의 성능 차이는 별로 없으나 DYNAMIC은 메모리가 조금 더 소비된다.

	float vertices1[] = { -1,-1,0 ,0,-1,0, 0,0,0 };

	glGenBuffers(1, &m_testVBO1);
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);


	float colors[] = { 1,0,0,1, 0,1,0,1, 0,0,1,1,1,0,0,1, 0,0,1,1, 0,1,0,1 };

	glGenBuffers(1, &m_testVBOColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBOColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	//int size = 40000000000000;
	//float* testTemp = new float[size];
	//memset(testTemp, 1, sizeof(float) * size);
	
	//GLuint testVBO1 = 0;
	//glGenBuffers(1, &testVBO1);
	//glBindBuffer(GL_ARRAY_BUFFER, testVBO1); //bind는 시간이 많이 걸리지 않는다.
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*size, testTemp, GL_STATIC_DRAW);
}

void Renderer::Class0310_Rendering()
{
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), 0, 0, 0, 1);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), 1, 1, 1, 1);

	int attribLocation = glGetAttribLocation(m_SolidRectShader, "a_Position");

	glEnableVertexAttribArray(attribLocation);
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);
	glVertexAttribPointer(attribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	int attribLocation1 = glGetAttribLocation(m_SolidRectShader, "a_Position1");

	glEnableVertexAttribArray(attribLocation1);
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO1);
	glVertexAttribPointer(attribLocation1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);

	if (point) frame--;
	else frame++;
	int uniformScale = glGetUniformLocation(m_SolidRectShader, "u_Scale");
	glUniform1f(uniformScale, frame/60.0);

	if (frame == 60) point = !point;
	else if (frame == 0) point = !point;


	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::DrawParticle()
{
	GLuint program = m_ParticleShader;
	glUseProgram(program);//shader ID를 넘긴다.
	//int posLoc = glGetAttribLocation(program, "a_Position");
	//glEnableVertexAttribArray(posLoc);
	//glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	//glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//int colorLoc = glGetAttribLocation(program, "a_Color");
	//glEnableVertexAttribArray(colorLoc);
	//glBindBuffer(GL_ARRAY_BUFFER, m_ParticleColorVBO);
	//glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int posLoc = glGetAttribLocation(program, "a_Position");
	glEnableVertexAttribArray(posLoc);
	int colorLoc = glGetAttribLocation(program, "a_Color");
	glEnableVertexAttribArray(colorLoc);
	int uvLoc = glGetAttribLocation(program, "a_UV");
	glEnableVertexAttribArray(uvLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticlePosColUVVBO);

	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float)*9, 0);
	glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, sizeof(float)*9, (GLvoid*)(sizeof(float)*3));
	glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float)*9, (GLvoid*)(sizeof(float)*7));

	int posLoc3 = glGetAttribLocation(program, "a_Vel");
	glEnableVertexAttribArray(posLoc3);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVelVBO);
	glVertexAttribPointer(posLoc3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	int posLoc4 = glGetAttribLocation(program, "a_emit_time");
	glEnableVertexAttribArray(posLoc4);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleEmitTimeVBO);
	glVertexAttribPointer(posLoc4, 1, GL_FLOAT, GL_FALSE, 0, 0);

	int posLoc5 = glGetAttribLocation(program, "a_life_time");
	glEnableVertexAttribArray(posLoc5);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleLifeTimeVBO);
	glVertexAttribPointer(posLoc5, 1, GL_FLOAT, GL_FALSE, 0, 0);

	int posLoc6 = glGetAttribLocation(program, "a_Period");
	glEnableVertexAttribArray(posLoc6);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticlePeriodVBO);
	glVertexAttribPointer(posLoc6, 1, GL_FLOAT, GL_FALSE, 0, 0);

	int posLoc7 = glGetAttribLocation(program, "a_Amp");
	glEnableVertexAttribArray(posLoc7);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleAmpVBO);
	glVertexAttribPointer(posLoc7, 1, GL_FLOAT, GL_FALSE, 0, 0);

	int posLoc8 = glGetAttribLocation(program, "a_Value");
	glEnableVertexAttribArray(posLoc8);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleValueVBO);
	glVertexAttribPointer(posLoc8, 1, GL_FLOAT, GL_FALSE, 0, 0);

	//int posLoc2 = glGetAttribLocation(program, "a_Color");
	//glEnableVertexAttribArray(posLoc2);
	//glBindBuffer(GL_ARRAY_BUFFER, m_testVBOColor);
	//glVertexAttribPointer(posLoc2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	int timeLoc = glGetUniformLocation(program, "u_Time");
	glUniform1f(timeLoc, g_time);

	int accelLoc = glGetUniformLocation(program, "u_Accel");
	glUniform3f(accelLoc, 0.f,-2.8f,0.f);

	int texULoc = glGetUniformLocation(program, "u_Texture");
	glUniform1i(texULoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ParticleTexture);

	g_time += 0.001;

	glDrawArrays(GL_TRIANGLES, 0, m_ParticleVertexCount);

	glDisable(GL_BLEND);
}

void Renderer::DrawFragmentSandBox()
{
	GLuint shader = m_FragmentSandboxShader;
	glUseProgram(shader);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	int posLoc = glGetAttribLocation(shader, "a_Position");
	int texLoc = glGetAttribLocation(shader, "a_UV");
	glEnableVertexAttribArray(posLoc);
	glEnableVertexAttribArray(texLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_FragmentSandboxVBO);

	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));
	
	int pointLoc = glGetUniformLocation(shader, "u_point");
	glUniform2f(pointLoc, 0.5,0.5);

	float points[] = { 0.1,0.1,0.5,0.5,0.8,0.8 };
	int pointsLoc = glGetUniformLocation(shader, "u_points");
	glUniform2fv(pointsLoc, 3, points);

	int timeLoc = glGetUniformLocation(shader, "u_Time");
	glUniform1f(timeLoc, g_time);
	g_time += 0.001;

	int textureLoc = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(textureLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RGBTexture);


	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawAlphaClear()
{
	GLuint shader = m_AlphaClearShader;
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int posLoc = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(posLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_AlphaClearVBO);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0 , 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawVertexSandBox()
{
	GLuint shader = m_VertexSandboxShader;
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int posLoc = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(posLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_HorizontalLineVBO);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	int timeLoc = glGetUniformLocation(shader, "u_Time");
	//glUniform1f(timeLoc, g_time);
	g_time += 0.016;

	for (int i = 0; i <5; ++i)
	{
		glUniform1f(timeLoc, g_time + (float)i * 0.2);
		glDrawArrays(GL_LINE_STRIP, 0, m_HorizontalLineVertexCount);
	}

	glDisable(GL_BLEND);
}

void Renderer::DrawTextureSandBox()
{
	GLuint shader = m_TextureSandboxShader;
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int posLoc = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(posLoc);
	int textLoc = glGetAttribLocation(shader, "a_TexPos");
	glEnableVertexAttribArray(textLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(textLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_0Texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_1Texture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_2Texture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_3Texture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_4Texture);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_5Texture);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_MergedTexture);

	int texID[] = { 0,1 };
	GLuint samplerULoc = glGetUniformLocation(shader, "u_MultiTexSampler");
	glUniform1iv(samplerULoc, 2, texID);

	samplerULoc = glGetUniformLocation(shader, "u_TexSampler");
	glUniform1i(samplerULoc, 6);
	
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_CheckerBoardTexture);
	//glBindTexture(GL_TEXTURE_2D, m_RGBTexture);

	GLuint repeatULoc = glGetUniformLocation(shader, "u_XYReapeat");
	glUniform2f(repeatULoc, 4.f,4.f);

	GLuint StepULoc = glGetUniformLocation(shader, "u_Step");
	glUniform1f(StepULoc, (int)(g_time)%6);

	GLuint SeqNumLoc = glGetUniformLocation(shader, "u_SeqNum");
	glUniform1f(SeqNumLoc, g_time * 10.0);

	g_time += 0.08;

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawGridMesh()
{
	glUseProgram(m_GridMeshShader);

	int attrribPosition = glGetAttribLocation(m_GridMeshShader, "Position");

	glEnableVertexAttribArray(attrribPosition);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_DummyMesh);
	glVertexAttribPointer(attrribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_LINE_STRIP, 0, gDummyVertexCount);
}

void Renderer::CreateTextures()
{
	GLulong checkerboard[] =
	{
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF
	};
	glGenTextures(1, &m_CheckerBoardTexture);
	glBindTexture(GL_TEXTURE_2D, m_CheckerBoardTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerboard);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

GLuint Renderer::CreatePngTexture(char* filePath, GLuint samplingMethod)
{
	std::vector<unsigned char>image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		std::cout << "PNG image loading failed" << filePath<<std::endl;
		assert(0);
	}
	GLuint temp;
	glGenTextures(1, &temp);
	glBindTexture(GL_TEXTURE_2D, temp);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);
	return temp;
}