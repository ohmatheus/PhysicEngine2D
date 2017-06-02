#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "GlobalVariables.h"
#include "SDLRenderWindow.h"
#include "PhysicEngine.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "World.h"
#include <SFML/Graphics.hpp>


void InitApplication(int width, int height, float worldHeight)
{
	gVars = new SGlobalVariables();

	//gVars->pRenderWindow = new CSDLRenderWindow(width, height);
	gVars->pSFMLRenderWindow = new sf::RenderWindow(sf::VideoMode(width, height), "Fluids and Collision");
	gVars->pRenderer = new CRenderer(worldHeight);
	gVars->pSceneManager = new CSceneManager();
	gVars->pPhysicEngine = new CPhysicEngine();

	gVars->bDebug = false;
}

void RunApplication()
{
	//gVars->pRenderWindow->Init();

	while (gVars->pSFMLRenderWindow->isOpen())
	{
		sf::Event event;
		while (gVars->pSFMLRenderWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				gVars->pSFMLRenderWindow->close();

		}
		// Update stuf
		gVars->pRenderer->Update();
		//SDL_GL_SwapWindow(window);

		//window.draw(shape);
		gVars->pSFMLRenderWindow->display();
	}

	gVars->pRenderer->Reset();

}

#endif