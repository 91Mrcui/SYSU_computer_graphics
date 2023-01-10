#ifndef RTWINDOWSAPP_H
#define RTWINDOWSAPP_H

#include "SDL2/SDL.h"

#include <string>
#include <sstream>
#include <vector>
#include <memory>

#include "vec3.h"

class WindowsApp final
{
private:
	WindowsApp() = default;

	WindowsApp(WindowsApp&) = delete;
	WindowsApp& operator=(const WindowsApp&) = delete;
	bool setup(int width, int height, std::string title);

public:
	typedef std::shared_ptr<WindowsApp> ptr;

	~WindowsApp();

	//Event
	void processEvent();
	bool shouldWindowClose() const { return m_quit; }
	int getMouseMotionDeltaX() const { return m_mouse_delta_x; }
	int getMouseMotionDeltaY() const { return m_mouse_delta_y; }
	int getMouseWheelDelta() const { return m_wheel_delta; }
	bool getIsMouseLeftButtonPressed() const { return m_mouse_left_button_pressed; }

	void updateScreenSurface(const std::vector<std::vector<color>> &canvas);

	static WindowsApp::ptr getInstance();
	static WindowsApp::ptr getInstance(int width, int height, const std::string title = "winApp");

private:

	//Mouse tracking
	int m_last_mouse_x, m_last_mouse_y;
	int m_mouse_delta_x, m_mouse_delta_y;
	bool m_mouse_left_button_pressed = false;
	int m_last_wheel_pos;
	int m_wheel_delta;

	//Screen size
	int m_screen_width;
	int m_screen_height;

	bool m_quit = false;

	//Window title
	std::string m_window_title;

	//Event handler
	SDL_Event m_events;

	//Window handler
	SDL_Window* m_window_handle = nullptr;
	SDL_Surface* m_screen_surface = nullptr;

	//Singleton pattern
	static WindowsApp::ptr m_instance;
};

#endif
