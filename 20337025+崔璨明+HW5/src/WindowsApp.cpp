#include "WindowsApp.h"

#include <iostream>
#include <iomanip>
#include <array>

WindowsApp::ptr WindowsApp::m_instance = nullptr;

bool WindowsApp::setup(int width, int height, std::string title)
{
	m_screen_width = width;
	m_screen_height = height;
	m_window_title = title;

	m_last_mouse_x = 0;
	m_last_mouse_y = 0;
	m_mouse_delta_x = 0;
	m_mouse_delta_y = 0;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Create window
	m_window_handle = SDL_CreateWindow(
		m_window_title.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_screen_width,
		m_screen_height,
		SDL_WINDOW_SHOWN);

	if (m_window_handle == nullptr)
	{
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	m_renderer_handle = SDL_CreateRenderer(m_window_handle, -1, SDL_RENDERER_ACCELERATED);

	if (m_renderer_handle == nullptr)
	{
		std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}


	return true;
}

void WindowsApp::clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	//Clear screen
	SDL_SetRenderDrawColor(m_renderer_handle, r, g, b, a);
	SDL_RenderClear(m_renderer_handle);
}

void WindowsApp::drawRectFill(Uint8 r, Uint8 g, Uint8 b, int x, int y, int w, int h)
{
	//Render filled quad
	SDL_Rect fillRect = { x, y, w, h };
	SDL_SetRenderDrawColor(m_renderer_handle, r, g, b, 0xFF);
	SDL_RenderFillRect(m_renderer_handle, &fillRect);
}

void WindowsApp::drawRectLine(Uint8 r, Uint8 g, Uint8 b, int x, int y, int w, int h)
{
	//Render outlined quad
	SDL_Rect fillRect = { x, y, w, h };
	SDL_SetRenderDrawColor(m_renderer_handle, r, g, b, 0xFF);
	SDL_RenderDrawRect(m_renderer_handle, &fillRect);
}

void WindowsApp::drawLine(Uint8 r, Uint8 g, Uint8 b, int x0, int y0, int x1, int y1)
{
	//Draw line
	SDL_SetRenderDrawColor(m_renderer_handle, r, g, b, 0xFF);
	SDL_RenderDrawLine(m_renderer_handle, x0, y0, x1, y1);
}

void WindowsApp::drawPoint(Uint8 r, Uint8 g, Uint8 b, int x, int y)
{
	//Draw point
	SDL_SetRenderDrawColor(m_renderer_handle, r, g, b, 0xFF);
	SDL_RenderDrawPoint(m_renderer_handle, x, y);
}

void WindowsApp::updateScreen()
{
	SDL_RenderPresent(m_renderer_handle);
}

WindowsApp::~WindowsApp()
{
	//Destroy window
	SDL_DestroyRenderer(m_renderer_handle);
	SDL_DestroyWindow(m_window_handle);
	m_window_handle = nullptr;
	m_renderer_handle = nullptr;

	//Quit SDL subsystems
	SDL_Quit();
}

void WindowsApp::processEvent()
{
	m_wheel_delta = 0;
	//Handle events queue
	while (SDL_PollEvent(&m_events) != 0)
	{
		//Quit the program
		if (m_events.type == SDL_QUIT || (
			m_events.type == SDL_KEYDOWN && m_events.key.keysym.sym == SDLK_ESCAPE))
		{
			m_quit = true;
		}
		if (m_events.type == SDL_KEYDOWN && m_events.key.keysym.sym == SDLK_g)
		{
			m_g_keyboard_pressed = true;
		}
		if (m_events.type == SDL_KEYDOWN && m_events.key.keysym.sym == SDLK_c)
		{
			m_c_keyboard_pressed = true;
		}
		if (m_events.type == SDL_MOUSEMOTION)
		{
			static bool firstEvent = true;
			if (firstEvent)
			{
				firstEvent = false;
				m_last_mouse_x = m_events.motion.x;
				m_last_mouse_y = m_events.motion.y;
				m_mouse_delta_x = 0;
				m_mouse_delta_y = 0;
			}
			else
			{
				m_mouse_delta_x = m_events.motion.x - m_last_mouse_x;
				m_mouse_delta_y = m_events.motion.y - m_last_mouse_y;
				m_last_mouse_x = m_events.motion.x;
				m_last_mouse_y = m_events.motion.y;
			}
		}
		if (m_events.type == SDL_MOUSEBUTTONDOWN && m_events.button.button == SDL_BUTTON_LEFT)
		{
			m_mouse_left_button_pressed = true;
			m_last_mouse_x = m_events.motion.x;
			m_last_mouse_y = m_events.motion.y;
			m_mouse_delta_x = 0;
			m_mouse_delta_y = 0;
			m_mouse_click_x = m_events.motion.x;
			m_mouse_click_y = m_events.motion.y;
		}
		if (m_events.type == SDL_MOUSEBUTTONUP && m_events.button.button == SDL_BUTTON_LEFT)
		{
			m_mouse_left_button_pressed = false;
		}
		if (m_events.type == SDL_MOUSEWHEEL)
		{
			m_wheel_delta = m_events.wheel.y;
		}
	}
}

WindowsApp::ptr WindowsApp::getInstance()
{
	if (m_instance == nullptr)
	{
		return getInstance(800, 600, "WinApp");
	}
	return m_instance;
}

WindowsApp::ptr WindowsApp::getInstance(int width, int height, const std::string title)
{
	if (m_instance == nullptr)
	{
		m_instance = std::shared_ptr<WindowsApp>(new WindowsApp());
		if (!m_instance->setup(width, height, title))
		{
			return nullptr;
		}
	}
	return m_instance;
}
