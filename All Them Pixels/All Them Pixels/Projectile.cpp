#include  "Projectile.h"

Projectile::Projectile(Vector2f position, Vector2f speed) : Entity(position)
{
	Shapes::rectangle(shape, 0, position, 10, 10);

	this->speed = speed;
}

void Projectile::applyTransform(Transform transform)
{
	Entity::applyTransform(transform);

	for (int i = 0; i < 4; i++)
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
	window->draw(shape, 4, PrimitiveType::Quads);
}

void Projectile::update(UpdateInfo info)
{
	translate(speed);
}