#include <vector>
#include <random>
#include "Boid.h"
#include "Obstacle.h"

class Goal
{
	Vec3f pos;
	float scale;
};

class Model
{
public:
	std::mt19937 MersenneTwisterPRNG;
	std::uniform_real_distribution<double> m_URD;	

	std::vector<Boid*> boids;
	std::vector<Obstacle*> objects;
	Behaviour bhvr;
	Goal follow;


	double RAND_1()
	{
		return (2.0 * m_URD(MersenneTwisterPRNG) - 1.0);    // [-1,1]
	}

	Model();
    Behaviour read_input();
    virtual void init();
    virtual void render();
	virtual void update(float dt);
};

class Model1 : public Model
{
public:
	void init() override;
};

class Model2 : public Model
{
public:
	void init() override;
};

struct Face
{
    int v_indices[3];

    Face(int i, int j, int k)
    {
        v_indices[0] = i;
        v_indices[1] = j;
        v_indices[2] = k;
    }
};

extern void reloadColorUniform(float r, float g, float b);
