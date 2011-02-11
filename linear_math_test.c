#include "linear_math.h"
#include <check.h>

#define FLOATS_ARE_EQUAL(a, b) (fabs((a) - (b)) < MATH_EPSILON)

/**************************************************************************
  vec2: two-dimensional vector
**************************************************************************/

#define CHECK_VEC2(v, x, y)						\
do {									\
	fail_unless(FLOATS_ARE_EQUAL(VEC_X(v), x),			\
		    "%f X value expected, got: %f", (x), VEC_X(v));	\
	fail_unless(FLOATS_ARE_EQUAL(VEC_Y(v), y),			\
		    "%f Y value expected, got: %f", (y), VEC_Y(v));	\
} while (0)

START_TEST(test_vec2_adda)
{
	vec2_t a = {1.2f, 5.4f};
	vec2_t b = {1.3f, 4.3f};
	vec2_adda(a, b);
	CHECK_VEC2(a, 2.5f, 9.7f);
}
END_TEST

START_TEST(test_vec2_suba)
{
	vec2_t a = {4.7f, 6.9f};
	vec2_t b = {1.2f, 5.5f};
	vec2_suba(a, b);
	CHECK_VEC2(a, 3.5f, 1.4f);
}
END_TEST

START_TEST(test_vec2_mula)
{
	vec2_t a = {7.5f, 2.2f};
	vec2_t b = {2.0f, 1.5f};
	vec2_mula(a, b);
	CHECK_VEC2(a, 15.0f, 3.3f);
}
END_TEST

START_TEST(test_vec2_diva)
{
	vec2_t a = {4.8f, 3.9f};
	vec2_t b = {2.0f, 3.0f};
	vec2_diva(a, b);
	CHECK_VEC2(a, 2.4f, 1.3f);
}
END_TEST

START_TEST(test_vec2_add)
{
	vec2_t a = {1.5f, 17.4f};
	vec2_t b = {8.5f, 2.6f};
	vec2_t c;
	vec2_add(c, a, b);
	CHECK_VEC2(c, 10.0f, 20.0f);
}
END_TEST

START_TEST(test_vec2_sub)
{
	vec2_t a = {10.0f, 30.0f};
	vec2_t b = {3.7f, 5.4f};
	vec2_t c;
	vec2_sub(c, a, b);
	CHECK_VEC2(c, 6.3f, 24.6f);
}
END_TEST

START_TEST(test_vec2_mul)
{
	vec2_t a = {4.6f, 3.2f};
	vec2_t b = {5.0f, 2.5f};
	vec2_t c;
	vec2_mul(c, a, b);
	CHECK_VEC2(c, 23.0f, 8.0f);

}
END_TEST

START_TEST(test_vec2_div)
{
	vec2_t a = {7.8f, 3.0f};
	vec2_t b = {2.0f, 6.0f};
	vec2_t c;
	vec2_div(c, a, b);
	CHECK_VEC2(c, 3.9f, 0.5f);
}
END_TEST

START_TEST(test_vec2_sadda)
{
	vec2_t a = {5.4f, 7.1f};
	vec2_sadda(a, 2.7f);
	CHECK_VEC2(a, 8.1f, 9.8f);
}
END_TEST

START_TEST(test_vec2_ssuba)
{
	vec2_t a = {3.4f, 7.6f};
	vec2_ssuba(a, 4.8f);
	CHECK_VEC2(a, -1.4f, 2.8f);
}
END_TEST

START_TEST(test_vec2_smula)
{
	vec2_t a = {5.76f, 7.8f};
	vec2_smula(a, 2.0f);
	CHECK_VEC2(a, 11.52f, 15.6f);
}
END_TEST

START_TEST(test_vec2_sdiva)
{
	vec2_t a = {15.3f, 24.0f};
	vec2_sdiva(a, -3.0f);
	CHECK_VEC2(a, -5.1f, -8.0f);
}
END_TEST

START_TEST(test_vec2_sadd)
{
	vec2_t b, a = {4.6f, 7.8f};
	vec2_sadd(b, a, -0.5f);
	CHECK_VEC2(b, 4.1f, 7.3f);
}
END_TEST

