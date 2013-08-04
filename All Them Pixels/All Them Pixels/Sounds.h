#pragma once

#include "Config.h"
#include <SFML/Audio.hpp>
#include <list>
#include <direct.h>

using std::list;
using namespace sf;

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
	int volumeMusic;
	int volumeEffects;
	int volumeUI;
	Music music;
	SoundBuffer coinBuffer;
	SoundBuffer hurtBuffer;
	SoundBuffer selectBuffer;
	SoundBuffer shootBuffer;
	list<Sound *> soundQueue;

public:
	Sounds();

	void play(SoundTypes what, Vector2f = Vector2f(0,0));
};