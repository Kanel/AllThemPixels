#pragma once

#include "Config.h"
#include <SFML/Audio.hpp>
#include <list>
#include <direct.h>
#include <cassert>

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
	struct SoundIdentity
	{
		Sound * sound;
		SoundTypes type;
	};

protected:
	int volumeMusic;
	int volumeEffects;
	int volumeUI;
	Music music;
	SoundBuffer coinBuffer;
	SoundBuffer hurtBuffer;
	SoundBuffer selectBuffer;
	SoundBuffer shootBuffer;
	list<SoundIdentity> soundQueue;

public:
	Sounds();

	void setMusicVolume(int volume);
	void setEffectsVolume(int volume);
	void setUIVolume(int volume);

	void play(SoundTypes what, Vector2f = Vector2f(0,0));

protected:
	void updateVolume();
};