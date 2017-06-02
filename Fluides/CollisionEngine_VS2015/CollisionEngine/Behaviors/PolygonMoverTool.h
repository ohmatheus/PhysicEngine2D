#ifndef _POLYGON_MOVER_TOOL_H_
#define _POLYGON_MOVER_TOOL_H_

#include "Behavior.h"
#include "PhysicEngine.h"
#include "GlobalVariables.h"
#include "Renderer.h"
#include "RenderWindow.h"
#include "World.h"

class CPolygonMoverTool : public CBehavior
{
	CPolygonPtr	GetClickedPolygon()
	{
		sf::Vector2f pt, n;
		sf::Vector2f mousePoint = gVars->pRenderer->ScreenToWorldPos(sf::Vector2f(sf::Mouse::getPosition()));
		CPolygonPtr clickedPoly;

		gVars->pWorld->ForEachPolygon([&](CPolygonPtr poly)
		{
			if (poly->IsPointInside(mousePoint))
			{
				clickedPoly = poly;
			}
		});

		return clickedPoly;
	}

	virtual void Update(float frameTime) override
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			if (!m_selectedPoly)
			{
				m_selectedPoly = GetClickedPolygon();
				m_prevMousePos = gVars->pRenderer->ScreenToWorldPos(sf::Vector2f(sf::Mouse::getPosition()));
				m_translate = sf::Mouse::isButtonPressed(sf::Mouse::Left);
				m_clickMousePos = m_prevMousePos;

				if (m_selectedPoly)
					m_clickAngle = m_selectedPoly->rotation.GetAngle();
			}
			else
			{
				sf::Vector2f mousePoint = gVars->pRenderer->ScreenToWorldPos(sf::Vector2f(sf::Mouse::getPosition()));

				if (m_translate)
				{
					m_selectedPoly->position += mousePoint - m_prevMousePos;
					m_selectedPoly->speed = sf::Vector2f();
				}
				else
				{
					sf::Vector2f from = m_clickMousePos - m_selectedPoly->position;
					sf::Vector2f to = mousePoint - m_selectedPoly->position;

					m_selectedPoly->rotation.SetAngle(m_clickAngle + from.Angle(to)); 
					m_selectedPoly->speed = sf::Vector2f();
				}

				m_prevMousePos = mousePoint;
			}
		}
		else
		{
			m_selectedPoly.reset();
		}
	}

private:
	CPolygonPtr			m_selectedPoly;
	bool				m_translate;
	sf::Vector2f		m_prevMousePos;
	sf::Vector2f		m_clickMousePos;
	float				m_clickAngle;
};

#endif