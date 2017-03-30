#include "Model.h"

// the size of one square of flag
float scale = 0.125f;

void Model5::createFaces()
{
	float adj_dist_sq = scale * scale + 0.001f;  // distance of an adjacent vertex
	for (int i = 0; i < masses.size(); i++)
	{
		Mass *a = masses[i];

		for (int j = i + 1; j < masses.size(); j++)
		{
			Mass *b = masses[j];

			float dist_sq_ba = (b->pos - a->pos).lengthSquared();

			if (dist_sq_ba > adj_dist_sq)
				continue;

			for (int k = j + 1; k < masses.size(); k++)
			{
				Mass *c = masses[k];

				float dist_sq_ca = (c->pos - a->pos).lengthSquared();

				if (dist_sq_ca > adj_dist_sq)
					continue;

				for (int n = k + 1; n < masses.size(); n++)
				{
					Mass *d = masses[n];

					float dist_sq_db = (d->pos - b->pos).lengthSquared();
					float dist_sq_dc = (d->pos - c->pos).lengthSquared();

					if (dist_sq_db > adj_dist_sq ||
						dist_sq_dc > adj_dist_sq)
						continue;

					Face f = Face(j, i, k);
					faces.push_back(f);
					f = Face(j, k, n);
					faces.push_back(f);
				}
			}
		}
	}
}

void Model5::init()
{
	time = -1;
	int w = 30;
	int h = 15;

	float max_dist_squared = 2 * scale * scale;
	Vec3f start = Vec3f(-w, h, 0) * scale / 2; 
	Mass *mass_heap = new Mass[w * h];
	Mass *prev_mass = 0;

	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
		{
			Mass *m = &mass_heap[y * w + x];
			m->mass = 0.1f;
			m->pos = start + Vec3f(x, -y, 0) * scale;
			m->vel = Vec3f(0, 0, 0);

			masses.push_back(m);
		}

	// fix 3 points to flag pole
	masses[0]->fixed = true;
	masses[h / 2]->fixed = true;
	masses[h - 1]->fixed = true;

	for (int i = 0; i < masses.size(); i++)
	{
		Mass *a = masses[i];

		for (int j = 0; j < masses.size(); j++)
		{
			if (i == j)
				continue;

			Mass *b = masses[j];
			float dist = (b->pos - a->pos).lengthSquared();

			if (dist > max_dist_squared + scale / 100)
				continue;

			Spring *s = new Spring();
            s->k = 1000;
			s->damp = 0.5f;
			s->x_rest = sqrt(dist);
			s->mass_1 = a;
			s->mass_2 = b;
			s->color = Vec3f(1, 0, 0);
			springs.push_back(s);
		}
	}

	// create faces
	createFaces();

	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
		{
			Mass *m = &mass_heap[y * w + x];
			m->pos = start + Vec3f(x, -y, RAND_1() / 100) * scale;
		}

	//for (int i = 0; i < springs.size(); i++)
	//	springs[i]->load();

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	glGenBuffers(1, &vertBufferID);
	glGenBuffers(1, &normBufferID);

	verts.clear();
	normals.clear();
	v_normals = new Vec3f[masses.size()];
	updateNormals();
	updateGPU();
}

void Model5::elecForce()
{
	for(int i = 0 ; i < masses.size(); i++)
		for (int j = i + 1; j < masses.size(); j++)
		{
			Mass *a = masses[i];
			Mass *b = masses[j];
			Vec3f d_pos = b->pos - a->pos;
			float dist_sq = d_pos.lengthSquared();
			if (dist_sq < scale * scale)
			{
				// all masses have the same charge so use some value for charge^2
				float charge_sq = 0.05f;
				d_pos.normalize();
				Vec3f f = d_pos * 0.5f * charge_sq / dist_sq;
			
				a->force -= f;
				b->force += f;
			}
		}
}
void Model5::windForce()
{
	Vec3f wind_force = pow(sin(time) + 1.25f, 2) * Vec3f(1, 0, 0);

	for (int i = 0; i < faces.size(); i++)
	{
		Mass *a = masses[faces[i].v_indices[0]];
		Mass *b = masses[faces[i].v_indices[1]];
		Mass *c = masses[faces[i].v_indices[2]];

		for (int j = 0; j < 3; j++)
		{
			Vec3f norm = v_normals[faces[i].v_indices[j]];
			//masses[faces[i].v_indices[j]]->force += abs(wind_force.dotProduct(norm)) * wind_force;
			masses[faces[i].v_indices[j]]->force += wind_force.dotProduct(norm) * norm;
		}
	}
}

