#ifndef _SPHERE_SIMULATION_H_
#define _SPHERE_SIMULATION_H_

#include "Behavior.h"
#include "PhysicEngine.h"
#include "GlobalVariables.h"
#include "Renderer.h"
#include "World.h"

#define RADIUS 2.0f
#define DISTANCE 5.0f

struct SConstraint
{
	SConstraint(CPolygonPtr _A, CPolygonPtr _B)
		: A(_A), B(_B){}

	CPolygonPtr A, B;
	float distance = DISTANCE;
};

class CSphereSimulation : public CBehavior
{
private:

	void InitChain(size_t count, const sf::Vector2f& start)
	{
		for (size_t i = 0; i < count; ++i)
		{
			m_chain.push_back(AddCircle(start + sf::Vector2f(0.0f, -(float)i * DISTANCE)));
		}
	}

	void SolveChainConstraints()
	{
		//m_chain[0]->speed = sf::Vector2f();
	}

	virtual void Start() override
	{
		for (float x = -12.0f; x < 12.0f; x += 5.0f)
		{
			for (float y = -22.0f; y < 22.0f; y += 10.0f)
			{
				AddCircle(sf::Vector2f(x + Random(-0.1f, 0.1f), y + Random(-0.1f, 0.1f)))->speed.x = 50.0f;
			}
		}

		InitChain(8, sf::Vector2f(20.0f, 20.0f));

		gVars->pPhysicEngine->Activate(false);
	}

	virtual void Update(float frameTime) override
	{
		for (CPolygonPtr& circle : m_circles)
		{
			circle->speed.y -= 20.0f * frameTime;
			circle->speed -= circle->speed * 0.3f * frameTime;
		}

		for (size_t i = 0; i < m_circles.size(); ++i)
		{
			for (size_t j = i + 1; j < m_circles.size(); ++j)
			{
				CPolygonPtr c1 = m_circles[i];
				CPolygonPtr c2 = m_circles[j];
			
				sf::Vector2f diffPos = c2->position - c1->position;
				sf::Vector2f diffSpeed = c2->speed - c1->speed;
				if (diffPos.GetSqrLength() < 4.0f * RADIUS * RADIUS && ((diffSpeed | diffPos) < 0.0f))
				{
					// Handle collisions here
				}
			}
		}

		float hWidth = gVars->pRenderer->GetWorldWidth() * 0.5f;
		float hHeight = gVars->pRenderer->GetWorldHeight() * 0.5f;

		for (CPolygonPtr& circle : m_circles)
		{
			if (circle->position.x < -hWidth + RADIUS && circle->speed.x < 0)
			{
				circle->speed.x *= -1.0f;
			}
			else if (circle->position.x > hWidth - RADIUS && circle->speed.x > 0)
			{
				circle->speed.x *= -1.0f;
			}
			if (circle->position.y < -hHeight + RADIUS && circle->speed.y < 0)
			{
				circle->speed.y *= -1.0f;
			}
			else if (circle->position.y > hHeight - RADIUS && circle->speed.y > 0)
			{
				circle->speed.y *= -1.0f;
			}
		}
			
		SolveChainConstraints();

		for (CPolygonPtr& circle : m_circles)
		{
			circle->position += circle->speed * frameTime;
		}
	}

	CPolygonPtr AddCircle(const sf::Vector2f& pos, float radius = RADIUS)
	{
		CPolygonPtr circle = gVars->pWorld->AddSymetricPolygon(radius, 50);
		circle->density = 0.0f;
		circle->position = pos;
		m_circles.push_back(circle);

		return circle;
	}

	std::vector<CPolygonPtr>	m_circles;
	std::vector<CPolygonPtr>	m_chain;
};

#endif