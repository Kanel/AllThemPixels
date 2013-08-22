#include  "Projectile.h"

Projectile::Projectile(Vector2f position, Vector2f speed, float damage, int piercing, int ttl, VertexCollection * vertexSource, Color color, EntityTypes type) : Entity(position), shape(position, 5, color, Hexagon::FlatTopped, vertexSource), piercedTargets(piercing)
{
	this->ttl = ttl;
	this->speed = speed;
	this->type = type;
	this->expended = false;
	this->damage = damage;
	this->piercing = piercing;

	piercedTargetsIndex = 0;
	float morphlength = log(Vector2fMath::length(speed)) / log(5.0f);
	float morphwidth = log(damage) / log(5.0f);
	shape.morph(morphlength, morphwidth);
	rotate(Vector2fMath::getAngle(speed),position);
}

Vector2f Projectile::getSpeed()
{
	return speed;
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

	expended = piercing <= 0;
}

bool Projectile::hasPierced(Entity * entity)
{
	for (auto target : piercedTargets)
	{
		if (target == entity)
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

void Projectile::update(UpdateInfo info, Controls * controls)
{
	ttl--;
	
	expended = (ttl < 1);

	translate(speed * (info.updateInterval / 1000.0f));
}