START_TEST(test_vec2_ssub)
{
	vec2_t b, a = {5.8f, -1.4f};
	vec2_ssub(b, a, 17.0f);
	CHECK_VEC2(b, -11.2f, -18.4f);
}
END_TEST

START_TEST(test_vec2_smul)
{
	vec2_t b, a = {65.4f, 76.3f};
	vec2_smul(b, a, 0.5f);
	CHECK_VEC2(b, 32.7f, 38.15f);
}
END_TEST

START_TEST(test_vec2_sdiv)
{
	vec2_t b, a = {10.0f, 10.0f};
	vec2_sdiv(b, a, 4.0f);
	CHECK_VEC2(b, 2.5f, 2.5f);
}
END_TEST

START_TEST(test_vec2_set)
{
	vec2_t a;
	vec2_set(a, 5.7f, 31337.0f);
	CHECK_VEC2(a, 5.7f, 31337.0f);
}
END_TEST

START_TEST(test_vec2_neg)
{
	vec2_t b, a = {4.7f, 1.3f};
	vec2_neg(b, a);
	CHECK_VEC2(b, -4.7f, -1.3f);
}
END_TEST

START_TEST(test_vec2_copy)
{
	vec2_t b, a = {6.4f, 7.6f};
	vec2_copy(b, a);
	CHECK_VEC2(b, 6.4f, 7.6f);
}
END_TEST

START_TEST(test_vec2_length2)
{
	vec2_t a = {4.0f, 3.0f};
	float len2 = vec2_length2(a);
	fail_unless(FLOATS_ARE_EQUAL(len2, 25.0f),
		    "%f length2 expected, got: %f", 25.0f, len2);
}
END_TEST

START_TEST(test_vec2_length)
{
	vec2_t a = {4.0f, 3.0f};
	float len = vec2_length(a);
	fail_unless(FLOATS_ARE_EQUAL(len, 5.0f),
		    "%f length expected, got: %f", 5.0f, len);

}
END_TEST

START_TEST(test_vec2_normalize)
{
	vec2_t a = {7.0f, 0.0f};
	vec2_t b = {0.0f, 3.0f};
	vec2_t c = {1.0f, 1.0f};
	vec2_normalize(a);
	vec2_normalize(b);
	vec2_normalize(c);
	CHECK_VEC2(a, 1.0f, 0.0f);
	CHECK_VEC2(b, 0.0f, 1.0f);
	CHECK_VEC2(c, 0.7071067f, 0.7071067f);
}
END_TEST

START_TEST(test_vec2_distance)
{
	vec2_t a = {5.0f, 4.0f};
	vec2_t b = {1.0f, 1.0f};
	float distance = vec2_distance(a, b);
	fail_unless(FLOATS_ARE_EQUAL(distance, 5.0f),
		    "%f distance expected, got: %f", 5.0f, distance);

	vec2_set(a, 4.0f, 5.0f);
	vec2_set(b, 4.7071067f, 5.7071067f);
	distance = vec2_distance(a, b);
	fail_unless(FLOATS_ARE_EQUAL(distance, 1.0f),
		    "%f distance expected, got: %f", 1.0f, distance);
}
END_TEST

START_TEST(test_vec2_distance2)
{
	vec2_t a = {-3.0f, -4.0f};
	vec2_t b = {0.0f, 0.0f};
	float d2 = vec2_distance2(a, b);
	fail_unless(FLOATS_ARE_EQUAL(d2, 25.0f),
		    "%f distance2 expected, got: %f", 25.0f, d2);
}
END_TEST

START_TEST(test_vec2_equals)
{
	vec2_t b, a = {5.4f, 7.6f};
	vec2_copy(b, a);
	fail_unless(vec2_equals(a, b),
		    "vectors equality expected, got: (%f,%f) != (%f,%f)",
		    VEC_X(a), VEC_Y(a), VEC_X(b), VEC_Y(b));

	vec2_set(a, 5.0f, 4.0f);
	vec2_set(b, 0.0f, 4.0f);
	fail_unless(!vec2_equals(a, b),
		    "vectors unequality expected, got: (%f,%f) == (%f,%f)",
		    VEC_X(a), VEC_Y(a), VEC_X(b), VEC_Y(b));
}
END_TEST

