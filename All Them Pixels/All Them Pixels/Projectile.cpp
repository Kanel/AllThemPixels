#include  "Projectile.h"

Projectile::Projectile(Vector2f position, Vector2f speed, int ttl) : Entity(position)
{
	Shapes::hexagon(shape,0,position,5,Color(0,0,0,255));
	//Shapes::rectangle(shape, 0, position, 10, 10);
	
	this->ttl = ttl;
	this->speed = speed;
	this->type = EntityTypes::ProjectileEntity;
	this->expended = false;
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