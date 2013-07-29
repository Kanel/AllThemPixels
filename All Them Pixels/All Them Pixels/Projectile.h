#pragma once

#include "Entity.h"
#include "Shapes.h"
#include "Hexagon.h"
#include "VertexCollection.h"

using std::vector;

class Projectile : public Entity
{
protected:
	bool expended;
	Hexagon shape;
	Vector2f speed;
	int ttl;
	float damage;
	int piercing;
	int piercedTargetsIndex;
	vector<Entity *> piercedTargets;

public:
	Projectile(Vector2f position, Vector2f speed, float damage, int piercing, int ttl, VertexCollection * vertexSource, Color color = Color(0,0,0,255), EntityTypes type = ProjectileEntity);

	Vector2f getSpeed();

	float getDamage();
	int getPiercing();
	void addPiercedTarget(Entity * entity);
	bool hasPierced(Entity * entity);

	void expend() override;
	bool isExpended() override;
	bool isEtheral() override;

	void applyTransform(Transform transform) override;

	Rect<float> getBoundingBox() override;
	ConvexHull getConvexHull() override;

	void update(UpdateInfo info) override;
};