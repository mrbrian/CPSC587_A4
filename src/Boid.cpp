#include "Boid.h"

 Boid::Boid()
 {

 }

float Boid::linear_weight(Boid *b, Behaviour *bhvr)
{
    float r = (b->pos - pos).length();
    float r_inner = -1;
    float r_outer = -1;

    if (r < bhvr->rad_a)
        r_inner = std::min(bhvr->rad_a, bhvr->rad_f);
    r_inner = std::min(r_inner, bhvr->rad_v);

    float result = (r - r_inner) / (r_outer - r_inner);
    return result;
}

//Vec3f Boid::following(Boid *b)

void Boid::update(std::vector<Boid*> *boids, Behaviour *bhvr)
{
    for (int j = 0; j < boids->size(); j++)
    {
        Boid &b = *(*boids)[j];

        // get boids within radius
/*
        Vec3f heading_f = following();
        Vec3f heading_v = velocity();
        Vec3f heading_a = avoid();
        */
    }
}
