#pragma once

/*
 * Linear math.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* Math constants. */
#define MATH_PI			(3.14159265359f)
#define MATH_DEG_TO_RAD		(MATH_PI / 180.0f)
#define MATH_RAD_TO_DEG		(180.0f / MATH_PI)
#define MATH_EPSILON		(1e-6f)

/*
 * Basic math types.
 * But don't forget that these are just arrays of numbers. And you can use
 * vec3_t functions on vec4_t for example.
 */
typedef int rect_t[4];		/* x, y, w, h */
typedef float vec2_t[2];	/* x. y */
typedef float vec3_t[3];	/* x, y, z */
typedef float vec4_t[4];	/* x, y, z, w */
typedef float quat_t[4];	/* x, y, z, w */
typedef float aabb_t[6];	/* vec3_t min, vec3_t max */
typedef float mat4_t[16];	/* matrix 4x4 */
typedef float plane_t[4];	/* nx, ny, nz, d */
typedef float frustum_t[24];	/* far, near, left, right, bottom, top planes */

/* Fast 16-bytes aligned types (SSE) */
typedef float vec4_sse_t[4] __attribute__((aligned(16)));
typedef float quat_sse_t[4] __attribute__((aligned(16)));
typedef float mat4_sse_t[16] __attribute__((aligned(16)));

enum plane_side {
	PLANE_FRONT,
	PLANE_BACK,
	PLANE_BOTH
};

enum frustum_side {
	FRUSTUM_INSIDE,
	FRUSTUM_OUTSIDE,
	FRUSTUM_BOTH
};

enum frustum_plane {
	FRUSTUM_PLANE_NEAR,
	FRUSTUM_PLANE_FAR,
	FRUSTUM_PLANE_LEFT,
	FRUSTUM_PLANE_RIGHT,
	FRUSTUM_PLANE_BOTTOM,
	FRUSTUM_PLANE_TOP
};

/*
 * If I'm writing down a column-major matrix as a C array, it looks transposed
 * on a paper (in a text file). This macro changes situation.
 *
 * E.g. always use this for constants:
 * mat4_t m = MAT4(...);
 *
 * instead of:
 * mat4_t m = {...};
 */
#define MAT4(m0,m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11,m12,m13,m14,m15)	\
	{m0, m4, m8,  m12,						\
	 m1, m5, m9,  m13,						\
	 m2, m6, m10, m14,						\
	 m3, m7, m11, m15}

/* Accessors. */
#define RECT_X(r) ((r)[0])
#define RECT_Y(r) ((r)[1])
#define RECT_W(r) ((r)[2])
#define RECT_H(r) ((r)[3])

#define VEC_X(v) ((v)[0])
#define VEC_Y(v) ((v)[1])
#define VEC_Z(v) ((v)[2])
#define VEC_W(v) ((v)[3])

#define QUAT_X(q) ((q)[0])
#define QUAT_Y(q) ((q)[1])
#define QUAT_Z(q) ((q)[2])
#define QUAT_W(q) ((q)[3])

// TODO: wrong?
#define MAT4_RC(m, row, column) ((m)[(row)*4+(column)])

#define MAT4_AXIS_X(m) (vec3_t){m[0], m[4], m[8]}
#define MAT4_AXIS_Y(m) (vec3_t){m[1], m[5], m[9]}
#define MAT4_AXIS_Z(m) (vec3_t){m[2], m[6], m[10]}

#define PLANE_NX(p) ((p)[0])
#define PLANE_NY(p) ((p)[1])
#define PLANE_NZ(p) ((p)[2])
#define PLANE_D(p) ((p)[3])

#define AABB_MIN(s) (s)
#define AABB_MAX(s) ((s)+3)
#define AABB_MIN_X(s) ((s)[0])
#define AABB_MIN_Y(s) ((s)[1])
#define AABB_MIN_Z(s) ((s)[2])
#define AABB_MAX_X(s) ((s)[3])
#define AABB_MAX_Y(s) ((s)[4])
#define AABB_MAX_Z(s) ((s)[5])

#define FRUSTUM_PLANE(f, p) ((f)+((p)*4))

/* Constants */
#define VEC2_ZERO (vec2_t){0.0f, 0.0f}
#define VEC2_UNIT_X (vec2_t){1.0f, 0.0f}
#define VEC2_UNIT_Y (vec2_t){0.0f, 1.0f}

#define VEC3_ZERO (vec3_t){0.0f, 0.0f, 0.0f}
#define VEC3_UNIT_X (vec3_t){1.0f, 0.0f, 0.0f}
#define VEC3_UNIT_Y (vec3_t){0.0f, 1.0f, 0.0f}
#define VEC3_UNIT_Z (vec3_t){0.0f, 0.0f, 1.0f}

