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
	constexpr int gHeight = 600;
	constexpr int gWidth = 800;
	WindowsApp::ptr winApp = WindowsApp::getInstance(gWidth, gHeight, "CGAssignment5: Bezier Curve 20337025");
	if (winApp == nullptr)
	{
		std::cerr << "Error: failed to create a window handler" << std::endl;
		return -1;
	}

	double t = 0.0f;
	std::unique_ptr<BezierCurve> bezier = nullptr;
	bezier.reset(new BezierCurve());

	constexpr double gDt = 0.001f;
	bool gStartToGenerated = false;

	// Window app loop
	while (!winApp->shouldWindowClose())
	{
		// Process event
		winApp->processEvent();

		if (winApp->isGKeyBoardPressed() && bezier->numControlPoints() > 2)
		{
			gStartToGenerated = true;
			t = 0.0f;
			bezier->nextBezierCurve();
		}
		else if (winApp->isCKeyBoardPressed())
		{
			gStartToGenerated = false;
			bezier->clearAll();
		}

		// Clear screen
		winApp->clearScreen(0, 0, 0);
		bezier->drawControlPointsAndLines(winApp.get());
		bezier->drawBezierCurvePoints(winApp.get());

		if (!gStartToGenerated)
		{
			// Collect the points which were clicked by mouse
			if (winApp->getIsMouseLeftButtonPressed())
			{
				auto pos = winApp->getMouseClickPos();
				bezier->addControlPoints(pos[0], pos[1]);
			}
		}
		else
		{
			// Start to generate the Beizer curve 

			if (t <= 1.0)
			{
				bezier->evaluate(t);
				bezier->drawAuxiliaryLines(winApp.get());

				t += gDt;
				SDL_Delay(1);
			}
			else
			{
				bezier->clearControlPoints();
				gStartToGenerated = false;
			}
		}

		// Update to screen
		winApp->updateScreen();

	}

	return 0;
}