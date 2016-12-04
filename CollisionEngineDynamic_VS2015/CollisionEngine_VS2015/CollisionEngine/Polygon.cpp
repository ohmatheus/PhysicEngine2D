#include "Polygon.h"
#include <GL/glu.h>

#include "InertiaTensor.h"

#include "PhysicEngine.h"
#include "Collision.h"

CPolygon::CPolygon(size_t index)
	: m_vertexBufferId(0), m_index(index), density(0.1f)
{
}

CPolygon::~CPolygon()
{
	DestroyBuffers();
}

void CPolygon::Build()
{
	m_lines.clear();

	ComputeArea();
	RecenterOnCenterOfMass();
	ComputeLocalInertiaTensor();

	CreateBuffers();
	BuildLines();
	mass = density * GetArea();

	CreateAABB();
}

void CPolygon::Draw()
{
	// Set transforms (qssuming model view mode is set)
	float transfMat[16] = {	rotation.X.x, rotation.X.y, 0.0f, 0.0f,
							rotation.Y.x, rotation.Y.y, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f,
							position.x, position.y, -1.0f, 1.0f };
	glPushMatrix();
	glMultMatrixf(transfMat);

	// Draw vertices
	BindBuffers();
	glDrawArrays(GL_LINE_LOOP, 0, points.size());
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}

size_t	CPolygon::GetIndex() const
{
	return m_index;
}

float	CPolygon::GetArea() const
{
	return fabsf(m_signedArea);
}

Vec2	CPolygon::TransformPoint(const Vec2& point) const
{
	return position + rotation * point;
}

Vec2	CPolygon::InverseTransformPoint(const Vec2& point) const
{
	return rotation.GetInverseOrtho() * (point - position);
}

bool	CPolygon::IsPointInside(const Vec2& point) const
{
	float maxDist = -FLT_MAX;

	for (const Line& line : m_lines)
	{
		Line globalLine = line.Transform(rotation, position);
		float pointDist = globalLine.GetPointDist(point);
		maxDist = Max(maxDist, pointDist);
	}

	return maxDist <= 0.0f;
}

bool	CPolygon::IsLineIntersectingPolygon(const Line& line, Vec2& colPoint, float& colDist) const
{
	//float dist = 0.0f;
	float minDist = FLT_MAX;
	Vec2 minPoint;
	float lastDist = 0.0f;
	bool intersecting = false;

	for (const Vec2& point : points)
	{
		Vec2 globalPoint = TransformPoint(point);
		float dist = line.GetPointDist(globalPoint);
		if (dist < minDist)
		{
			minPoint = globalPoint;
			minDist = dist;
		}

		intersecting = intersecting || (dist != 0.0f && lastDist * dist < 0.0f);
		lastDist = dist;
	}

	if (minDist <= 0.0f)
	{
		colDist = -minDist;
		colPoint = minPoint;
	}
	return (minDist <= 0.0f);
}

bool	CPolygon::CheckCollision(const CPolygon& poly, SCollision& collision) const
{
	collision.distance = std::numeric_limits<float>::max();

	if (!CheckCollisionInternal(poly, collision))
		return false;

	if (!poly.CheckCollisionInternal(*this, collision, false))
		return false;

	return true;
}

bool	CPolygon::CheckCollisionInternal(const CPolygon& poly, SCollision& collision, bool isMain) const
{
	Vec2 point1, point2, normal, collisionPoint;
	ProjectionLimits myLimits, polyLimits;
	float dist;

	for (Line l : m_lines)
	{
		l = l.Transform(rotation, position);
		normal = l.GetNormal().Normalized();

		myLimits = GetProjectionLimits(normal);
		polyLimits = poly.GetProjectionLimits(normal);

		if (myLimits.max - polyLimits.min < polyLimits.max - myLimits.min)
		{
			dist = myLimits.max - polyLimits.min;
			collisionPoint = IsPointInside(polyLimits.minPoint) ? polyLimits.minPoint : myLimits.maxPoint;
			if (!isMain)
				normal *= -1.f;
		}
		else
		{
			dist = polyLimits.max - myLimits.min;
			collisionPoint = poly.IsPointInside(myLimits.minPoint) ? myLimits.minPoint : polyLimits.maxPoint;
			if (isMain)
				normal *= -1.f;
		}

		if (dist < 0)
			return false;
		if (dist < collision.distance)
		{
			collision.point = collisionPoint;
			collision.distance = dist;
			collision.normal = normal;
		}
	}

	return true;
}