/* Other useful macros */
#undef MAX
#undef MIN
#undef CLAMP
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MAX3(a,b,c) MAX((a), MAX((b), (c)))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MIN3(a,b,c) MIN((a), MIN((b), (c)))
#define CLAMP(v, l, h)			\
do {					\
	if ((v) > (h))			\
		(v) = (h);		\
	if ((v) < (l))			\
		(v) = (l);		\
} while (0)

/*
   /------------------------------------------------------------------\
  /                   Vector math operations table.                    \
  +-------+-----------------------------------+--------+---------------+
  | name  | meaning                           | args   | example       |
  +-------+-----------------------------------+--------+---------------+
  | adda  | addition assignment               | 2      | out += v      |
  | suba  | subtraction assignment            | 2      | out -= v      |
  | mula  | multiplication assignment         | 2      | out *= v      |
  | diva  | division assignment               | 2      | out /= v      |
  | add   | addition                          | 3      | out = v1 + v2 |
  | sub   | subtraction                       | 3      | out = v1 - v2 |
  | mul   | multiplication                    | 3      | out = v1 * v2 |
  | div   | division                          | 3      | out = v1 / v2 |
  | sadda | scalar addition assignment        | 2      | out += s      |
  | ssuba | scalar subtraction assignment     | 2      | out -= s      |
  | smula | scalar multiplication assignment  | 2      | out *= s      |
  | sdiva | scalar division assignment        | 2      | out /= s      |
  | sadd  | scalar addition                   | 3      | out = v1 + s  |
  | ssub  | scalar subtraction                | 3      | out = v1 - s  |
  | smul  | scalar multiplication             | 3      | out = v1 * s  |
  | sdiv  | scalar division                   | 3      | out = v1 / s  |
  +-------+-----------------------------------+--------+---------------+
*/

/**************************************************************************
  vec2: two-dimensional vector
**************************************************************************/

/************************************************************
 * Macro generated vector operation functions for vec2      *
 * (select in vim with visual mode and type !m4 to process) *
 ************************************************************
define(`ASSIGNMENT_VEC2_FUNCTION',
`static inline void vec2_$2(vec2_t out, const vec2_t v)
{
	VEC_X(out) $1 VEC_X(v);
	VEC_Y(out) $1 VEC_Y(v);
}
')dnl
define(`GENERAL_VEC2_FUNCTION',
`static inline void vec2_$2(vec2_t out, const vec2_t v1, const vec2_t v2)
{
	VEC_X(out) = VEC_X(v1) $1 VEC_X(v2);
	VEC_Y(out) = VEC_Y(v1) $1 VEC_Y(v2);
}
')dnl
ASSIGNMENT_VEC2_FUNCTION(+=, adda)
ASSIGNMENT_VEC2_FUNCTION(-=, suba)
ASSIGNMENT_VEC2_FUNCTION(*=, mula)
ASSIGNMENT_VEC2_FUNCTION(/=, diva)
GENERAL_VEC2_FUNCTION(+, add)
GENERAL_VEC2_FUNCTION(-, sub)
GENERAL_VEC2_FUNCTION(*, mul)
GENERAL_VEC2_FUNCTION(/, div)
*/
static inline void vec2_adda(vec2_t out, const vec2_t v)
{
	VEC_X(out) += VEC_X(v);
	VEC_Y(out) += VEC_Y(v);
}

static inline void vec2_suba(vec2_t out, const vec2_t v)
{
	VEC_X(out) -= VEC_X(v);
	VEC_Y(out) -= VEC_Y(v);
}

static inline void vec2_mula(vec2_t out, const vec2_t v)
{
	VEC_X(out) *= VEC_X(v);
	VEC_Y(out) *= VEC_Y(v);
}

static inline void vec2_diva(vec2_t out, const vec2_t v)
{
	VEC_X(out) /= VEC_X(v);
	VEC_Y(out) /= VEC_Y(v);
}

static inline void vec2_add(vec2_t out, const vec2_t v1, const vec2_t v2)
{
	VEC_X(out) = VEC_X(v1) + VEC_X(v2);
	VEC_Y(out) = VEC_Y(v1) + VEC_Y(v2);
}

static inline void vec2_sub(vec2_t out, const vec2_t v1, const vec2_t v2)
{
	VEC_X(out) = VEC_X(v1) - VEC_X(v2);
	VEC_Y(out) = VEC_Y(v1) - VEC_Y(v2);
}

static inline void vec2_mul(vec2_t out, const vec2_t v1, const vec2_t v2)
{
	VEC_X(out) = VEC_X(v1) * VEC_X(v2);
	VEC_Y(out) = VEC_Y(v1) * VEC_Y(v2);
}

