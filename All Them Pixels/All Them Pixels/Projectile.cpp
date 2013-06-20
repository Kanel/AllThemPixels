#include  "Projectile.h"

Projectile::Projectile(Vector2f position, Vector2f speed, int ttl) : Entity(position)
{
	Shapes::hexagon(shape,0,position,5,Color(250,250,250,100));
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
	return Rect<float>(position.x - 5, position.y - 5, 10, 10);
}

void Projectile::draw(RenderWindow * window)
{
	window->draw(shape, 6, PrimitiveType::TrianglesFan);
}

void Projectile::update(UpdateInfo info)
{
	ttl--;
	
	expended = !(ttl < 1);

	translate(speed);
}