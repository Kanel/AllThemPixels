#include  "Projectile.h"

Projectile::Projectile(Vector2f position, Vector2f speed, float damage, int piercing, int ttl, VertexCollection * vertexSource, Color color, EntityTypes type) : Entity(position), shape(position, 5, color, Hexagon::FlatTopped, vertexSource), piercedTargets(piercing - 1)
{
	this->ttl = ttl;
	this->speed = speed;
	this->type = type;
	this->expended = false;
	this->damage = damage;
	this->piercing = piercing;

	piercedTargetsIndex = 0;
}

float Projectile::getDamage()
{
	return this->damage;
}

int Projectile::getPiercing()
{
	return piercing;
}

void Projectile::addPiercedTarget(Entity * entity)
{
	assert(piercing > 0 && "Never call this function when piercing is zero.");

	piercedTargets[piercedTargetsIndex++] = entity;
	piercing--;
}

bool Projectile::hasPierced(Entity * entity)
{
	for (auto entity : piercedTargets)
	{
		if (entity == entity)
		{
			return true;
		}
	}
	return false;
}

void Projectile::expend()
{
	expended = true;
}

bool Projectile::isExpended()
{
	return expended;
}

bool Projectile::isEtheral()
{
	return false;
}

void Projectile::applyTransform(Transform transform)
{
	Entity::applyTransform(transform);

	shape.applyTransform(transform);
}

Rect<float> Projectile::getBoundingBox()
{
	return shape.getBoundingBox();
}

ConvexHull Projectile::getConvexHull()
{
	return shape.getConvexHull();
}

void Projectile::update(UpdateInfo info)
{
	ttl--;
	
	expended = (ttl < 1);

	translate(speed);
}