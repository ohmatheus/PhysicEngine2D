#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <GL/glew.h>
#include <vector>
#include <memory>


#include "Maths.h"


struct SCollision;

class CPolygon
{
	struct ProjectionLimits
	{
		float min;
		Vec2 minPoint;
		float max;
		Vec2 maxPoint;
	};

private:
	friend class CWorld;

	CPolygon(size_t index);
public:
	~CPolygon();

	Vec2				position;
	Mat2				rotation;
	std::vector<Vec2>	points;
	AABB				aabb;

	void				Build();
	void				Draw();
	size_t				GetIndex() const;

	float				GetArea() const;
	float				GetInvMass() const;

	Vec2				TransformPoint(const Vec2& point) const;
	Vec2				InverseTransformPoint(const Vec2& point) const;

	bool				IsPointInside(const Vec2& point) const;

	bool				IsLineIntersectingPolygon(const Line& line, Vec2& colPoint, float& colDist) const;
	bool				CheckCollision(const CPolygon& poly, SCollision &col) const;
	bool				CheckCollisionInternal(const CPolygon& poly, SCollision& collision, bool isMain = true) const;

	ProjectionLimits	GetProjectionLimits(Vec2 const& target) const;

	void				CreateAABB();
	void				UpdateAABB();

	float				GetMass() const;
	float				GetInertiaTensor() const;
	float				GetInvInertiaTensor() const;

	Vec2				GetPointVelocity(const Vec2& point) const;

	// Physics
	float				density;
	float				mass;
	Vec2				speed;
	float				angularVelocity = 0.0f;
	Vec2				forces;
	float				torques = 0.0f;

private:
	void				CreateBuffers();
	void				BindBuffers();
	void				DestroyBuffers();

	void				BuildLines();

	void				ComputeArea();
	void				RecenterOnCenterOfMass(); // Area must be computed
	void				ComputeLocalInertiaTensor(); // Must be centered on center of mass

	GLuint				m_vertexBufferId;
	size_t				m_index;

	std::vector<Line>	m_lines;

	float				m_signedArea;

	// Physics
	float				m_localInertiaTensor; // don't consider mass
	float				m_InvInertiaTensor = 0.f;
};

typedef std::shared_ptr<CPolygon>	CPolygonPtr;

#endif

