#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <GL/glew.h>
#include <vector>
#include <memory>

#include "Maths.h"

class CPolygon
{
private:
	friend class CWorld;

	CPolygon(size_t index);
public:
	~CPolygon();

	sf::Vector2f				position;
	Mat2						rotation;
	std::vector<sf::Vector2f>	points;

	void				Build();
	void				Draw();
	size_t				GetIndex() const;

	float				GetArea() const;

	sf::Vector2f				TransformPoint(const sf::Vector2f& point) const;
	sf::Vector2f				InverseTransformPoint(const sf::Vector2f& point) const;

	// if point is outside then returned distance is negative (and doesn't make sense)
	bool				IsPointInside(const sf::Vector2f& point) const;

	bool				CheckCollision(const CPolygon& poly, sf::Vector2f& colPoint, sf::Vector2f& colNormal, float& colDist) const;

	// Physics
	float				density;
	sf::Vector2f				speed;

private:
	void				CreateBuffers();
	void				BindBuffers();
	void				DestroyBuffers();

	void				BuildLines();

	GLuint				m_vertexBufferId;
	size_t				m_index;

	std::vector<Line>	m_lines;
};

typedef std::shared_ptr<CPolygon>	CPolygonPtr;

#endif