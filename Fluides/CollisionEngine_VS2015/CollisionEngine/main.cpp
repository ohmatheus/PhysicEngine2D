// CollisionEngine.cpp : définit le point d'entrée pour l'application console.
//
#include "stdafx.h"


#include <iostream>
#include <string>

#include "Application.h"
#include "SceneManager.h"
#include "Scenes/SceneDebugCollisions.h"
#include "Scenes/SceneBouncingPolys.h"
#include "Scenes/SceneSpheres.h"
#include "SceneFluid.h"
#include <SFML\Graphics.hpp>

/*
* Entry point
*/
int _tmain(int argc, char** argv)
{

	/*sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}*/


	InitApplication(1260, 768, 50.0f);

	gVars->pSceneManager->AddScene(new CSceneFluid());


	RunApplication();
	return 0;
}

