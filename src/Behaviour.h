#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

class Behaviour
{
public:
    float rad_a;
    float rad_f;
    float rad_v;

    float weight_a;
    float weight_f;
    float weight_v;

	float fov; // radian

	Behaviour() {}

	Behaviour(float ra,
		float rf,
		float rv,
		float wa,
		float wf,
		float wv,
		float fv)
	{
		rad_a = ra;
		rad_f = rf;
		rad_v = rv;
		weight_a = wa;
		weight_f = wf;
		weight_v = wv;

		fov = fv;
	}
};

#endif // BEHAVIOUR_H
