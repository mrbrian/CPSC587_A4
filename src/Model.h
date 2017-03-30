#include <vector>
#include <random>
#include "glad\glad.h"
#include "glm\common.hpp"

class Model
{
public:

	std::mt19937 MersenneTwisterPRNG;
	std::uniform_real_distribution<double> m_URD;	

	double RAND_1()
	{
		return (2.0 * m_URD(MersenneTwisterPRNG) - 1.0);    // [-1,1]
	}

    virtual void init();
    virtual void render();
	virtual void update(float dt);
	void read_input();
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