static inline void vec2_div(vec2_t out, const vec2_t v1, const vec2_t v2)
{
	VEC_X(out) = VEC_X(v1) / VEC_X(v2);
	VEC_Y(out) = VEC_Y(v1) / VEC_Y(v2);
}
/* END
 ************************************************************************/

/************************************************************
 * Macro generated scalar operation functions for vec2      *
 * (select in vim with visual mode and type !m4 to process) *
 ************************************************************
define(`SCALAR_ASSIGNMENT_VEC2_FUNCTION',
`static inline void vec2_$2(vec2_t out, float s)
{
	VEC_X(out) $1 s;
	VEC_Y(out) $1 s;
}
')dnl
define(`SCALAR_GENERAL_VEC2_FUNCTION',
`static inline void vec2_$2(vec2_t out, const vec2_t v1, float s)
{
	VEC_X(out) = VEC_X(v1) $1 s;
	VEC_Y(out) = VEC_Y(v1) $1 s;
}
')dnl
SCALAR_ASSIGNMENT_VEC2_FUNCTION(+=, sadda)
SCALAR_ASSIGNMENT_VEC2_FUNCTION(-=, ssuba)
SCALAR_ASSIGNMENT_VEC2_FUNCTION(*=, smula)
SCALAR_ASSIGNMENT_VEC2_FUNCTION(/=, sdiva)
SCALAR_GENERAL_VEC2_FUNCTION(+, sadd)
SCALAR_GENERAL_VEC2_FUNCTION(-, ssub)
SCALAR_GENERAL_VEC2_FUNCTION(*, smul)
SCALAR_GENERAL_VEC2_FUNCTION(/, sdiv)
*/
static inline void vec2_sadda(vec2_t out, float s)
{
	VEC_X(out) += s;
	VEC_Y(out) += s;
}

static inline void vec2_ssuba(vec2_t out, float s)
{
	VEC_X(out) -= s;
	VEC_Y(out) -= s;
}

static inline void vec2_smula(vec2_t out, float s)
{
	VEC_X(out) *= s;
	VEC_Y(out) *= s;
}

static inline void vec2_sdiva(vec2_t out, float s)
{
	VEC_X(out) /= s;
	VEC_Y(out) /= s;
}

static inline void vec2_sadd(vec2_t out, const vec2_t v1, float s)
{
	VEC_X(out) = VEC_X(v1) + s;
	VEC_Y(out) = VEC_Y(v1) + s;
}

static inline void vec2_ssub(vec2_t out, const vec2_t v1, float s)
{
	VEC_X(out) = VEC_X(v1) - s;
	VEC_Y(out) = VEC_Y(v1) - s;
}

static inline void vec2_smul(vec2_t out, const vec2_t v1, float s)
{
	VEC_X(out) = VEC_X(v1) * s;
	VEC_Y(out) = VEC_Y(v1) * s;
}

static inline void vec2_sdiv(vec2_t out, const vec2_t v1, float s)
{
	VEC_X(out) = VEC_X(v1) / s;
	VEC_Y(out) = VEC_Y(v1) / s;
}

/* END
 ************************************************************************/

/* utility functions for vec2 */
static inline void vec2_set(vec2_t out, float x, float y)
{
	VEC_X(out) = x;
	VEC_Y(out) = y;
}

static inline void vec2_neg(vec2_t out, const vec2_t v)
{
	VEC_X(out) = -VEC_X(v);
	VEC_Y(out) = -VEC_Y(v);
}

static inline void vec2_copy(vec2_t out, const vec2_t v)
{
	memcpy(out, v, sizeof(float) * 2);
}

static inline float vec2_length2(const vec2_t v)
{
	return (VEC_X(v)*VEC_X(v) + VEC_Y(v)*VEC_Y(v));
}

static inline float vec2_length(const vec2_t v)
{
	return sqrtf(vec2_length2(v));
}

static inline void vec2_normalize(vec2_t out)
{
	float ilength = 1.0f / vec2_length(out);
	vec2_smula(out, ilength);
}

static inline float vec2_distance(const vec2_t v1, const vec2_t v2)
{
	vec2_t v;
	vec2_sub(v, v1, v2);
	return vec2_length(v);
}

static inline float vec2_distance2(const vec2_t v1, const vec2_t v2)
{
	vec2_t v;
	vec2_sub(v, v1, v2);
	return vec2_length2(v);
}

static inline bool vec2_equals(const vec2_t v1, const vec2_t v2)
{
	return (VEC_X(v1) == VEC_X(v2) && VEC_Y(v1) == VEC_Y(v2));
}

static inline bool vec2_nearly_equals(const vec2_t v1, const vec2_t v2)
{
	return (fabs(VEC_X(v1) - VEC_X(v2)) < MATH_EPSILON &&
		fabs(VEC_Y(v1) - VEC_Y(v2)) < MATH_EPSILON);
}