START_TEST(test_vec2_dot)
{
	vec2_t a = {4.3f, 7.6f};
	vec2_t b = {2.0f, -2.0f};
	float dot = vec2_dot(a, b);
	fail_unless(FLOATS_ARE_EQUAL(dot, -6.6f),
		    "%f dot product expected, got: %f", -6.6f, dot);
}
END_TEST

START_TEST(test_vec2_rotate)
{
	vec2_t a = {1.0f, 0.0f};
	vec2_rotate(a, 45.0f, VEC2_ZERO);
	CHECK_VEC2(a, 0.7071067f, 0.7071067f);

	vec2_set(a, 1.0f, 0.0f);
	vec2_rotate(a, 90.0f, VEC2_ZERO);
	CHECK_VEC2(a, 0.0f, 1.0f);

	vec2_set(a, 2.0f, 0.0f);
	vec2_rotate(a, -45.0f, (vec2_t){1.0f, 0.0f});
	CHECK_VEC2(a, 1.7071067f, -0.7071067f);
}
END_TEST

/**************************************************************************
  vec3: three-dimensional vector
**************************************************************************/

#define CHECK_VEC3(v, x, y, z)						\
do {									\
	fail_unless(FLOATS_ARE_EQUAL(VEC_X(v), x),			\
		    "%f X value expected, got: %f", (x), VEC_X(v));	\
	fail_unless(FLOATS_ARE_EQUAL(VEC_Y(v), y),			\
		    "%f Y value expected, got: %f", (y), VEC_Y(v));	\
	fail_unless(FLOATS_ARE_EQUAL(VEC_Z(v), z),			\
		    "%f Z value expected, got: %f", (z), VEC_Z(v));	\
} while (0)

START_TEST(test_vec3_adda)
{
	vec3_t a = {4.6f, 4.3f, 1.2f};
	vec3_t b = {1.2f, 7.8f, 5.1f};
	vec3_adda(a, b);
	CHECK_VEC3(a, 5.8f, 12.1f, 6.3f);
}
END_TEST

START_TEST(test_vec3_suba)
{
	vec3_t a = {1.1f, 8.4f, 2.3f};
	vec3_t b = {4.3f, 6.4f, 3.3f};
	vec3_suba(a, b);
	CHECK_VEC3(a, -3.2f, 2.0f, -1.0f);
}
END_TEST

START_TEST(test_vec3_mula)
{
	vec3_t a = {1.1f, 5.3f, 7.2f};
	vec3_t b = {1.0f, -1.0f, 2.0f};
	vec3_mula(a, b);
	CHECK_VEC3(a, 1.1f, -5.3f, 14.4f);
}
END_TEST

START_TEST(test_vec3_diva)
{
	vec3_t a = {5.4f, 7.0f, 1.1f};
	vec3_t b = {2.0f, -2.0f, 2.0f};
	vec3_diva(a, b);
	CHECK_VEC3(a, 2.7f, -3.5f, 0.55f);
}
END_TEST

START_TEST(test_vec3_add)
{
	vec3_t a = {-2.3f, 4.7f, 1.5f};
	vec3_t b = {1.0f, -1.0f, -7.3f};
	vec3_t c;
	vec3_add(c, a, b);
	CHECK_VEC3(c, -1.3f, 3.7f, -5.8f);
}
END_TEST

START_TEST(test_vec3_sub)
{
	vec3_t a = {4.7f, 2.3f, 1.9f};
	vec3_t b = {7.5f, 2.3f, 1.1f};
	vec3_t c;
	vec3_sub(c, a, b);
	CHECK_VEC3(c, -2.8f, 0.0f, 0.8f);
}
END_TEST

START_TEST(test_vec3_mul)
{
	vec3_t a = {1.3f, -1.5f, -7.0f};
	vec3_t b = {5.4f, 3.0f, 0.0f};
	vec3_t c;
	vec3_mul(c, a, b);
	CHECK_VEC3(c, 7.02f, -4.5f, 0.0f);
}
END_TEST

START_TEST(test_vec3_div)
{
	vec3_t a = {20.0f, -20.0f, 10.0f};
	vec3_t b = {1.0f, -1.0f, 5.0f};
	vec3_t c;
	vec3_div(c, a, b);
	CHECK_VEC3(c, 20.0f, 20.0f, 2.0f);
}
END_TEST

