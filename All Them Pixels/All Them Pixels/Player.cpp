#include "Player.h"

Player::Player(unsigned int hp, Vector2f position) : Destructible(hp, position)
{

}

void Player::applyTransform(Transform transform)
{
	
}

Rect<float> Player::getBoundingBox()
{
	return Rect<float>();
}

void Player::draw(RenderWindow * window)
{

}

void Player::update(UpdateInfo info)
{

}