static inline float vec2_dot(const vec2_t v1, const vec2_t v2)
{
	return (VEC_X(v1)*VEC_X(v2) + VEC_Y(v1)*VEC_Y(v2));
}

// for left-handed coordinate system, rotation is CW when imaginary Z goes
// towards you
void vec2_rotate(vec2_t out, float deg, const vec2_t origin);

/**************************************************************************
  vec3: three-dimensional vector
**************************************************************************/

/************************************************************
 * Macro generated vector operation functions for vec3      *
 * (select in vim with visual mode and type !m4 to process) *
 ************************************************************
define(`ASSIGNMENT_VEC3_FUNCTION',
`static inline void vec3_$2(vec3_t out, const vec3_t v)
{
	VEC_X(out) $1 VEC_X(v);
	VEC_Y(out) $1 VEC_Y(v);
	VEC_Z(out) $1 VEC_Z(v);
}
')dnl
define(`GENERAL_VEC3_FUNCTION',
`static inline void vec3_$2(vec3_t out, const vec3_t v1, const vec3_t v2)
{
	VEC_X(out) = VEC_X(v1) $1 VEC_X(v2);
	VEC_Y(out) = VEC_Y(v1) $1 VEC_Y(v2);
	VEC_Z(out) = VEC_Z(v1) $1 VEC_Z(v2);
}
')dnl
ASSIGNMENT_VEC3_FUNCTION(+=, adda)
ASSIGNMENT_VEC3_FUNCTION(-=, suba)
ASSIGNMENT_VEC3_FUNCTION(*=, mula)
ASSIGNMENT_VEC3_FUNCTION(/=, diva)
GENERAL_VEC3_FUNCTION(+, add)
GENERAL_VEC3_FUNCTION(-, sub)
GENERAL_VEC3_FUNCTION(*, mul)
GENERAL_VEC3_FUNCTION(/, div)
*/
static inline void vec3_adda(vec3_t out, const vec3_t v)
{
	VEC_X(out) += VEC_X(v);
	VEC_Y(out) += VEC_Y(v);
	VEC_Z(out) += VEC_Z(v);
}

static inline void vec3_suba(vec3_t out, const vec3_t v)
{
	VEC_X(out) -= VEC_X(v);
	VEC_Y(out) -= VEC_Y(v);
	VEC_Z(out) -= VEC_Z(v);
}

static inline void vec3_mula(vec3_t out, const vec3_t v)
{
	VEC_X(out) *= VEC_X(v);
	VEC_Y(out) *= VEC_Y(v);
	VEC_Z(out) *= VEC_Z(v);
}

static inline void vec3_diva(vec3_t out, const vec3_t v)
{
	VEC_X(out) /= VEC_X(v);
	VEC_Y(out) /= VEC_Y(v);
	VEC_Z(out) /= VEC_Z(v);
}

static inline void vec3_add(vec3_t out, const vec3_t v1, const vec3_t v2)
{
	VEC_X(out) = VEC_X(v1) + VEC_X(v2);
	VEC_Y(out) = VEC_Y(v1) + VEC_Y(v2);
	VEC_Z(out) = VEC_Z(v1) + VEC_Z(v2);
}

static inline void vec3_sub(vec3_t out, const vec3_t v1, const vec3_t v2)
{
	VEC_X(out) = VEC_X(v1) - VEC_X(v2);
	VEC_Y(out) = VEC_Y(v1) - VEC_Y(v2);
	VEC_Z(out) = VEC_Z(v1) - VEC_Z(v2);
}

static inline void vec3_mul(vec3_t out, const vec3_t v1, const vec3_t v2)
{
	VEC_X(out) = VEC_X(v1) * VEC_X(v2);
	VEC_Y(out) = VEC_Y(v1) * VEC_Y(v2);
	VEC_Z(out) = VEC_Z(v1) * VEC_Z(v2);
}

static inline void vec3_div(vec3_t out, const vec3_t v1, const vec3_t v2)
{
	VEC_X(out) = VEC_X(v1) / VEC_X(v2);
	VEC_Y(out) = VEC_Y(v1) / VEC_Y(v2);
	VEC_Z(out) = VEC_Z(v1) / VEC_Z(v2);
}

/* END
 ************************************************************************/

