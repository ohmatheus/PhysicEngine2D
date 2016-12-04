#ifndef _DISPLAY_COLLISION_H_
#define _DISPLAY_COLLISION_H_

#include "Behavior.h"
#include "PhysicEngine.h"
#include "GlobalVariables.h"
#include "Renderer.h"
#include "RenderWindow.h"
#include "World.h"

#include <string>
#include <iostream>

class CDisplayCollision : public CBehavior
{
public:
	CPolygonPtr polyA;
	CPolygonPtr polyB;

private:
	virtual void Update(float frameTime) override
	{
		gVars->pPhysicEngine->Activate(false);

		SCollision col;
		if (polyA->CheckCollision(*polyB, col))
		{
			gVars->pRenderer->DisplayTextWorld("collision point", col.point);
			gVars->pRenderer->DisplayText("Collision distance : " + std::to_string(col.distance), 50, 50);
			//gVars->pRenderer->DisplayText("Normal X : " + std::to_string(normal.x), 50, 80);
			//gVars->pRenderer->DisplayText("Normal Y : " + std::to_string(normal.y), 50, 110);

			gVars->pRenderer->DrawLine(col.point, col.point + col.normal * col.distance, 1.0f, 0.0f, 1.0f);
		}
	}
};


#endif