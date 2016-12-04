#ifndef _SCENE_SMALL_PHYSIC_H_
#define _SCENE_SMALL_PHYSIC_H_

#include "BaseScene.h"


class CSceneSmallPhysic : public CBaseScene
{
public:
	CSceneSmallPhysic(float scale = 1.f) : CBaseScene(0.5f * scale, 10.0f * scale), m_scale(scale){}

private:
	virtual void Create() override
	{
		CBaseScene::Create();

		float size = 0.5f;

		gVars->pWorld->AddBehavior<CBehaviourSmallPhysic>(nullptr);

		Vec2 start = Vec2(0, -gVars->pRenderer->GetWorldHeight() * 0.5f + (0.5f + 0.5f * size) * m_scale) - Vec2(0, -m_scale * 0);
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 15; ++j)
			{
				CPolygonPtr p = gVars->pWorld->AddSquare(size * m_scale);
				p->position = start - Vec2(i * m_scale, -j * m_scale) * size /*+ Vec2(Random(-0.01f, 0.01f), Random(-0.01f, 0.01f)) * m_scale*/;
				p->density = 2.f;
				p->Build();
			}
		}		
		
		CPolygonPtr circle = gVars->pWorld->AddSymetricPolygon(1.0f * m_scale, 50);
		circle->position = Vec2(5.0f * m_scale, -2.5f * m_scale);

		circle->speed.x = -40.0f * m_scale;
		circle->speed.y = 0.0f * m_scale;
		circle->density = 2.f;
		circle->Build();

	}

	float m_scale;
};

#endif