START_TEST(test_vec3_sadda)
{
	vec3_t a = {5.4f, 7.1f, 125.8f};
	vec3_sadda(a, 7.0f);
	CHECK_VEC3(a, 12.4f, 14.1f, 132.8f);
}
END_TEST

START_TEST(test_vec3_ssuba)
{
	vec3_t a = {5.4f, 7.1f, 125.8f};
	vec3_ssuba(a, -7.0f);
	CHECK_VEC3(a, 12.4f, 14.1f, 132.8f);
}
END_TEST

START_TEST(test_vec3_smula)
{
	vec3_t a = {12.4f, 5.0f, 56.2f};
	vec3_smula(a, -0.5f);
	CHECK_VEC3(a, -6.2f, -2.5f, -28.1f);
}
END_TEST

START_TEST(test_vec3_sdiva)
{
	vec3_t a = {100.0f, 101.0f, 110.1f};
	vec3_sdiva(a, 4.0f);
	CHECK_VEC3(a, 25.0f, 25.25f, 27.525f);
}
END_TEST

START_TEST(test_vec3_sadd)
{
	vec3_t b, a = {0.14f, 0.17f, -0.57f};
	vec3_sadd(b, a, 1.0f);
	CHECK_VEC3(b, 1.14f, 1.17f, 0.43f);
}
END_TEST

START_TEST(test_vec3_ssub)
{
	vec3_t b, a = {14.4f, 15.5f, 16.6f};
	vec3_ssub(b, a, 4.0f);
	CHECK_VEC3(b, 10.4f, 11.5f, 12.6f);
}
END_TEST

START_TEST(test_vec3_smul)
{
	vec3_t b, a = {5.4f, 17.6f, 30.2f};
	vec3_smul(b, a, 0.5f);
	CHECK_VEC3(b, 2.7f, 8.8f, 15.1f);
}
END_TEST

START_TEST(test_vec3_sdiv)
{
	vec3_t b, a = {5.0f, -5.0f, 7.0f};
	vec3_sdiv(b, a, 0.5f);
	CHECK_VEC3(b, 10.0f, -10.0f, 14.0f);
}
END_TEST

START_TEST(test_vec3_set)
{
	vec3_t a;
	vec3_set(a, 999.999f, 666.666f, 333.333f);
	CHECK_VEC3(a, 999.999f, 666.666f, 333.333f);
}
END_TEST

START_TEST(test_vec3_neg)
{
	vec3_t b, a = {5.5f, 7.7f, 8.8f}; // where is 6?
	vec3_neg(b, a);
	CHECK_VEC3(b, -5.5f, -7.7f, -8.8f);
}
END_TEST

START_TEST(test_vec3_copy)
{
	vec3_t b, a = {7.4f, 3.0f, 3.1f};
	vec3_copy(b, a);
	CHECK_VEC3(b, 7.4f, 3.0f, 3.1f);
}
END_TEST

START_TEST(test_vec3_length2)
{
	vec3_t a = {1.0f, 1.0f, 1.0f};
	float len2 = vec3_length2(a);
	fail_unless(FLOATS_ARE_EQUAL(len2, 3.0f),
		    "%f length2 expected, got: %f", 3.0f, len2);
}
END_TEST

START_TEST(test_vec3_length)
{
	vec3_t a = {5.0f, 5.0f, 5.0f};
	float len = vec3_length(a);
	fail_unless(FLOATS_ARE_EQUAL(len, 8.66025403f),
		    "%f length expected, got: %f", 8.66025403f, len);
}
END_TEST

START_TEST(test_vec3_normalize)
{
	vec3_t a = {5.0f, 5.0f, 5.0f};
	vec3_normalize(a);
	CHECK_VEC3(a, 0.57735024f, 0.57735024f, 0.57735024f);
}
END_TEST

START_TEST(test_vec3_inv)
{
	vec3_t b, a = {4.0f, -8.0f, 5.0f};
	vec3_inv(b, a);
	CHECK_VEC3(b, 0.25f, -0.125f, 0.2f);
}
END_TEST

