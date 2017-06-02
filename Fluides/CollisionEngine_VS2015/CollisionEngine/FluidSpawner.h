#ifndef _FLUID_SPAWNER_H_
#define _FLUID_SPAWNER_H_

#include "Behavior.h"
#include "FluidSystem.h"
#include "GlobalVariables.h"
#include "Renderer.h"
#include "RenderWindow.h"
#include "World.h"


class CFluidSpawner: public CBehavior
{
private:
	virtual void Update(float frameTime) override
	{
#if 0
		bool clicking = gVars->pRenderWindow->GetMouseButton(0);
		if (!m_clicking && clicking)
		{
			Vec2 mousePoint = gVars->pRenderer->ScreenToWorldPos(gVars->pRenderWindow->GetMousePos());
			CFluidSystem::Get().Spawn(mousePoint - Vec2(0.5f, 0.5f), mousePoint + Vec2(0.5f, 0.5f), 10.0f, Vec2(15.0f, 15.0f));
		}
		m_clicking = clicking;
#else
		float timeBetweenEach = 1 / m_ParticlePerSecond;


		if (gVars->pRenderWindow->GetMouseButton(0))
		{
			if (!m_clicking)
			{
				m_clicking = true;
				m_TimeSinceActive = 0.f;
			}
		}

		if (m_clicking && !gVars->pRenderWindow->GetMouseButton(0))
		{
			m_clicking = false;
			m_TimeSinceActive = 0.f;
		}

		if (m_clicking)
		{
			m_TimeSinceActive += frameTime;

			size_t shouldSpawn = 0;
			if (m_TimeSinceActive > timeBetweenEach)
			{
				shouldSpawn = m_TimeSinceActive / timeBetweenEach;
				m_TimeSinceActive -= timeBetweenEach * shouldSpawn;
			}

			if (shouldSpawn > 0)
			{
				Vec2 mousePoint = gVars->pRenderer->ScreenToWorldPos(gVars->pRenderWindow->GetMousePos());
				CFluidSystem::Get().ContinuousSpawn(mousePoint - m_Min, mousePoint + m_Max, shouldSpawn, m_Speed);
			}
		}

#endif
	}
	Vec2	m_Min = Vec2(0.1f, 0.4f);
	Vec2	m_Max = Vec2(0.0f, 0.4f);
	Vec2	m_Speed = Vec2(90.0f, -10.0f);

	bool	m_clicking = false;
	float	m_ParticlePerSecond = 500.f;
	float	m_TimeSinceActive = 0.f;
};


#endif