CPolygon::ProjectionLimits	CPolygon::GetProjectionLimits(Vec2 const& target) const
{
	ProjectionLimits limits{ std::numeric_limits<float>::max(), Vec2(), std::numeric_limits<float>::lowest(), Vec2() };
	Vec2 point;
	float dot;

	for (int i = 0; i < points.size(); ++i)
	{
		point = TransformPoint(points[i]);
		dot = target | point;
		if (limits.min > dot)
		{
			limits.min = dot;
			limits.minPoint = point;
		}
		if (limits.max < dot)
		{
			limits.max = dot;
			limits.maxPoint = point;
		}
	}

	return limits;
}

void CPolygon::UpdateAABB()
{
	aabb.Center(position);
	for (const Vec2& point : points)
	{
		aabb.Extend(TransformPoint(point));
	}
}

float CPolygon::GetMass() const
{
	return density * GetArea();
}

float CPolygon::GetInertiaTensor() const
{
	return m_localInertiaTensor * GetMass();
}

float CPolygon::GetInvInertiaTensor() const
{
	return m_InvInertiaTensor;
}

float CPolygon::GetInvMass() const
{
	return (density == 0.f) ? 0.f : 1.f / (density * GetArea());
}

Vec2 CPolygon::GetPointVelocity(const Vec2& point) const
{
	return speed + (point - position).GetNormal() * angularVelocity;
}

void CPolygon::CreateBuffers()
{
	DestroyBuffers();

	float* vertices = new float[3 * points.size()];
	for (size_t i = 0; i < points.size(); ++i)
	{
		vertices[3 * i] = points[i].x;
		vertices[3 * i + 1] = points[i].y;
		vertices[3 * i + 2] = 0.0f;
	}

	glGenBuffers(1, &m_vertexBufferId);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * points.size(), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] vertices;
}

void CPolygon::BindBuffers()
{
	if (m_vertexBufferId != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, (void*)0);
	}
}


void CPolygon::DestroyBuffers()
{
	if (m_vertexBufferId != 0)
	{
		glDeleteBuffers(1, &m_vertexBufferId);
		m_vertexBufferId = 0;
	}
}

void CPolygon::BuildLines()
{
	for (size_t index = 0; index < points.size(); ++index)
	{
		const Vec2& pointA = points[index];
		const Vec2& pointB = points[(index + 1) % points.size()];

		Vec2 lineDir = (pointA - pointB).Normalized();

		m_lines.push_back(Line(pointB, lineDir, (pointA - pointB).GetLength()));
	}
}

void CPolygon::ComputeArea()
{
	m_signedArea = 0.0f;
	for (size_t index = 0; index < points.size(); ++index)
	{
		const Vec2& pointA = points[index];
		const Vec2& pointB = points[(index + 1) % points.size()];
		m_signedArea += pointA.x * pointB.y - pointB.x * pointA.y;
	}
	m_signedArea *= 0.5f;
}

void CPolygon::RecenterOnCenterOfMass()
{
	Vec2 centroid;
	for (size_t index = 0; index < points.size(); ++index)
	{
		const Vec2& pointA = points[index];
		const Vec2& pointB = points[(index + 1) % points.size()];
		float factor = pointA.x * pointB.y - pointB.x * pointA.y;
		centroid.x += (pointA.x + pointB.x) * factor;
		centroid.y += (pointA.y + pointB.y) * factor;
	}
	centroid /= 6.0f * m_signedArea;

	for (Vec2& point : points)
	{
		point -= centroid;
	}
	position += centroid;
}

void CPolygon::ComputeLocalInertiaTensor()
{
	m_localInertiaTensor = 0.0f;
	for (size_t i = 0; i + 1 < points.size(); ++i)
	{
		const Vec2& pointA = points[i];
		const Vec2& pointB = points[i + 1];

		m_localInertiaTensor += ComputeInertiaTensor_Triangle(Vec2(), pointA, pointB);
	}
	float mass = density * GetArea();
	m_InvInertiaTensor = mass == 0.f ? 0.f : 1.f / (m_localInertiaTensor * mass);
}

void CPolygon::CreateAABB()
{
	float minX = FLT_MAX, maxX = FLT_MIN, minY = FLT_MAX, maxY = FLT_MIN;

	for (Vec2 pt : points)
	{
		minX = Min(pt.x, minX);
		maxX = Max(pt.x, maxX);
		minY = Min(pt.y, minY);
		maxY = Max(pt.y, maxY);
	}

	aabb.min = Vec2(minX, minY);
	aabb.max = Vec2(maxX, maxY);
}