void Model5::update(float dt)
{
	time += dt;
	windForce();
	elecForce();
	Model::update(dt);
}

void Model5::updateGPU()
{
	//upload to gpu
	glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(Vec3f) * verts.size(), // byte size of Vec3f, 3 of them
		verts.data(),      // pointer (Vec3f*) to contents of verts
		GL_STREAM_DRAW);   // Usage pattern of GPU buffer

	glEnableVertexAttribArray(0); // match layout # in shader
	glVertexAttribPointer(0,        // attribute layout # above
		3,        // # of components (ie XYZ )
		GL_FLOAT, // type of components
		GL_FALSE, // need to be normalized?
		0,        // stride
		(void *)0 // array buffer offset
	);

	glBindBuffer(GL_ARRAY_BUFFER, normBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(Vec3f) * normals.size(), // byte size of Vec3f, 3 of them
		normals.data(),      // pointer (Vec3f*) to contents of verts
		GL_STREAM_DRAW);   // Usage pattern of GPU buffer

	glEnableVertexAttribArray(1); // match layout # in shader
	glVertexAttribPointer(1,        // attribute layout # above
		3,        // # of components (ie XYZ )
		GL_FLOAT, // type of components
		GL_FALSE, // need to be normalized?
		0,        // stride
		(void *)0 // array buffer offset
	);
}

void Model5::updateNormals()
{
	normals.clear();
	// zero the normals
	for (int i = 0; i < masses.size(); i++)
		v_normals[i] = Vec3f(0, 0, 0);

	// add up the per-face normals for each vertex
	for (int i = 0; i < faces.size(); i++)
	{
		int idx_1 = faces[i].v_indices[0];
		int idx_2 = faces[i].v_indices[1];
		int idx_3 = faces[i].v_indices[2];
		Mass *a = masses[idx_1];
		Mass *b = masses[idx_2];
		Mass *c = masses[idx_3];

		Vec3f norm = calcNormal(a->pos, b->pos, c->pos);
		v_normals[idx_1] += norm;
		v_normals[idx_2] += norm;
		v_normals[idx_3] += norm;
	}

	// normalize & push per-face normals to GPU list
	for (int i = 0; i < faces.size(); i++)
	{
		int idx_1 = faces[i].v_indices[0];
		int idx_2 = faces[i].v_indices[1];
		int idx_3 = faces[i].v_indices[2];

		(&v_normals[idx_1])->normalize();
		(&v_normals[idx_2])->normalize();
		(&v_normals[idx_3])->normalize();

		normals.push_back(v_normals[idx_1]);
		normals.push_back(v_normals[idx_2]);
		normals.push_back(v_normals[idx_3]);
	}
}

void Model5::render()
{
	verts.clear();	

	for (int i = 0; i < faces.size(); i++)
	{
		int idx_1 = faces[i].v_indices[0];
		int idx_2 = faces[i].v_indices[1];
		int idx_3 = faces[i].v_indices[2];
		Mass *a = masses[idx_1];
		Mass *b = masses[idx_2];
		Mass *c = masses[idx_3];

		verts.push_back(a->pos);
		verts.push_back(b->pos);
		verts.push_back(c->pos);
	}
	updateNormals();

	reloadColorUniform(0.5f, 0.5f, 0.5f);

	// Use VAO that holds buffer bindings
	// and attribute config of buffers
	glBindVertexArray(vaoID);
	updateGPU();

	// Draw Quads, start at vertex 0, draw 4 of them (for a quad)
	glDrawArrays(GL_TRIANGLES, 0, verts.size());

	glBindVertexArray(0);
}

