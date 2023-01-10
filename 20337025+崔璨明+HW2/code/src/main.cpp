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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "TRWindowsApp.h"
#include "TRRenderer.h"

#include <iostream>

using namespace TinyRenderer;

int main(int argc, char* args[])
{
	constexpr int width =  666;
	constexpr int height = 500;

	TRWindowsApp::ptr winApp = TRWindowsApp::getInstance(width, height, "CGAssignment2: Rasterization_20337025");

	if (winApp == nullptr)
	{
		return -1;
	}

	TRRenderer::ptr renderer = std::make_shared<TRRenderer>(width, height);

	//camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.379474f, 3.60811f);

	glm::vec3 lookAtTarget = glm::vec3(0.0f);

	renderer->setViewMatrix(TRRenderer::calcViewMatrix(cameraPos, lookAtTarget, glm::vec3(0.0, 1.0, 0.0f)));
	renderer->setProjectMatrix(TRRenderer::calcPerspProjectMatrix(45.0f, static_cast<float>(width) / height, 0.001f, 10.0f), 0.001f, 10.0f);

	//Load the rendering data
	renderer->loadDrawableMesh("model/diablo3_pose.obj");
	renderer->loadDrawableMesh("model/floor.obj");

	winApp->readyToStart();

	glm::mat4 model_mat = glm::mat4(1.0f);

	//Rendering loop
	while (!winApp->shouldWindowClose())
	{
		//Process event
		winApp->processEvent();

		//Clear frame buffer (both color buffer and depth buffer)
		renderer->clearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		//Draw call
		renderer->renderAllDrawableMeshes();

		//Display to screen
		double deltaTime = winApp->updateScreenSurface(
			renderer->commitRenderedColorBuffer(),
			width, 
			height,
			4,
			renderer->getNumberOfClipFaces(),
			renderer->getNumberOfCullFaces());

		//Model transformation
		{
			model_mat = glm::rotate(model_mat, (float)deltaTime * 0.001f, glm::vec3(0, 1, 0));
			renderer->setModelMatrix(model_mat);
		}

		//Camera operation
		{
			//Camera rotation
			if (winApp->getIsMouseLeftButtonPressed())
			{
				int deltaX = winApp->getMouseMotionDeltaX();
				glm::mat4 cameraRotMat = glm::rotate(glm::mat4(1.0f), -deltaX * 0.001f, glm::vec3(0, 1, 0));
				cameraPos = glm::vec3(cameraRotMat * glm::vec4(cameraPos, 1.0f));
				renderer->setViewMatrix(TRRenderer::calcViewMatrix(cameraPos, lookAtTarget, glm::vec3(0.0, 1.0, 0.0f)));
			}

			//Camera zoom in and zoom out
			if (winApp->getMouseWheelDelta() != 0)
			{
				glm::vec3 dir = glm::normalize(cameraPos - lookAtTarget);
				float dist = glm::length(cameraPos - lookAtTarget);
				glm::vec3 newPos = cameraPos + (winApp->getMouseWheelDelta() * 0.1f) * dir;
				if (glm::length(newPos - lookAtTarget) > 1.0f)
				{
					cameraPos = newPos;
					renderer->setViewMatrix(TRRenderer::calcViewMatrix(cameraPos, lookAtTarget, glm::vec3(0.0, 1.0, 0.0f)));
				}
			}
		}
	}

	renderer->unloadDrawableMesh();

	return 0;
}
