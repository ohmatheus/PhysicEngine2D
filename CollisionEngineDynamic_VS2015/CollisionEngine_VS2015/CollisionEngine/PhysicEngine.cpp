#include "PhysicEngine.h"

#include <iostream>
#include <string>
#include <algorithm>

#include "GlobalVariables.h"
#include "World.h"
#include "Renderer.h" // for debugging only
#include "Timer.h"

#include "BroadPhase.h"
#include "BroadPhaseBrut.h"


void	CPhysicEngine::Reset()
{
	m_pairsToCheck.clear();
	m_collidingPairs.clear();

	m_active = true;

	//m_broadPhase = new CBroadPhaseBrut;
	m_broadPhase = new SweepAndPrune();

}

void	CPhysicEngine::Activate(bool active)
{
	m_active = active;
}

void	CPhysicEngine::DetectCollisions()
{
	CTimer timer;
	timer.Start();
	CollisionBroadPhase();
	timer.Stop();
	if (gVars->bDebug)
	{
		gVars->pRenderer->DisplayText("Collision broadphase duration " + std::to_string(timer.GetDuration() * 1000.0f) + " ms");
	}

	timer.Start();
	CollisionNarrowPhase();
	timer.Stop();
	if (gVars->bDebug)
	{
		gVars->pRenderer->DisplayText("Collision narrowphase duration " + std::to_string(timer.GetDuration() * 1000.0f) + " ms, collisions : " + std::to_string(m_collidingPairs.size()));
	}
}


void	CPhysicEngine::Step(float deltaTime)
{
	deltaTime = Min(deltaTime, 1.0f / 15.0f);

	if (!m_active)
	{
		return;
	}

	Vec2 gravity(0, -9.8f);
	float elasticity = 0.6f;

	gVars->pWorld->ForEachPolygon([&](CPolygonPtr poly)
	{
		if (poly->density == 0.0f)
			return;

		poly->rotation.Rotate(RAD2DEG(poly->angularVelocity * deltaTime));
		//poly->CreateAABB();

		poly->position += poly->speed * deltaTime;
		poly->speed += gravity * deltaTime;
	});

	DetectCollisions();
}

void	CPhysicEngine::CollisionBroadPhase()
{
	m_pairsToCheck.clear();
	m_broadPhase->GetCollidingPairsToCheck(m_pairsToCheck);
}

void	CPhysicEngine::CollisionNarrowPhase()
{
	m_collidingPairs.clear();
	for (const SPolygonPair& pair : m_pairsToCheck)
	{
		SCollision collision;
		collision.polyA = pair.polyA;
		collision.polyB = pair.polyB;
		collision.distance = 0;
		if (pair.polyA->CheckCollision(*(pair.polyB), collision))
		{
			m_collidingPairs.push_back(collision);
		}
	}
}

SweepAndPrune::SweepAndPrune()
{
}

SweepAndPrune::~SweepAndPrune()
{
}

void SweepAndPrune::GetCollidingPairsToCheck(std::vector<SPolygonPair>& pairsToCheck)
{
	SortByX();
	std::vector<CPolygonPtr> &polygons = gVars->pWorld->Polygons();

	for (int i = 0; i < polygons.size(); ++i)
	{
		CPolygonPtr pm = polygons[i];
		AABB box = pm->aabb;
		for (int j = i + 1; j < polygons.size(); ++j)
		{
			CPolygonPtr pm2 = polygons[j];
			AABB box2 = pm2->aabb;
			if (box.max.x >= box2.min.x && box.max.y >= box2.min.y &&
				box.min.x < box2.max.x && box.min.y < box2.max.y)
			{
				pairsToCheck.push_back(SPolygonPair(pm, pm2));
			}
			else
				break;
		}
	}
}

AABB SweepAndPrune::CreateAABB(const CPolygonPtr poly) const
{
	AABB boundingBox;
	float minX = FLT_MAX, maxX = FLT_MIN, minY = FLT_MAX, maxY = FLT_MIN;

	for (Vec2 pt : poly->points)
	{
		minX = Min(pt.x, minX);
		maxX = Max(pt.x, maxX);
		minY = Min(pt.y, minY);
		maxY = Max(pt.y, maxY);
	}

	boundingBox.min = Vec2(minX, minY);
	boundingBox.max = Vec2(maxX, maxY);

	return boundingBox;
}

float SweepAndPrune::GetMinX(const CPolygonPtr & poly)
{
	float min = FLT_MAX;
	for (Vec2 pt : poly->points)
		min = Min(min, pt.x);
	return min;
}

void SweepAndPrune::SortByX()
{
	std::vector<CPolygonPtr> &polygons = gVars->pWorld->Polygons();
	std::sort(polygons.begin(), polygons.end(), SortFloat);
}

