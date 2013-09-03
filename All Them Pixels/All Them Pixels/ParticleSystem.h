#pragma once

#include "Effect.h"
#include "UpdateInfo.h"
#include "VertexCollection.h"
#include <list>

using std::list;

class ParticleSystem : public Effect
{
public:
	struct Particle
	{
		int vertexOffset;
		float age;
		float grow;
		float shrink;
		float duration;
		float templateSize[3];
		float size;
		Color color;
		Vector2f position;
		Vector2f speed;
	};

public:
	float grow;
	float growVariance;
	float shrink;
	float shrinkVariance;
	int particleDuration;
	int particleDurationVariance;
	float size[3];
	float sizeVariance[3];
	float direction;
	float directionVariance;
	float speed;
	float speedVariance;
	float spawnRate;
	int lastSpawnEvent;
	Color color[3];
	VertexCollection * vertexSource;
	list<Particle> particles;
	Vector2f systemSpeed;
	
private:
	int getRandomInt(int min, int max);
	float getRandomFloat(float min, float max);
	float getRandomValue(float base, float variance);
	float getProgression(float a, float b, float progression);
	Color getColorProgression(Color a, Color b, float progression);
	Vector2f getRandomDirection(float base, float variance);
	void updateParticles(UpdateInfo info);
	void cleanup();
	void spawnCycle(UpdateInfo info);

public:
	ParticleSystem(int start, int duration, Vector2f position, Vector2f systemSpeed, VertexCollection * vertexSource);
	virtual ~ParticleSystem();

	void spawnParticle();
	void despawnParticle(Particle * particle);

	void update(UpdateInfo info);
};