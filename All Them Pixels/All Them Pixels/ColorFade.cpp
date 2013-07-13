#include "ColorFade.h"

ColorFade::ColorFade(Color color)
{
	this->color = color;

	rest[0] = 0;
	rest[1] = 0;
	rest[2] = 0;
	rest[3] = 0;
}

void ColorFade::add(Fade fade)
{
	fades.push_back(fade);
}

void ColorFade::clear(Color color)
{
	this->color = color;

	rest[0] = 0;
	rest[1] = 0;
	rest[2] = 0;
	rest[3] = 0;

	fades.clear();
}

Color ColorFade::getColor()
{
	return color;
}

void ColorFade::update(UpdateInfo info)
{
	for (list<Fade>::iterator it = fades.begin();  it != fades.end();)
	{
		Fade * fade = &*it;

		if (fade->age >= fade->lifetime)
		{
			it = fades.erase(it);
		}
		else
		{
			if (fade->delay < fade->age)
			{
				Color change;

				rest[0] += fade->r;
				rest[1] += fade->g;
				rest[2] += fade->b;
				rest[3] += fade->a;

				change = Color(rest[0], rest[1], rest[2], rest[3]);

				rest[0] -= change.r;
				rest[1] -= change.g;
				rest[2] -= change.b;
				rest[3] -= change.a;

				color += change;
			}
			fade->age += info.updateInterval;

			it++;
		}
	}
}