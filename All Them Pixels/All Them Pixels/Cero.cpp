#include "Cero.h"

Cero::Cero(Vector2f position, int radius)
{
	float angle;
	float currentAngle = 0;

	this->radius = radius;

	centerSides = 32;
	centerRadius = 10;	
	spirals = 5;
	spiralSegments = 200;
	spiralLength = 750;
	particleCount = (spiralSegments - 1) * 6;
	particles = new Vertex*[spirals];

	for(int i = 0; i < spirals; i++)
	{
		particles[i] = new Vertex[particleCount];
	}
	centerCircle = new Vertex[centerSides + 2];
	angle = (M_PI * 2) / centerSides;
	rotation = 0;
	spiralBend = 0;
}

Cero::~Cero()
{
	for(int i = 0; i < spirals; i++)
	{
		delete[] particles[i];
	}
	delete[] particles;
	delete[] centerCircle;
}

void Cero::update(UpdateInfo info)
{
	float currentAngle = 0;
	float angle = (M_PI * 2) / centerSides;
	float progress = (info.elapsedGameTime < 750) ? 1 : 1 - (info.elapsedGameTime - 750) / 1000.0f;
	float spiralAngle = 0;
	float spiralAngleIncrement = (360 / spirals);

	progress = (progress < 0) ? 0 : progress;
	activeSegemnts = spiralSegments * progress;
	rotation = info.elapsedGameTime / 25.0f;
	spiralBend = 30 + (20 * log10(info.elapsedGameTime / 150.0f));	
	centerRadius = radius - radius * exp(-info.elapsedGameTime / 250.0f);

	for (int a = 0; a < spirals; a++, spiralAngle += spiralAngleIncrement)
	{
		Vector2f A;
		Vector2f B;
		Vector2f AA;

		for (int i = 0, index = 0; i < activeSegemnts; i++)
		{			
			Vector2f C;
			Vector2f D;
			Vector2f CC;
			Transform transformLeft;
			Transform transformRight;
			Vector2f position = Vector2f(-centerRadius + 1, i * (spiralLength / spiralSegments));
			float segmentRotation = rotation + spiralAngle + spiralBend * exp(-(1 * ((float)i / spiralSegments)));
			
			transformLeft.rotate(-2 + segmentRotation);
			transformRight.rotate(+2 + segmentRotation);

			C = transformLeft.transformPoint(position);
			CC = C - Vector2fMath::leftNormal(Vector2fMath::unitVector(C - A)) * 4.0f;
			D = transformRight.transformPoint(position);

			if (i != 0)
			{
				particles[a][index++] = Vertex(AA, Color(255, 0, 0));
				particles[a][index++] = Vertex(B, Color(255, 0, 0));
				particles[a][index++] = Vertex(CC, Color(255, 0, 0));
				particles[a][index++] = Vertex(CC, Color(255, 0, 0));
				particles[a][index++] = Vertex(D, Color(255, 0, 0));
				particles[a][index++] = Vertex(B, Color(255, 0, 0));
			}
			A = C;
			B = D;
			AA = CC;
		}
	}

	centerCircle[0] = Vertex(Vector2f(), Color(255, 0, 0));

	for (int i = 1; i <= centerSides; i++, currentAngle += angle)
	{
		float x = cosf(currentAngle + (rotation * (M_PI * 2 / 360))) * centerRadius;
		float y = sinf(currentAngle + (rotation * (M_PI * 2 / 360))) * centerRadius;

		centerCircle[i] = Vertex(Vector2f(x,  y), Color(255, 0, 0));
	}
	float x = cosf((rotation * (M_PI * 2 / 360))) * centerRadius;
	float y = sinf((rotation * (M_PI * 2 / 360))) * centerRadius;

	centerCircle[centerSides + 1] = Vertex(Vector2f(x, y), Color(255, 0, 0));
}

void Cero::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();
	
	for(int i = 0; i < spirals; i++)
	{
		target.draw(particles[i], (activeSegemnts - 1) * 6, PrimitiveType::Triangles, states);
	}
	target.draw(centerCircle, centerSides + 2, PrimitiveType::TrianglesFan, states);
}