/************************************************************
 * Macro generated scalar operation functions for vec3      *
 * (select in vim with visual mode and type !m4 to process) *
 ************************************************************
define(`SCALAR_ASSIGNMENT_VEC3_FUNCTION',
`static inline void vec3_$2(vec3_t out, float s)
{
	VEC_X(out) $1 s;
	VEC_Y(out) $1 s;
	VEC_Z(out) $1 s;
}
')dnl
define(`SCALAR_GENERAL_VEC3_FUNCTION',
`static inline void vec3_$2(vec3_t out, const vec3_t v1, float s)
{
	VEC_X(out) = VEC_X(v1) $1 s;
	VEC_Y(out) = VEC_Y(v1) $1 s;
	VEC_Z(out) = VEC_Z(v1) $1 s;
}
')dnl
SCALAR_ASSIGNMENT_VEC3_FUNCTION(+=, sadda)
SCALAR_ASSIGNMENT_VEC3_FUNCTION(-=, ssuba)
SCALAR_ASSIGNMENT_VEC3_FUNCTION(*=, smula)
SCALAR_ASSIGNMENT_VEC3_FUNCTION(/=, sdiva)
SCALAR_GENERAL_VEC3_FUNCTION(+, sadd)
SCALAR_GENERAL_VEC3_FUNCTION(-, ssub)
SCALAR_GENERAL_VEC3_FUNCTION(*, smul)
SCALAR_GENERAL_VEC3_FUNCTION(/, sdiv)
*/
static inline void vec3_sadda(vec3_t out, float s)
{
	VEC_X(out) += s;
	VEC_Y(out) += s;
	VEC_Z(out) += s;
}

static inline void vec3_ssuba(vec3_t out, float s)
{
	VEC_X(out) -= s;
	VEC_Y(out) -= s;
	VEC_Z(out) -= s;
}

static inline void vec3_smula(vec3_t out, float s)
{
	VEC_X(out) *= s;
	VEC_Y(out) *= s;
	VEC_Z(out) *= s;
}

static inline void vec3_sdiva(vec3_t out, float s)
{
	VEC_X(out) /= s;
	VEC_Y(out) /= s;
	VEC_Z(out) /= s;
}

static inline void vec3_sadd(vec3_t out, const vec3_t v1, float s)
{
	VEC_X(out) = VEC_X(v1) + s;
	VEC_Y(out) = VEC_Y(v1) + s;
	VEC_Z(out) = VEC_Z(v1) + s;
}

static inline void vec3_ssub(vec3_t out, const vec3_t v1, float s)
{
	VEC_X(out) = VEC_X(v1) - s;
	VEC_Y(out) = VEC_Y(v1) - s;
	VEC_Z(out) = VEC_Z(v1) - s;
}

static inline void vec3_smul(vec3_t out, const vec3_t v1, float s)
{
	VEC_X(out) = VEC_X(v1) * s;
	VEC_Y(out) = VEC_Y(v1) * s;
	VEC_Z(out) = VEC_Z(v1) * s;
}

static inline void vec3_sdiv(vec3_t out, const vec3_t v1, float s)
{
	VEC_X(out) = VEC_X(v1) / s;
	VEC_Y(out) = VEC_Y(v1) / s;
	VEC_Z(out) = VEC_Z(v1) / s;
}

/* END
 ************************************************************************/

/* utility functions for vec3 */
static inline void vec3_set(vec3_t out, float x, float y, float z)
{
	VEC_X(out) = x;
	VEC_Y(out) = y;
	VEC_Z(out) = z;
}

static inline void vec3_neg(vec3_t out, const vec3_t v)
{
	VEC_X(out) = -VEC_X(v);
	VEC_Y(out) = -VEC_Y(v);
	VEC_Z(out) = -VEC_Z(v);
}

static inline void vec3_copy(vec3_t out, const vec3_t v)
{
	memcpy(out, v, sizeof(float) * 3);
}

static inline float vec3_length2(const vec3_t v)
{
	return (VEC_X(v)*VEC_X(v) + VEC_Y(v)*VEC_Y(v) + VEC_Z(v)*VEC_Z(v));
}

static inline float vec3_length(const vec3_t v)
{
	return sqrtf(vec3_length2(v));
}

static inline void vec3_normalize(vec3_t out)
{
	float ilength = 1.0f / vec3_length(out);
	vec3_smula(out, ilength);
}

static inline void vec3_inv(vec3_t out, const vec3_t v)
{
	VEC_X(out) = 1.0f / VEC_X(v);
	VEC_Y(out) = 1.0f / VEC_Y(v);
	VEC_Z(out) = 1.0f / VEC_Z(v);
}

static inline float vec3_distance(const vec3_t v1, const vec3_t v2)
{
	vec3_t v;
	vec3_sub(v, v1, v2);
	return vec3_length(v);
}

static inline float vec3_distance2(const vec3_t v1, const vec3_t v2)
{
	vec3_t v;
	vec3_sub(v, v1, v2);
	return vec3_length2(v);
}

static inline float vec3_dot(const vec3_t v1, const vec3_t v2)
{
	return (VEC_X(v1)*VEC_X(v2) + VEC_Y(v1)*VEC_Y(v2) + VEC_Z(v1)*VEC_Z(v2));
}

