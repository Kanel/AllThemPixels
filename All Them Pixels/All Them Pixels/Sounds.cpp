#include "Sounds.h"

using namespace sf;

Sounds::Sounds()
{
	char cwd[FILENAME_MAX];
	if (!_getcwd(cwd, sizeof(cwd)));//error
	std::string currentPath(cwd);
	currentPath += "\\";//replace \\ with / somehow *-)

	if(this->music.openFromFile("D:/Temp/battle_prelude.ogg"))
	{
		music.setLoop(true);
		music.play();
	}
	else ; //error loading music, no big deal.

	coinBuffer.loadFromFile("coin1.wav");
	hurtBuffer.loadFromFile("hurt1.wav");
	selectBuffer.loadFromFile("select1.wav");
	shootBuffer.loadFromFile("shoot1.wav");

}

/*
void Sounds::toggleBGMusic(bool inSafe, bool on)
{
	if(this->music.openFromFile(inSafe ? "D:/Temp/elevator_music.ogg" : "D:/Temp/battle_prelude.ogg"))
	{
		music.setLoop(true);
		music.play();
	}
	else ;
}*/

void Sounds::play(SoundTypes what, Vector2f where)
{
	/*Sound * sound = new Sound();

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
	
	sound->setPitch(1 + (float)(rand() % 100) / 100.0f);
	sound->play();
	soundQueue.push_front(sound);

	sound = soundQueue.back();
	if (sound->getStatus() == Sound::Stopped) //not sure if works
		soundQueue.pop_back();
		*/
	
}