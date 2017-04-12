#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

class Behaviour
{
public:
    int num_boids;
    int num_objs;
	float rad_in;
	float rad_out;

    float weight_a;
    float weight_f;
    float weight_v;

	float fov; // radian
	float world; 
	float distr;

	Behaviour() {}

    Behaviour(
        int num_b,
        int num_o,
        float r_in,
		float r_out,
		float wa,
		float wf,
		float wv,
		float fv,
		float wb,
		float ds)
    {

        num_boids = num_b;
        num_objs = num_o;
        rad_in = r_in;

		rad_out = r_out;
		weight_a = wa;
		weight_f = wf;
		weight_v = wv;

		fov = fv;
		world = wb;
		distr = ds;
	}
};

#endif // BEHAVIOUR_H