static inline void vec3_cross(vec3_t out, const vec3_t v1, const vec3_t v2)
{
	VEC_X(out) = VEC_Y(v1) * VEC_Z(v2) - VEC_Z(v1) * VEC_Y(v2);
	VEC_Y(out) = VEC_Z(v1) * VEC_X(v2) - VEC_X(v1) * VEC_Z(v2);
	VEC_Z(out) = VEC_X(v1) * VEC_Y(v2) - VEC_Y(v1) * VEC_X(v2);
}

static inline void vec3_advance(vec3_t out, const vec3_t origin, const vec3_t dir,
				float distance)
{
	VEC_X(out) = VEC_X(origin) + VEC_X(dir) * distance;
	VEC_Y(out) = VEC_Y(origin) + VEC_Y(dir) * distance;
	VEC_Z(out) = VEC_Z(origin) + VEC_Z(dir) * distance;
}

static inline void vec3_advancea(vec3_t out, const vec3_t dir, float distance)
{
	VEC_X(out) += VEC_X(dir) * distance;
	VEC_Y(out) += VEC_Y(dir) * distance;
	VEC_Z(out) += VEC_Z(dir) * distance;
}

static inline bool vec3_equals(const vec3_t v1, const vec3_t v2)
{
	return (VEC_X(v1) == VEC_X(v2) && VEC_Y(v1) == VEC_Y(v2) && VEC_Z(v1) == VEC_Z(v2));
}

static inline bool vec3_nearly_equals(const vec3_t v1, const vec3_t v2)
{
	return (fabs(VEC_X(v1) - VEC_X(v2)) < MATH_EPSILON &&
		fabs(VEC_Y(v1) - VEC_Y(v2)) < MATH_EPSILON &&
		fabs(VEC_Z(v1) - VEC_Z(v2)) < MATH_EPSILON);
}

// for left-handed coordinate system, rotation is CW, when specified axis goes
// towards you
void vec3_rotate_x(vec3_t out, float deg, const vec3_t origin);
void vec3_rotate_y(vec3_t out, float deg, const vec3_t origin);
void vec3_rotate_z(vec3_t out, float deg, const vec3_t origin);

/**************************************************************************
  vec4: four-dimensional vector
**************************************************************************/

static inline void vec4_set(vec4_t out, float x, float y, float z, float w)
{
	VEC_X(out) = x;
	VEC_Y(out) = y;
	VEC_Z(out) = z;
	VEC_W(out) = w;
}

static inline void vec4_neg(vec4_t out, const vec4_t v)
{
	VEC_X(out) = -VEC_X(v);
	VEC_Y(out) = -VEC_Y(v);
	VEC_Z(out) = -VEC_Z(v);
	VEC_W(out) = -VEC_W(v);
}

static inline void vec4_copy(vec4_t out, const vec4_t v)
{
	memcpy(out, v, sizeof(float) * 4);
}

static inline bool vec4_equals(const vec4_t v1, const vec4_t v2)
{
	return (VEC_X(v1) == VEC_X(v2) &&
		VEC_Y(v1) == VEC_Y(v2) &&
		VEC_Z(v1) == VEC_Z(v2) &&
		VEC_W(v1) == VEC_W(v2));
}

static inline bool vec4_nearly_equals(const vec4_t v1, const vec4_t v2)
{
	return (fabs(VEC_X(v1) - VEC_X(v2)) < MATH_EPSILON &&
		fabs(VEC_Y(v1) - VEC_Y(v2)) < MATH_EPSILON &&
		fabs(VEC_Z(v1) - VEC_Z(v2)) < MATH_EPSILON &&
		fabs(VEC_W(v1) - VEC_W(v2)) < MATH_EPSILON);
}

/**************************************************************************
  mat4: 4x4 OpenGL (column-major) matrix
**************************************************************************/

bool mat4_equals(const mat4_t m1, const mat4_t m2);
bool mat4_nearly_equals(const mat4_t m1, const mat4_t m2);
void mat4_copy(mat4_t out, const mat4_t m);
void mat4_transform_vec3(vec3_t v, const mat4_t m);
void mat4_mul(mat4_t out, const mat4_t m1, const mat4_t m2);
bool mat4_is_identity(const mat4_t m);
float mat4_determinant(const mat4_t m);
void mat4_inverse(mat4_t out, const mat4_t m);
void mat4_set_identity(mat4_t out);
void mat4_set_zero(mat4_t out);
void mat4_set_rotate(mat4_t out, const vec3_t axis, float angle);
void mat4_set_rotate_x(mat4_t out, float angle);
void mat4_set_rotate_y(mat4_t out, float angle);
void mat4_set_rotate_z(mat4_t out, float angle);
void mat4_set_scale(mat4_t out, const vec3_t scale);
void mat4_set_translate(mat4_t out, const vec3_t translate);
void mat4_set_perspective(mat4_t out, float fov, float aspect, float znear, float zfar);
void mat4_set_ortho(mat4_t out, float left, float right, float bottom, float top, float znear, float zfar);
void mat4_set_look_at(mat4_t out, const vec3_t eye, const vec3_t center, const vec3_t up);

