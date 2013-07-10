#include  "Projectile.h"

Projectile::Projectile(Vector2f position, Vector2f speed, float damage, int ttl, VertexCollection * vertexSource, Color color, EntityTypes type) : Entity(position), shape(position, 5, color, Hexagon::FlatTopped, vertexSource)
{
	this->ttl = ttl;
	this->speed = speed;
	this->type = type;
	this->expended = false;
	this->damage = damage;
}

float Projectile::getDamage()
{
	return this->damage;
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
	return Rect<float>(position.x - 3, position.y - 3, 6, 6);
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