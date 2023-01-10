/*The MIT License (MIT)

Copyright (c) 2021-Present, Wencong Yang (yangwc3@mail2.sysu.edu.cn).

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.*/

#include <array>
#include <vector>
#include <thread>
#include <iostream>

#include "WindowsApp.h"
#include "Utils.h"

int main(int argc, char* args[])
{
	// Create window app
	constexpr int gHeight = 500;
	constexpr int gWidth = 500;
	WindowsApp::ptr winApp = WindowsApp::getInstance(gWidth, gHeight, "CGAssignment6: Mass-Spring Simulation 203337025");
	if (winApp == nullptr)
	{
		std::cerr << "Error: failed to create a window handler" << std::endl;
		return -1;
	}

	double t = 0.0f;
	std::unique_ptr<Simulator> simulator = nullptr;
	simulator.reset(new Simulator());

	bool startToSimulate = false;

	// Window app loop
	while (!winApp->shouldWindowClose())
	{
		// Process event
		winApp->processEvent();

		if (winApp->getIsMouseLeftButtonPressed())
		{
			auto pos = winApp->getMouseClickPos();
			simulator->addNewParticle(float(pos[0]) / winApp->getWidth(), 1.0f - float(pos[1]) / winApp->getHeight());
		}

		if (winApp->isGKeyBoardPressed())
		{
			startToSimulate = !startToSimulate;
		}

		// Clear screen
		winApp->clearScreen(255, 255, 255);
		simulator->drawRopeAndPoint(winApp.get());
		winApp->updateScreen();

		if (startToSimulate)
		{
			simulator->simulate();
		}

	}

	return 0;
}