START_TEST(test_vec3_distance)
{
	vec3_t a = {5.0f, 0.0f, 5.0f};
	vec3_t b = {0.0f, 5.0f, 0.0f};
	float d = vec3_distance(a, b);
	fail_unless(FLOATS_ARE_EQUAL(d, 8.66025403f),
		    "%f distance expected, got: %f", 8.66025403f, d);
}
END_TEST

START_TEST(test_vec3_distance2)
{
	vec3_t a = {4.5f, 7.8f, -3.3f};
	vec3_t b = {1.2f, -7.5f, 4.8f};
	float d2 = vec3_distance2(a, b);
	fail_unless(FLOATS_ARE_EQUAL(d2, 310.590027f),
		    "%f distance2 expected, got: %f", 310.590027f, d2);
}
END_TEST

START_TEST(test_vec3_dot)
{
	vec3_t a = {1.3f, -1.5f, -7.0f};
	vec3_t b = {5.4f, 3.0f, 0.0f};
	float dot = vec3_dot(a, b);
	fail_unless(FLOATS_ARE_EQUAL(dot, 2.52f),
		    "f%f dot product expected, got: %f", 2.52f, dot);
}
END_TEST

START_TEST(test_vec3_cross)
{
	vec3_t a = {5.0f, 0.0f, 0.0f};
	vec3_t b = {0.0f, 5.0f, 5.0f};
	vec3_t c;
	vec3_cross(c, a, b);
	CHECK_VEC3(c, 0.0f, -25.0f, 25.0f);
}
END_TEST

START_TEST(test_vec3_advance)
{
	vec3_t b, a = {4.5f, 0.0f, 7.1f};
	vec3_advance(b, a, VEC3_UNIT_Y, 3.0f);
	CHECK_VEC3(b, 4.5f, 3.0f, 7.1f);

	vec3_advance(b, a, VEC3_UNIT_X, -2.0f);
	CHECK_VEC3(b, 2.5f, 0.0f, 7.1f);

	vec3_advance(a, b, (vec3_t){0.0f, 1.0f, 1.0f}, 2.0f);
	CHECK_VEC3(a, 2.5f, 2.0f, 9.1f);
}
END_TEST

START_TEST(test_vec3_equals)
{
	vec3_t b, a = {1.4f, -3.1415f, 5.16f};
	vec3_copy(b, a);
	fail_unless(vec3_equals(a, b),
		    "vectors equality expected, got: (%f,%f,%f) != (%f,%f,%f)",
		    VEC_X(a), VEC_Y(a), VEC_Z(a),
		    VEC_X(b), VEC_Y(b), VEC_Z(b));

	vec3_set(b, 1.0f, 1.0f, 5.16f);
	fail_unless(!vec3_equals(a, b),
		    "vectors unequality expected, got: (%f,%f,%f) != (%f,%f,%f)",
		    VEC_X(a), VEC_Y(a), VEC_Z(a),
		    VEC_X(b), VEC_Y(b), VEC_Z(b));
}
END_TEST

START_TEST(test_vec3_rotate)
{
	vec3_t a;
	vec3_copy(a, VEC3_UNIT_Y);
	vec3_rotate_x(a, 45.0f, VEC3_ZERO);
	CHECK_VEC3(a, 0.0f, 0.7071067f, 0.7071067f);

	vec3_rotate_z(a, -45.0f, VEC3_ZERO);
	CHECK_VEC3(a, 0.5f, 0.5f, 0.7071067f);

	vec3_copy(a, VEC3_UNIT_Z);
	vec3_rotate_x(a, 45.0f, VEC3_ZERO);
	CHECK_VEC3(a, 0.0f, -0.7071067f, 0.7071067f);

	vec3_set(a, 0.0f, 2.0f, 0.0f);
	vec3_rotate_x(a, 45.0f, (vec3_t){0.0f, 1.0f, 0.0f});
	CHECK_VEC3(a, 0.0f, 1.7071067f, 0.7071067f);

	vec3_rotate_y(a, 45.0f, (vec3_t){0.0f, 1.0f, 0.0f});
	CHECK_VEC3(a, 0.5f, 1.7071067f, 0.5f);
}
END_TEST

