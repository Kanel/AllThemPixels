#include "ParticleSystem.h"

int ParticleSystem::getRandomInt(int min, int max)
{
	return rand() % (max - min) + min;
}

float ParticleSystem::getRandomFloat(float min, float max)
{
	return getRandomInt(min * 100.0f, max * 100.0f) / 100.0f;
}

float ParticleSystem::getRandomValue(float base, float variance)
{
	float min = base - (variance / 2);
	float max = base + (variance / 2);

	return getRandomFloat(min, max);
}

float ParticleSystem::getProgression(float a, float b, float progression)
{
	return a + ((b - a) * progression);
}

Color ParticleSystem::getColorProgression(Color a, Color b, float progression)
{
	Color color;

	color.r = getProgression(a.r, b.r, progression);
	color.g = getProgression(a.g, b.g, progression);
	color.b = getProgression(a.b, b.b, progression);
	color.a = getProgression(a.a, b.a, progression);

	return color;
}

Vector2f ParticleSystem::getRandomDirection(float base, float variance)
{
	float angle = getRandomValue(base, variance);

	return Vector2f(cosf(angle), sinf(angle));
}

void ParticleSystem::updateParticles(UpdateInfo info)
{
// Update particles
	for (std::list<Particle>::iterator it = particles.begin(); it != particles.end();)
	{
		Particle &particle = (*it);
		Vertex * vertices = &(*vertexSource)[particle.vertexOffset];
		float radius;
		float a, b, c, d;

		// Update  particle age
		particle.age += info.updateInterval;

		// Update color and size progression
		if (particle.age < particle.grow)
		{
			float progression = particle.age / particle.grow;

			particle.color = getColorProgression(color[0], color[1], progression);
			particle.size = getProgression(particle.templateSize[0], particle.templateSize[1], progression);
		}
		else if (particle.age > particle.duration - shrink)
		{
			float progression = (particle.age - (particle.duration - particle.shrink)) / particle.shrink;

			particle.color = getColorProgression(color[1], color[2], progression);
			particle.size = getProgression(particle.templateSize[1], particle.templateSize[2], progression);
		}
			
		vertices[0].color = particle.color;
		vertices[1].color = particle.color;
		vertices[2].color = particle.color;
		vertices[3].color = particle.color;

		// Update particle position
		particle.position += (systemSpeed * 0.25f + particle.speed) * (info.updateInterval / 1000.0f);		

		// Update particle visual
		radius = particle.size / 2;
		a = particle.position.x + radius;
		b = particle.position.x - radius;
		c = particle.position.y + radius;
		d = particle.position.y - radius;

		vertices[0].position = Vector2f(a, c);
		vertices[1].position = Vector2f(b, c);
		vertices[2].position = Vector2f(b, d);
		vertices[3].position = Vector2f(a, d);
		
		// Increment iterator
		it++;
	}
}

void ParticleSystem::cleanup()
{
	// Clean up
	for (std::list<Particle>::iterator it = particles.begin(); it != particles.end();)
	{
		if ((*it).age < (*it).duration)
		{
			it++;
		}
		else
		{
			vertexSource->remove((*it).vertexOffset);

			it = particles.erase(it);
		}
	}
}

void ParticleSystem::spawnCycle(UpdateInfo info)
{
	// Spawn particles
	if (age <= duration && info.elapsedGameTime - lastSpawnEvent >= spawnRate)
	{
		int particlesToSpawn = (info.elapsedGameTime - lastSpawnEvent) / spawnRate;

		for (int i = 0; i < particlesToSpawn; i++)
		{
			spawnParticle();
		}

		lastSpawnEvent = info.elapsedGameTime;
	}
}

ParticleSystem::ParticleSystem(int start, int duration, Vector2f position, Vector2f systemSpeed, VertexCollection * vertexSource) : Effect(start, duration)
{
	// Default settings
	age = 0;
	grow = 200;
	growVariance = 50;
	shrink = 600;
	shrinkVariance = 100;
	particleDuration = 1000;
	particleDurationVariance = 400;
	size[0] = 4;
	size[1] = 10;
	size[2] = 4;
	sizeVariance[0] = 8;
	sizeVariance[1] = 10;
	sizeVariance[2] = 4;
	direction = 0;
	directionVariance = 6.29f;
	speed = 100;
	speedVariance = 100;
	spawnRate = 1500;
	lastSpawnEvent = 0;
	color[0] = Color(0, 0, 255, 150);
	color[1] = Color(0, 0, 255, 50);
	color[2] = Color(0, 0, 255, 15);

	this->position = position;
	this->vertexSource = vertexSource;
	this->systemSpeed = systemSpeed;

	// Make sure to initilize random (should unravel why)
	srand (time(NULL));
}

ParticleSystem::~ParticleSystem()
{
	for (auto particle : particles)
	{
		vertexSource->remove(particle.vertexOffset);
	}
}

void ParticleSystem::spawnParticle()
{
	Particle particle;

	particle.age = 0;
	particle.color = color[0];
	particle.grow = getRandomValue(grow, growVariance);
	particle.duration = getRandomValue(particleDuration, particleDurationVariance);
	particle.speed = getRandomDirection(direction, directionVariance) * getRandomValue(speed, speedVariance);
	particle.shrink = getRandomValue(shrink, shrinkVariance);
	particle.size = getRandomValue(size[0], sizeVariance[0]);
	particle.templateSize[0] = particle.size;
	particle.templateSize[1] = getRandomValue(size[1], sizeVariance[1]);
	particle.templateSize[2] = getRandomValue(size[2], sizeVariance[2]);

	// Shape
	particle.position = position;
	particle.vertexOffset = vertexSource->add();

	particles.push_back(particle);
}

void ParticleSystem::despawnParticle(Particle * particle)
{
	for (std::list<Particle>::iterator it = particles.begin(); it != particles.end();)
	{
		if (&(*it) == particle)
		{
			it = particles.erase(it);

			break;
		}
		else
		{
			it++;
		}
	}
}

void ParticleSystem::update(UpdateInfo info)
{
	age += info.updateInterval;	
	
	updateParticles(info);
	cleanup();
	spawnCycle(info);
}