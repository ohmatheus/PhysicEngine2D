#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIABLES_H_

#include <SFML\Graphics.hpp>

struct SGlobalVariables
{
	//class CRenderWindow*	pRenderWindow;
	class sf::RenderWindow*	pSFMLRenderWindow;
	class CRenderer*		pRenderer;
	class CWorld*			pWorld;
	class CSceneManager*	pSceneManager;
	class CPhysicEngine*	pPhysicEngine;

	bool					bDebug;
};

extern SGlobalVariables*	gVars;

#endif