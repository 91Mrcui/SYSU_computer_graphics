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

	//Get window surface
	m_screen_surface = SDL_GetWindowSurface(m_window_handle);

	return true;
}

WindowsApp::~WindowsApp()
{
	//Destroy window
	SDL_DestroyWindow(m_window_handle);
	m_window_handle = nullptr;

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

void WindowsApp::updateScreenSurface(const std::vector<std::vector<color>> &canvas)
{
	//Update pixels
	int height = canvas.size();
	int width = canvas[0].size();
	SDL_LockSurface(m_screen_surface);
	{
		Uint32* destPixels = (Uint32*)m_screen_surface->pixels;
		for (int j = 0; j < height; ++j)
		{
			for (int i = 0; i < width; ++i)
			{
				const auto &pixel = canvas[j][i];

				Uint32 color = SDL_MapRGB(
					m_screen_surface->format,
					static_cast<uint8_t>(pixel[0] * 255),
					static_cast<uint8_t>(pixel[1] * 255),
					static_cast<uint8_t>(pixel[2] * 255));
				destPixels[(height - 1 - j) * width + i] = color;
			}
		}
	}
	SDL_UnlockSurface(m_screen_surface);
	SDL_UpdateWindowSurface(m_window_handle);
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
