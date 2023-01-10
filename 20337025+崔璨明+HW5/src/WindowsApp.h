#ifndef RTWINDOWSAPP_H
#define RTWINDOWSAPP_H

#include "SDL2/SDL.h"

#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <array>

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
	std::array<int, 2> getMouseClickPos() const { return { m_mouse_click_x, m_mouse_click_y }; }

	bool getIsMouseLeftButtonPressed() 
	{
		bool ans = m_mouse_left_button_pressed;
		m_mouse_left_button_pressed = false;
		return ans;
	}

	bool isGKeyBoardPressed() 
	{
		bool ans = m_g_keyboard_pressed;
		m_g_keyboard_pressed = false; 
		return ans;
	}

	bool isCKeyBoardPressed()
	{
		bool ans = m_c_keyboard_pressed;
		m_c_keyboard_pressed = false;
		return ans;
	}

	//Draw
	void clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void drawRectFill(Uint8 r, Uint8 g, Uint8 b, int x, int y, int w, int h);
	void drawRectLine(Uint8 r, Uint8 g, Uint8 b, int x, int y, int w, int h);
	void drawLine(Uint8 r, Uint8 g, Uint8 b, int x0, int y0, int x1, int y1);
	void drawPoint(Uint8 r, Uint8 g, Uint8 b, int x, int y);
	void updateScreen();

	static WindowsApp::ptr getInstance();
	static WindowsApp::ptr getInstance(int width, int height, const std::string title = "winApp");

private:

	//Mouse tracking
	int m_last_mouse_x, m_last_mouse_y;
	int m_mouse_delta_x, m_mouse_delta_y;
	int m_mouse_click_x, m_mouse_click_y;
	bool m_mouse_left_button_pressed = false;
	bool m_c_keyboard_pressed = false;
	bool m_g_keyboard_pressed = false;
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
	SDL_Renderer* m_renderer_handle = nullptr;

	//Singleton pattern
	static WindowsApp::ptr m_instance;
};

#endif