/**************************************************************************
  quat: quaterion representing rotation
**************************************************************************/

static inline void quat_conjugate(quat_t out, const quat_t q)
{
	out[0] = -q[0];
	out[1] = -q[1];
	out[2] = -q[2];
	out[3] = q[3];
}

static inline void quat_copy(quat_t out, const quat_t q)
{
	memcpy(out, q, sizeof(float) * 4);
}

void quat_from_mat4(quat_t out, const mat4_t m);
void quat_from_angle(quat_t out, const vec3_t dir, float angle);
void quat_to_mat4(mat4_t out, const quat_t q);
void quat_transform_vec3(vec3_t out, const quat_t q);
void quat_slerp(quat_t out, const quat_t q1, const quat_t q2, float t);
void quat_mul(quat_t out, const quat_t q1, const quat_t q2);

/**************************************************************************
  rect: rectangle with integers
**************************************************************************/

static inline void rect_copy(rect_t out, const rect_t r)
{
	memcpy(out, r, sizeof(int) * 4);
}

static inline bool rect_equals(const rect_t r1, const rect_t r2)
{
	return (r1[0] == r2[0] &&
		r1[1] == r2[1] &&
		r1[2] == r2[2] &&
		r1[3] == r2[3]);
}

/**************************************************************************
  aabb: axis-aligned bounding box
**************************************************************************/

static inline void aabb_copy(aabb_t out, const aabb_t aabb)
{
	memcpy(out, aabb, sizeof(float) * 6);
}

static inline void aabb_set(aabb_t out, const vec3_t min, const vec3_t max)
{
	AABB_MIN_X(out) = VEC_X(min);
	AABB_MIN_Y(out) = VEC_Y(min);
	AABB_MIN_Z(out) = VEC_Z(min);
	AABB_MAX_X(out) = VEC_X(max);
	AABB_MAX_Y(out) = VEC_Y(max);
	AABB_MAX_Z(out) = VEC_Z(max);
}

static inline void aabb_expand(aabb_t out, float f)
{
	vec3_ssuba(AABB_MIN(out), f);
	vec3_sadda(AABB_MAX(out), f);
}

static inline void aabb_reset(aabb_t out, const vec3_t point)
{
	AABB_MIN_X(out) = VEC_X(point);
	AABB_MIN_Y(out) = VEC_Y(point);
	AABB_MIN_Z(out) = VEC_Z(point);
	AABB_MAX_X(out) = VEC_X(point);
	AABB_MAX_Y(out) = VEC_Y(point);
	AABB_MAX_Z(out) = VEC_Z(point);
}

static inline void aabb_center(vec3_t out, const aabb_t aabb)
{
	vec3_t tmp;
	vec3_add(tmp, AABB_MIN(aabb), AABB_MAX(aabb));
	vec3_sdiv(out, tmp, 2.0f);
}

void aabb_add_point(aabb_t aabb, const vec3_t point);
void aabb_add_aabb(aabb_t aabb1, const aabb_t aabb2);

/**************************************************************************
  plane: three-dimensional plane
**************************************************************************/

static inline void plane_set(plane_t out, const vec3_t origin, const vec3_t normal)
{
	vec3_copy(out, normal); /* it's assumed that normal is normalized */
	PLANE_D(out) = -vec3_dot(out, origin);
}

static inline void plane_from_points(plane_t out, const vec3_t v1, const vec3_t v2, const vec3_t v3)
{
	vec3_t v21, v31;
	vec3_sub(v21, v2, v1);
	vec3_sub(v31, v3, v1);
	vec3_cross(out, v21, v31);
	vec3_normalize(out);
	PLANE_D(out) = -vec3_dot(out, v1);
}

enum plane_side plane_side_point(const plane_t plane, const vec3_t point);
enum plane_side plane_side_aabb(const plane_t plane, const aabb_t aabb);

/**************************************************************************
  frustum
**************************************************************************/

void frustum_set(frustum_t out, const mat4_t matrix);
enum frustum_side frustum_side_aabb(const frustum_t frustum, const aabb_t aabb);

/**************************************************************************
  intersections
**************************************************************************/

bool aabb_clip_ray(const aabb_t aabb, const vec3_t orig, const vec3_t dir,
		   float *t_min, float *t_max);

