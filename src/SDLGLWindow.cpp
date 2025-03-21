#include "Pipeline/SDLGLWindow.h"
#include "Pipeline/Object.h"
#include "Pipeline/Camera.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/ext.hpp>

#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <stdexcept>
#include <iostream>



SDLGLWindow::SDLGLWindow(const char* _title, int _width, int _height) :
    m_Window(nullptr),
    m_Objects(),
	m_camera(nullptr),
    m_Quit(false)
{
    m_Window = SDL_CreateWindow(_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if(m_Window == nullptr)
    {
        printf("Could not create SDL Window\n");
        throw std::runtime_error("Jeepers");
    }
    printf("SDLWindow created\n");

    if (!SDL_GL_CreateContext(m_Window))
	{
        printf("Failed to create GL context\n");
		throw std::runtime_error("Damn");
	}

	if (glewInit() != GLEW_OK)
	{
        printf("Failed to start GLEW\n");
		throw std::runtime_error("Jinkies");
	}

	m_camera = new Camera();

    glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    printf("Window created\n");
}

SDLGLWindow::~SDLGLWindow()
{
    SDL_DestroyWindow(m_Window);
}

void SDLGLWindow::Update()
{
    // Handle input
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 0.25f);
	glm::vec3 left = glm::vec3(0.25f, 0.0f, 0.0f);

    SDL_Event e = { 0 };

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			m_Quit = true;
		}
	}

	const Uint8* key = SDL_GetKeyboardState(NULL);
	if (key[SDL_SCANCODE_A])
	{
		m_camera->Translate(left);
	}

	if (key[SDL_SCANCODE_D])
	{
		m_camera->Translate(-left);
	}
		
	if (key[SDL_SCANCODE_W])
	{
		m_camera->Translate(forward);
	}

	if (key[SDL_SCANCODE_S])
	{
		m_camera->Translate(-forward);
	}

    // Render
    int windowWidth, windowHeight;
    SDL_GetWindowSize(m_Window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Instruct OpenGL to use our shader program, VAO and texture
	m_camera->Use();

    for(int i = 0; i < m_Objects.size(); i++)
    {
        m_Objects[i]->Draw(m_camera->GetShader());
    }

	glUseProgram(0);

	SDL_GL_SwapWindow(m_Window);
}

void SDLGLWindow::AddObject(Object* _object)
{
    std::shared_ptr<Object> newObjectPtr(_object);
    m_Objects.push_back(newObjectPtr);

    printf("Object added\n");
}