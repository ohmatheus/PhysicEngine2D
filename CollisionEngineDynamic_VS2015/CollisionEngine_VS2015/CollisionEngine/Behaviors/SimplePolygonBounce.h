#ifndef _SIMPLE_COLLISION_BOUNCE_H_
#define _SIMPLE_COLLISION_BOUNCE_H_

#include "Behavior.h"
#include "PhysicEngine.h"
#include "GlobalVariables.h"
#include "Renderer.h"
#include "World.h"
#include "Polygon.h"

class CSimplePolygonBounce : public CBehavior
{
private:
	virtual void Update(float frameTime) override
	{
		gVars->pPhysicEngine->ForEachCollision([&](const SCollision& collision)
		{
			collision.polyA->position += collision.normal * collision.distance * -0.5f;
			collision.polyB->position += collision.normal * collision.distance * 0.5f;

			collision.polyA->speed.Reflect(collision.normal);
			collision.polyB->speed.Reflect(collision.normal);
		});

		float hWidth = gVars->pRenderer->GetWorldWidth() * 0.5f;
		float hHeight = gVars->pRenderer->GetWorldHeight() * 0.5f;

		gVars->pWorld->ForEachPolygon([&](CPolygonPtr poly)
		{
			poly->position += poly->speed * frameTime;

			if (poly->position.x < -hWidth)
			{
				poly->position.x = -hWidth;
				poly->speed.x *= -1.0f;
			}
			else if (poly->position.x > hWidth)
			{
				poly->position.x = hWidth;
				poly->speed.x *= -1.0f;
			}
			if (poly->position.y < -hHeight)
			{
				poly->position.y = -hHeight;
				poly->speed.y *= -1.0f;
			}
			else if (poly->position.y > hHeight)
			{
				poly->position.y = hHeight;
				poly->speed.y *= -1.0f;
			}
		});
	}
};


class CBehaviourSmallPhysic : public CBehavior
{
private:
	virtual void Update(float frameTime) override
	{
		gVars->pPhysicEngine->ForEachCollision([&](const SCollision& collision)
		{
			CPolygonPtr polyA = collision.polyA;
			CPolygonPtr polyB = collision.polyB;

			float		invMass1 = polyA->GetInvMass();
			float		invMass2 = polyB->GetInvMass();

			Vec2		colNormal = collision.normal;
			
			Vec2		initASpeed = polyA->speed;
			Vec2		initBSpeed = polyB->speed;

			Vec2		rA = collision.point - polyA->position;
			Vec2		rB = collision.point - polyB->position;

			Vec2		rAn = rA.GetNormal().Normalized();
			Vec2		rBn = rB.GetNormal().Normalized();

			Vec2		vAi = initASpeed + rAn * polyA->angularVelocity;
			Vec2		vBi = initBSpeed + rBn * polyB->angularVelocity;

			float		invTensorWorldA = polyA->GetInvInertiaTensor();
			float		invTensorWorldB = polyB->GetInvInertiaTensor();

			float		momentumA = (rA ^ colNormal) * invTensorWorldA;
			float		momentumB = (rB ^ colNormal) * invTensorWorldB;

			float		weightRotA = (rAn * momentumA) | colNormal;
			float		weightRotB = (rBn * momentumB) | colNormal;

			Vec2		tang = colNormal.GetNormal();
			float		vRel = colNormal | (vBi - vAi);
			float		vTang = tang | (vBi - vAi);
			const float InvMassAdd = invMass1 + invMass2;
			const float epsilonPlusOne = 1.6f;

			if (vRel < 0)
			{
				float damping = 0.3f;
				float correction = (collision.distance * damping) / InvMassAdd;

				polyB->position += colNormal * invMass2 * correction;
				polyA->position -= colNormal * invMass1 * correction;

				float J = (-epsilonPlusOne * vRel) / (InvMassAdd + weightRotA + weightRotB);

				polyB->speed += colNormal * (J * invMass2);
				polyA->speed -= colNormal * (J * invMass1);

				float frictionFactor = 0.6f;
				float Jfric = -vTang / InvMassAdd;
				Jfric = Clamp(Jfric, -abs(J) * frictionFactor, abs(J) * frictionFactor);

				polyB->speed += tang * (Jfric * invMass2);
				polyA->speed -= tang * (Jfric * invMass1);

				polyB->angularVelocity += J * momentumB;
				polyA->angularVelocity -= J * momentumA;
			}
		});

		float hWidth = gVars->pRenderer->GetWorldWidth() * 0.5f;
		float hHeight = gVars->pRenderer->GetWorldHeight() * 0.5f;

		gVars->pWorld->ForEachPolygon([&](CPolygonPtr poly)
		{
			if (poly->density == 0.0f)
				return;

			if (poly->position.x < -hWidth)
			{
				poly->position.x = -hWidth;
				poly->speed.x *= -1.0f;
			}
			else if (poly->position.x > hWidth)
			{
				poly->position.x = hWidth;
				poly->speed.x *= -1.0f;
			}
			if (poly->position.y < -hHeight)
			{
				poly->position.y = -hHeight;
				poly->speed.y *= -1.0f;
			}
			else if (poly->position.y > hHeight)
			{
				poly->position.y = hHeight;
				poly->speed.y *= -1.0f;
			}
		});
	}
};

#endif