static inline float plane_ray_intersect(const plane_t plane, const vec3_t origin,
					const vec3_t dir)
{
	return -(vec3_dot(origin, plane) + PLANE_D(plane)) / vec3_dot(dir, plane);
}

static inline bool ray_triangle_intersection(const vec3_t orig, const vec3_t dir,
					     const vec3_t a,
					     const vec3_t b,
					     const vec3_t c,
					     float *t_out,
					     float *u_out, float *v_out)
{
	vec3_t edge1;
	vec3_t edge2;
	vec3_t tvec, pvec, qvec;
	float det, invdet;
	float u, v, t;

	vec3_sub(edge1, c, a);
	vec3_sub(edge2, b, a);
	vec3_cross(pvec, dir, edge2);
	det = vec3_dot(edge1, pvec);

	if (det > -MATH_EPSILON && det < MATH_EPSILON)
		return false;

	invdet = 1.0f / det;
	vec3_sub(tvec, orig, a);

	u = vec3_dot(tvec, pvec) * invdet;
	if (u < 0.0f || u > 1.0f)
		return false;

	vec3_cross(qvec, tvec, edge1);
	v = vec3_dot(dir, qvec) * invdet;
	if (v < 0.0f || u + v > 1.0f)
		return false;

	t = vec3_dot(edge2, qvec) * invdet;
	if (t < 0.0f)
		return false;

	*t_out = t;
	*u_out = u;
	*v_out = v;
	return true;
}

static inline bool aabb_aabb_intersection(const aabb_t aabb1,
					  const aabb_t aabb2)
{
	return (AABB_MIN_X(aabb1) < AABB_MAX_X(aabb2) &&
		AABB_MIN_Y(aabb1) < AABB_MAX_Y(aabb2) &&
		AABB_MIN_Z(aabb1) < AABB_MAX_Z(aabb2) &&
		AABB_MAX_X(aabb1) > AABB_MIN_X(aabb2) &&
		AABB_MAX_Y(aabb1) > AABB_MIN_Y(aabb2) &&
		AABB_MAX_Z(aabb1) > AABB_MIN_Z(aabb2));
}

static inline bool point_in_2d_triangle(const vec2_t p,
					const vec2_t a,
					const vec2_t b,
					const vec2_t c,
					float *uout,
					float *vout)
{
	vec2_t v0, v1, v2;
	vec2_sub(v0, c, a);
	vec2_sub(v1, b, a);
	vec2_sub(v2, p, a);

	float dot00 = vec2_dot(v0, v0);
	float dot01 = vec2_dot(v0, v1);
	float dot02 = vec2_dot(v0, v2);
	float dot11 = vec2_dot(v1, v1);
	float dot12 = vec2_dot(v1, v2);

	float inv_det = 1.0f / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * inv_det;
	float v = (dot00 * dot12 - dot01 * dot02) * inv_det;

	*uout = u;
	*vout = v;

	return (u > 0.0f) && (v > 0.0f) && (u + v < 1.0f);
}

static inline bool point_in_cylinder(const vec3_t point,
				     const vec3_t origin, const vec3_t dir,
				     float height, float radius)
{
	/* TODO: It's possible to speed up this thing by using one
	 * to many pattern. Precalculate things like 'hsq' and 'rsq' for a
	 * cylinder and use them directly. Also there is '1.0f/hsq', also can
	 * be precomputed.
	 */
	vec3_t vechh, vech, o, pvec;
	float halfh = height * 0.5f;
	float hsq = height * height;
	float rsq = radius * radius;

	vec3_smul(vechh, dir, halfh);
	vec3_smul(vech, dir, height);

	vec3_sub(o, origin, vechh);
	vec3_sub(pvec, point, o);

	float d = vec3_dot(pvec, vech);

	if (d < 0.0f || d > hsq)
		return false;

	float dist = vec3_dot(pvec, pvec) - d*d / hsq;
	if (dist > rsq)
		return false;

	return true;
}

/* extents_out center is always == (0,0,0) */
void cylinder_to_obb(mat4_t basis_out, aabb_t extents_out,
		     const vec3_t origin, const vec3_t dir,
		     float height, float radius);

void basis_to_mat4(mat4_t out, const vec3_t origin, const vec3_t dir);

bool aabb_obb_intersection(const aabb_t aabb1, const mat4_t obb_basis,
			   const aabb_t obb);

/**************************************************************************
		   SSSS    SSSS   EEEEEE
		  S    S  S    S  E
		  S       S       E
		   SSSS    SSSS   EEEEEE
		       S       S  E
		  S    S  S    S  E
		   SSSS    SSSS   EEEEEE
**************************************************************************/

void mat4_sse_mul(mat4_sse_t out, const mat4_sse_t m1, const mat4_sse_t m2);
void quat_sse_mul(quat_sse_t out, const quat_sse_t q1, const quat_sse_t q2);
