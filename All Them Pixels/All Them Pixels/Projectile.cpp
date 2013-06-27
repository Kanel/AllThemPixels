#include  "Projectile.h"

Projectile::Projectile(Vector2f position, Vector2f speed, float damage, int ttl, Color color, EntityTypes type) : Entity(position)
{
	Shapes::hexagon(shape, 0, position, 5, color, true);
	//Shapes::rectangle(shape, 0, position, 10, 10);
	
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

	for (int i = 0; i < 6; i++)
	{
		shape[i].position = transform.transformPoint(shape[i].position);
	}
}

Rect<float> Projectile::getBoundingBox()
{
	return Rect<float>(position.x - 3, position.y - 3, 6, 6);
}

ConvexHull Projectile::getConvexHull()
{
	vector<Vector2f> vertecies;

	for (int i = 0; i < 6; i++)
	{
		vertecies.push_back(shape[i].position);
	}

	return MonotoneChain::getConvexHull(vertecies);
}

void Projectile::draw(RenderWindow * window)
{
	window->draw(shape, 6, PrimitiveType::TrianglesFan);
}

void Projectile::update(UpdateInfo info)
{
	ttl--;
	
	expended = (ttl < 1);

	translate(speed);
}