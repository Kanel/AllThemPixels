#pragma once

#include "Config.h"
#include <SFML/Audio.hpp>
#include <list>
#include <direct.h>

enum SoundTypes
{
	Shot = 1,
	EnemyDeath = 2,
	PlayerHit = 3,
	SkillScroll = 4
};

class Sounds
{
protected:
	sf::Music music;
	sf::SoundBuffer coinBuffer;
	sf::SoundBuffer hurtBuffer;
	sf::SoundBuffer selectBuffer;
	sf::SoundBuffer shootBuffer;
	std::list<sf::Sound *> soundQueue;

public:
	Sounds();

	void play(SoundTypes what, sf::Vector2f = sf::Vector2f(0,0));
};