#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
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
	m_ParticleShader = CompileShaders("./Shaders/Particle.vs", "./Shaders/Particle.fs");

	//Create VBOs
	CreateVertexBufferObjects();

	CreateParticleVBO(1000);

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
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::CreateParticleVBO(int numParticleCount)
{
	int vertexCount = 6;
	int particleCount = numParticleCount;
	int floatCount = 3;
	int totalFloatCount = particleCount * vertexCount * floatCount;
	int totalFloatCountSingle = particleCount * vertexCount * 1;

	m_ParticleVertexCount = particleCount * vertexCount;

	float* vertices = NULL;
	vertices = new float[totalFloatCount];

	float particleSize = 0.01f;

	int index = 0;
	for(int i = 0; i < numParticleCount ; ++i)
	{
		float particleCenterY = 0.f;
		float particleCenterX = 0.f;
		// -1 에서 1 사이 랜덤 값이 나옴

		vertices[index] = particleCenterX - particleSize;		index++;
		vertices[index] = particleCenterY + particleSize;		index++;
		vertices[index] = 0.f;		index++;
		vertices[index] = particleCenterX - particleSize;		index++;
		vertices[index] = particleCenterY - particleSize;		index++;
		vertices[index] = 0.f;		index++;
		vertices[index] = particleCenterX + particleSize;		index++;
		vertices[index] = particleCenterY + particleSize;		index++;
		vertices[index] = 0.f;		index++;

		vertices[index] = particleCenterX + particleSize;		index++;
		vertices[index] = particleCenterY + particleSize;		index++;
		vertices[index] = 0.f;		index++;
		vertices[index] = particleCenterX - particleSize; 		index++;
		vertices[index] = particleCenterY - particleSize; 		index++;
		vertices[index] = 0.f;		index++;
		vertices[index] = particleCenterX + particleSize;		index++;
		vertices[index] = particleCenterY - particleSize;		index++;
		vertices[index] = 0.f;		index++;
	}

	glGenBuffers(1, &m_ParticleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCount, vertices, GL_STATIC_DRAW);

	delete[] vertices;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float* speed = NULL;
	speed = new float[totalFloatCount];

	index = 0;
	for (int i = 0; i < numParticleCount; ++i)
	{
		float velX = 2.f * (float(rand()) / RAND_MAX) - 1.f;
		float velY = 2.f * (float(rand()) / RAND_MAX) - 1.f;
		// -1 에서 1 사이 랜덤 값이 나옴

		for(int j = 0; j < vertexCount; ++j)
		{
			speed[index] = velX;		index++;
			speed[index] = velY;		index++;
			speed[index] = 0.f;		index++;
		}
	}

	glGenBuffers(1, &m_ParticleSpeedVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleSpeedVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCount, speed, GL_STATIC_DRAW);

	delete[] speed;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float* emitTime = NULL;
	emitTime = new float[totalFloatCountSingle];

	index = 0;
	for (int i = 0; i < numParticleCount; ++i)
	{
		float fEmitTime = 10.f * (float(rand()) / RAND_MAX);

		for (int j = 0; j < vertexCount; ++j)
		{
			emitTime[index] = fEmitTime;
			index++;
		}
	}

	glGenBuffers(1, &m_ParticleEmitTimeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleEmitTimeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCountSingle, emitTime, GL_STATIC_DRAW);

	delete[] emitTime;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float* lifeTime = NULL;
	lifeTime = new float[totalFloatCountSingle];

	index = 0;
	for (int i = 0; i < numParticleCount; ++i)
	{
		float fLifeTime = 1.f * (float(rand()) / RAND_MAX);

		for (int j = 0; j < vertexCount; ++j)
		{
			lifeTime[index] = fLifeTime;
			index++;
		}
	}

	glGenBuffers(1, &m_ParticleLifeTimeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleLifeTimeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCountSingle, lifeTime, GL_STATIC_DRAW);

	delete[] lifeTime;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float* randAmp = NULL;
	randAmp = new float[totalFloatCountSingle];

	index = 0;
	for (int i = 0; i < numParticleCount; ++i)
	{
		float fRand = 1.f * (2.f * (float(rand()) / RAND_MAX) - 1.f);

		for (int j = 0; j < vertexCount; ++j)
		{
			randAmp[index] = fRand;
			index++;
		}
	}

	glGenBuffers(1, &m_ParticleAmpVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleAmpVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCountSingle, randAmp, GL_STATIC_DRAW);

	delete[] randAmp;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float* randPeriod = NULL;
	randPeriod = new float[totalFloatCountSingle];

	index = 0;
	for (int i = 0; i < numParticleCount; ++i)
	{
		float fRand = 2.f * (float(rand()) / RAND_MAX) - 1.f;

		for (int j = 0; j < vertexCount; ++j)
		{
			randPeriod[index] = fRand;
			index++;
		}
	}

	glGenBuffers(1, &m_ParticlePeriodVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticlePeriodVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCountSingle, randPeriod, GL_STATIC_DRAW);

	delete[] randPeriod;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float* verticeValue = NULL;
	verticeValue = new float[totalFloatCountSingle];

	index = 0;
	for (int i = 0; i < numParticleCount; ++i)
	{
		float value = 1.f * (float(rand()) / RAND_MAX);

		for (int j = 0; j < vertexCount; ++j)
		{
			verticeValue[index] = value;
			index++;
		}
	}

	glGenBuffers(1, &m_ParticleValueVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleValueVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCountSingle, verticeValue, GL_STATIC_DRAW);

	delete[] verticeValue;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

void Renderer::DrawParticle()
{
	GLuint program = m_ParticleShader;	// ID
	glUseProgram(program);

	//
	int posLoc = glGetAttribLocation(program, "a_Position");
	glEnableVertexAttribArray(posLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//

	//
	int velLoc = glGetAttribLocation(program, "a_Speed");
	glEnableVertexAttribArray(velLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleSpeedVBO);
	glVertexAttribPointer(velLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//

	//
	int emitTimeLoc = glGetAttribLocation(program, "a_EmitTime");
	glEnableVertexAttribArray(emitTimeLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleEmitTimeVBO);
	glVertexAttribPointer(emitTimeLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
	//

	//
	int lifeTimeLoc = glGetAttribLocation(program, "a_LifeTime");
	glEnableVertexAttribArray(lifeTimeLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleLifeTimeVBO);
	glVertexAttribPointer(lifeTimeLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
	//

	//
	int ampLoc = glGetAttribLocation(program, "a_Amp");
	glEnableVertexAttribArray(ampLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleAmpVBO);
	glVertexAttribPointer(ampLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
	//

	//
	int periodLoc = glGetAttribLocation(program, "a_Period");
	glEnableVertexAttribArray(periodLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticlePeriodVBO);
	glVertexAttribPointer(periodLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
	//

	//
	int timeLoc = glGetUniformLocation(program, "u_Time");
	glUniform1f(timeLoc, m_fTime);
	int accelLoc = glGetUniformLocation(program, "u_Accel");
	glUniform3f(accelLoc, 0.f, -0.1f, 0.f);
	//

	//
	int valueLoc = glGetAttribLocation(program, "a_Value");
	glEnableVertexAttribArray(valueLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleValueVBO);
	glVertexAttribPointer(valueLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
	//

	m_fTime += 0.01f;

	glDrawArrays(GL_TRIANGLES, 0, m_ParticleVertexCount);
		// 갯수는 정확히 : 다른 메모리 공간이 침범될수도 있음 (프로그램 죽음)
}
