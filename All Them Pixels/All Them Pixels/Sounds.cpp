#include "Sounds.h"

using namespace sf;

Sounds::Sounds()
{
	volumeMusic = SOUNDS_VOLUME_MUSIC;
	volumeEffects = SOUNDS_VOLUME_EFFECTS;
	volumeUI = SOUNDS_VOLUME_UI;

	if(music.openFromFile(SOUNDS_BACKGROUND_MUSIC))
	{
		music.setVolume(volumeMusic);
		music.setLoop(true);
		music.play();
	}
	else ; //error loading music, no big deal.

	coinBuffer.loadFromFile(SOUNDS_ENEMY_DEATH);
	hurtBuffer.loadFromFile(SOUNDS_PLAYER_HIT);
	selectBuffer.loadFromFile(SOUNDS_SELECTED);
	shootBuffer.loadFromFile(SOUNDS_FIREING);

}

void Sounds::setMusicVolume(int volume)
{
	bool changed = volumeMusic != volume;

	volumeMusic = volume;

	if (changed)
	{
		updateVolume();
	}
}

void Sounds::setEffectsVolume(int volume)
{
	bool changed = volumeEffects != volume;

	volumeEffects = volume;

	if (changed)
	{
		updateVolume();
	}
}

void Sounds::setUIVolume(int volume)
{
	bool changed = volumeUI != volume;

	volumeUI = volume;

	if (changed)
	{
		updateVolume();
	}
}

void Sounds::play(SoundTypes what, Vector2f where)
{
	Sound * sound = new Sound();
	SoundIdentity identity;

	switch (what)
	{
		case SoundTypes::EnemyDeath:
			sound->setBuffer(this->coinBuffer);
			sound->setVolume(volumeEffects);
			break;

		case SoundTypes::PlayerHit:
			sound->setBuffer(this->hurtBuffer);
			sound->setVolume(volumeEffects);
			break;

		case SoundTypes::SkillScroll:
			sound->setBuffer(this->selectBuffer);
			sound->setVolume(volumeUI);
			break;

		case SoundTypes::Shot:
			sound->setBuffer(this->shootBuffer);
			sound->setVolume(volumeEffects);
			break;

		default:
			assert(!"Someone forgot to update this switch after adding a new sound type.");
			break;
	}
	sound->setPosition(where.x, where.y, 0);
	sound->setPitch(1 + (float)(rand() % 100) / 100.0f);
	sound->play();

	identity.sound = sound;
	identity.type = what;

	soundQueue.push_front(identity);

	identity = soundQueue.back();

	if (identity.sound->getStatus() == Sound::Stopped) //not sure if works
	{
		soundQueue.pop_back();
		
		delete identity.sound;
	}
}

void Sounds::updateVolume()
{
	for (auto identity : soundQueue)
	{
		switch (identity.type)
		{
			case SoundTypes::EnemyDeath:
				identity.sound->setVolume(volumeEffects);
				break;

			case SoundTypes::PlayerHit:
				identity.sound->setVolume(volumeEffects);
				break;

			case SoundTypes::SkillScroll:
				identity.sound->setVolume(volumeUI);
				break;

			case SoundTypes::Shot:
				identity.sound->setVolume(volumeEffects);
				break;

			default:
				assert(!"Someone forgot to update this switch after adding a new sound type.");
				break;
		}
	}
	music.setVolume(volumeMusic);
}