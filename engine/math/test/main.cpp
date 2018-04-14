#include <vec2.h>
#include <mat3.h>
#include <mat4.h>
#include <transform.h>
#include <utility.h>
#include <simd_mat4.h>

#include <iostream>

using namespace math;

int main(int argc, const char * argv[]) {

	{
		printf("\nvec2 Tests\n\n");

		vec2f a(5.2, 9.1);
		vec2f b(1.3, 15.6);

		vec2f c = a - b;
		c.print();

		vec2f d = a.direction(b);
		d.print();
		printf("length: %f\n", d.length());
	}

	{
		printf("\nvec3 Tests\n\n");

		vec3f a(5.2, 9.1, 1.2);
		vec3f b(1.3, 15.6, 5.1);

		vec3f c = a - b;
		c.print();

		vec3f d = a.direction(b);
		d.print();
		printf("length: %f\n", d.length());

		vec3f j(0.0, 1.0, 0.0);
		vec3f k(0.0, 1.0, 0.0);

		vec3f r = j.cross(k);
		float theta = k.dot(j);

		printf("j x k = ");
		r.print();

		printf("j . k = %f", theta);
	}

	{
		printf("\nvec4 Tests\n\n");

		vec4f a(5.2, 9.1, 1.2, 7.1);
		vec4f b(1.3, 15.6, 5.1, 9.5);

		vec4f c = a - b;
		c.print();
		c = c.normalize();
		printf("length: %f\n", c.length());

		printf("sizeof(vec4f) = %lu\n\n", sizeof(vec4f));
	}

	{
		mat4f a(1, 5, 4, 7,
				3, 9, 3, 2,
				5, 8, 3, 4,
				0, 5, 1, 6);
		a.print();
		printf("\n");

		a = a * 2.0f;

		a.print();
		printf("\n");

		mat4f b = a.transpose();
		b.print();
	}

	{
		vec3f v(2.0f, 2.0f, 2.0f);
		mat3f m;

		vec3f a = m * v;

		printf("\n");
		a.print();
	}

	{
		vec4f v(2.0f, 2.0f, 2.0f, 2.0f);
		mat4f m;

		vec4f a = m * v;

		printf("\n");
		a.print();
	}

	{
		mat4f a;
		mat4f b;

		mat4f c = a * b;

		printf("\n");
		c.print();

		printf("\n");
		c[0].print();
		printf("\n");
		c[1].print();
		printf("\n");
		c[2].print();
		printf("\n");
		c[3].print();
	}

	{
		printf("Adjoint Test.\n A: \n\n");

		mat3f a(-4, -3, 3,
				 0, 2, -2,
				 1, 4, -1);

		a.print();

		printf("adj A: \n\n");
		mat3f adj = a.adjoint();
		adj.print();
		printf("\n");
	}

	{
		vec4f a(1.0f, 0.0f, 0.0f, 0.0f);
		a.print();
		printf("\n");

		mat4f rot1 = rotation(0.0f, 0.0f, radians(90.0f));
		rot1.print();
		printf("\n");

		mat4f rot2 = rotation(radians(90.0f), vec3f(0.0f, 0.0f, 1.0));
		rot2.print();
		printf("\n");

		vec4f r = rot1 * a;
		r.print();
		printf("\n");
	}

	{
		simd::mat4f m1;
		simd::mat4f m2;

		simd::mat4f m = m1 * m2;
	}

	std::cin.get();
	return 0;
}