Suite *linear_math_suite()
{
	Suite *s = suite_create("linear_math");

	TCase *tc_vec2 = tcase_create("vec2");
	// basic math, assignment form (A op= B)
	tcase_add_test(tc_vec2, test_vec2_adda);
	tcase_add_test(tc_vec2, test_vec2_suba);
	tcase_add_test(tc_vec2, test_vec2_mula);
	tcase_add_test(tc_vec2, test_vec2_diva);
	// basic math (A = B op C)
	tcase_add_test(tc_vec2, test_vec2_add);
	tcase_add_test(tc_vec2, test_vec2_sub);
	tcase_add_test(tc_vec2, test_vec2_mul);
	tcase_add_test(tc_vec2, test_vec2_div);
	// basic scalar math, assignment form (A op= B)
	tcase_add_test(tc_vec2, test_vec2_sadda);
	tcase_add_test(tc_vec2, test_vec2_ssuba);
	tcase_add_test(tc_vec2, test_vec2_smula);
	tcase_add_test(tc_vec2, test_vec2_sdiva);
	// basic scalar math (A = B op C)
	tcase_add_test(tc_vec2, test_vec2_sadd);
	tcase_add_test(tc_vec2, test_vec2_ssub);
	tcase_add_test(tc_vec2, test_vec2_smul);
	tcase_add_test(tc_vec2, test_vec2_sdiv);

	tcase_add_test(tc_vec2, test_vec2_set);
	tcase_add_test(tc_vec2, test_vec2_neg);
	tcase_add_test(tc_vec2, test_vec2_copy);
	tcase_add_test(tc_vec2, test_vec2_length2);
	tcase_add_test(tc_vec2, test_vec2_length);
	tcase_add_test(tc_vec2, test_vec2_normalize);
	tcase_add_test(tc_vec2, test_vec2_distance);
	tcase_add_test(tc_vec2, test_vec2_distance2);
	tcase_add_test(tc_vec2, test_vec2_equals);
	tcase_add_test(tc_vec2, test_vec2_dot);
	tcase_add_test(tc_vec2, test_vec2_rotate);

	TCase *tc_vec3 = tcase_create("vec3");
	// basic math, assignment form (A op= B)
	tcase_add_test(tc_vec3, test_vec3_adda);
	tcase_add_test(tc_vec3, test_vec3_suba);
	tcase_add_test(tc_vec3, test_vec3_mula);
	tcase_add_test(tc_vec3, test_vec3_diva);
	// basic math (A = B op C)
	tcase_add_test(tc_vec3, test_vec3_add);
	tcase_add_test(tc_vec3, test_vec3_sub);
	tcase_add_test(tc_vec3, test_vec3_mul);
	tcase_add_test(tc_vec3, test_vec3_div);
	// basic scalar math, assignment form (A op= B)
	tcase_add_test(tc_vec3, test_vec3_sadda);
	tcase_add_test(tc_vec3, test_vec3_ssuba);
	tcase_add_test(tc_vec3, test_vec3_smula);
	tcase_add_test(tc_vec3, test_vec3_sdiva);
	// basic scalar math (A = B op C)
	tcase_add_test(tc_vec3, test_vec3_sadd);
	tcase_add_test(tc_vec3, test_vec3_ssub);
	tcase_add_test(tc_vec3, test_vec3_smul);
	tcase_add_test(tc_vec3, test_vec3_sdiv);

	tcase_add_test(tc_vec3, test_vec3_set);
	tcase_add_test(tc_vec3, test_vec3_neg);
	tcase_add_test(tc_vec3, test_vec3_copy);
	tcase_add_test(tc_vec3, test_vec3_length2);
	tcase_add_test(tc_vec3, test_vec3_length);
	tcase_add_test(tc_vec3, test_vec3_normalize);
	tcase_add_test(tc_vec3, test_vec3_inv);
	tcase_add_test(tc_vec3, test_vec3_distance);
	tcase_add_test(tc_vec3, test_vec3_distance2);
	tcase_add_test(tc_vec3, test_vec3_dot);
	tcase_add_test(tc_vec3, test_vec3_cross);
	tcase_add_test(tc_vec3, test_vec3_advance);
	tcase_add_test(tc_vec3, test_vec3_equals);
	tcase_add_test(tc_vec3, test_vec3_rotate);

	suite_add_tcase(s, tc_vec2);
	suite_add_tcase(s, tc_vec3);
	return s;
}
