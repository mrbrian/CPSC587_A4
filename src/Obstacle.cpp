#include "Obstacle.h"
#define _USE_MATH_DEFINES
#include <math.h>

Sphere::Sphere(Vec3f p, float r)
{
	position = p;
	radius = r;
}

Vec3f Sphere::nearest_point(Vec3f pt) 
{
	Vec3f p = pt - position;
	return p.normalized() * radius;
}

void Sphere::render()
{
	// Use VAO that holds buffer bindings
	// and attribute config of buffers
	glBindVertexArray(vaoID);
	updateGPU();

	glPointSize(2);
	// Draw Quads, start at vertex 0, draw 4 of them (for a quad)
	// color.normalize();
	//reloadColorUniform(color.x(), color.y(), color.z());
	glDrawArrays(GL_POINTS, 0, verts.size());
	glBindVertexArray(0);
}

void Sphere::load()
{
    int num_phi = 16;
    int num_theta = 16;

    for (int i = -num_phi; i < num_phi - 1; i += 2)
    {
		float phi = i * M_PI / num_phi;
		float next_phi = (i+1) * M_PI / num_phi;

		for (int j = 0; j < num_theta - 1; j++)
        {
			float theta = j * 2.0f * M_PI / num_theta;
			float next_theta = (j+1) * 2.0f * M_PI / num_theta;

			Vec3f p1 = Vec3f(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi)) * radius;
			Vec3f p2 = Vec3f(cos(next_theta) * sin(phi), sin(next_theta) * sin(phi), cos(phi)) * radius;
			Vec3f p3 = Vec3f(cos(theta) * sin(next_phi), sin(theta) * sin(next_phi), cos(next_phi)) * radius;
			Vec3f p4 = Vec3f(cos(next_theta) * sin(next_phi), sin(next_theta) * sin(next_phi), cos(next_phi)) * radius;

			verts.push_back(p3 + position);
			verts.push_back(p2 + position);
			verts.push_back(p1 + position);

			verts.push_back(p4 + position);
			verts.push_back(p3 + position);
			verts.push_back(p2 + position);
		}
    }

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vertBufferID);
	updateGPU();
}

void Sphere::updateGPU()
{
	//upload to gpu
	glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);

	glBufferData(GL_ARRAY_BUFFER,
		sizeof(Vec3f) * verts.size(), // byte size of Vec3f, 2 of them
		verts.data(),      // pointer (Vec3f*) to contents of verts
		GL_STREAM_DRAW);   // Usage pattern of GPU buffer

	glVertexAttribPointer(0,        // attribute layout # above
		3,        // # of components (ie XYZ )
		GL_FLOAT, // type of components
		GL_FALSE, // need to be normalized?
		0,        // stride
		(void *)0 // array buffer offset
	);

	glEnableVertexAttribArray(0);
}
