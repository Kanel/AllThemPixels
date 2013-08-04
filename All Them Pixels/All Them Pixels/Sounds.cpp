#include "Sounds.h"

using namespace sf;

Sounds::Sounds()
{
	if(this->music.openFromFile(SOUNDS_BACKGROUND_MUSIC))
	{
		music.setLoop(true);
		music.play();
	}
	else ; //error loading music, no big deal.

	coinBuffer.loadFromFile(SOUNDS_ENEMY_DEATH);
	hurtBuffer.loadFromFile(SOUNDS_PLAYER_HIT);
	selectBuffer.loadFromFile(SOUNDS_SELECTED);
	shootBuffer.loadFromFile(SOUNDS_FIREING);

}

void Sounds::play(SoundTypes what, Vector2f where)
{
	Sound * sound = new Sound();

	switch (what)
	{
		case SoundTypes::EnemyDeath:
			sound->setBuffer(this->coinBuffer);
			//sound->setPosition(where.x, where.y, 0);
			break;

		case SoundTypes::PlayerHit:
			sound->setBuffer(this->hurtBuffer);
			break;

		case SoundTypes::SkillScroll:
			sound->setBuffer(this->selectBuffer);
			break;

		case SoundTypes::Shot:
			sound->setBuffer(this->shootBuffer);
			break;

		default:
			break;
	}
	sound->setPosition(where.x, where.y, 0);
	sound->setPitch(1 + (float)(rand() % 100) / 100.0f);
	sound->play();
	soundQueue.push_front(sound);

	sound = soundQueue.back();

	if (sound->getStatus() == Sound::Stopped) //not sure if works
	{
		soundQueue.pop_back();
		
		delete sound;
	}
		
	
}