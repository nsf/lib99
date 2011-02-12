#include "linear_math.h"
//#include <xmmintrin.h>
//#include <pmmintrin.h>
#include <float.h>

/**************************************************************************
  vec2: two-dimensional vector
**************************************************************************/

void vec2_rotate(vec2_t out, float deg, const vec2_t origin)
{
	deg *= MATH_DEG_TO_RAD;
	float cosdeg = cosf(deg);
	float sindeg = sinf(deg);

	vec2_suba(out, origin);
	vec2_set(out,
		 VEC_X(out) * cosdeg - VEC_Y(out) * sindeg,
		 VEC_X(out) * sindeg + VEC_Y(out) * cosdeg);
	vec2_adda(out, origin);
}

/**************************************************************************
  vec3: three-dimensional vector
**************************************************************************/

void vec3_rotate_x(vec3_t out, float deg, const vec3_t origin)
{
	deg *= MATH_DEG_TO_RAD;
	float cosdeg = cosf(deg);
	float sindeg = sinf(deg);

	VEC_Y(out) -= VEC_Y(origin);
	VEC_Z(out) -= VEC_Z(origin);
	vec3_set(out,
		 VEC_X(out),
		 VEC_Y(out) * cosdeg - VEC_Z(out) * sindeg,
		 VEC_Y(out) * sindeg + VEC_Z(out) * cosdeg);
	VEC_Y(out) += VEC_Y(origin);
	VEC_Z(out) += VEC_Z(origin);
}

void vec3_rotate_y(vec3_t out, float deg, const vec3_t origin)
{
	deg *= MATH_DEG_TO_RAD;
	float cosdeg = cosf(deg);
	float sindeg = sinf(deg);

	VEC_X(out) -= VEC_X(origin);
	VEC_Z(out) -= VEC_Z(origin);
	vec3_set(out,
		 VEC_Z(out) * sindeg + VEC_X(out) * cosdeg,
		 VEC_Y(out),
		 VEC_Z(out) * cosdeg - VEC_X(out) * sindeg);
	VEC_X(out) += VEC_X(origin);
	VEC_Z(out) += VEC_Z(origin);
}

void vec3_rotate_z(vec3_t out, float deg, const vec3_t origin)
{
	vec2_rotate(out, deg, origin);
}

/**************************************************************************
  mat4: 4x4 OpenGL matrix
**************************************************************************/

void mat4_copy(mat4_t out, const mat4_t m)
{
	memcpy(out, m, sizeof(float) * 16);
}

void mat4_transform_vec3(vec3_t v, const mat4_t m)
{
	vec3_t r;
	r[0] = m[0] * v[0] + m[4] * v[1] + m[8]  * v[2] + m[12];
	r[1] = m[1] * v[0] + m[5] * v[1] + m[9]  * v[2] + m[13];
	r[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14];
	vec3_copy(v, r);
}

// Resulting matrix is the m2 in the coordinate system of m1.
// In other words we're transforming m2 coordinate system by m1.
void mat4_mul(mat4_t out, const mat4_t m1, const mat4_t m2)
{
	out[0]  = m1[0] * m2[0]  + m1[4] * m2[1]  + m1[8]  * m2[2]  + m1[12] * m2[3];
	out[1]  = m1[1] * m2[0]  + m1[5] * m2[1]  + m1[9]  * m2[2]  + m1[13] * m2[3];
	out[2]  = m1[2] * m2[0]  + m1[6] * m2[1]  + m1[10] * m2[2]  + m1[14] * m2[3];
	out[3]  = m1[3] * m2[0]  + m1[7] * m2[1]  + m1[11] * m2[2]  + m1[15] * m2[3];

	out[4]  = m1[0] * m2[4]  + m1[4] * m2[5]  + m1[8]  * m2[6]  + m1[12] * m2[7];
	out[5]  = m1[1] * m2[4]  + m1[5] * m2[5]  + m1[9]  * m2[6]  + m1[13] * m2[7];
	out[6]  = m1[2] * m2[4]  + m1[6] * m2[5]  + m1[10] * m2[6]  + m1[14] * m2[7];
	out[7]  = m1[3] * m2[4]  + m1[7] * m2[5]  + m1[11] * m2[6]  + m1[15] * m2[7];

	out[8]  = m1[0] * m2[8]  + m1[4] * m2[9]  + m1[8]  * m2[10] + m1[12] * m2[11];
	out[9]  = m1[1] * m2[8]  + m1[5] * m2[9]  + m1[9]  * m2[10] + m1[13] * m2[11];
	out[10] = m1[2] * m2[8]  + m1[6] * m2[9]  + m1[10] * m2[10] + m1[14] * m2[11];
	out[11] = m1[3] * m2[8]  + m1[7] * m2[9]  + m1[11] * m2[10] + m1[15] * m2[11];

	out[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8]  * m2[14] + m1[12] * m2[15];
	out[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9]  * m2[14] + m1[13] * m2[15];
	out[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
	out[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
}

bool mat4_equals(const mat4_t m1, const mat4_t m2)
{
	return (fabs(m1[0] - m2[0])   < MATH_EPSILON &&
		fabs(m1[1] - m2[1])   < MATH_EPSILON &&
		fabs(m1[2] - m2[2])   < MATH_EPSILON &&
		fabs(m1[3] - m2[3])   < MATH_EPSILON &&
		fabs(m1[4] - m2[4])   < MATH_EPSILON &&
		fabs(m1[5] - m2[5])   < MATH_EPSILON &&
		fabs(m1[6] - m2[6])   < MATH_EPSILON &&
		fabs(m1[7] - m2[7])   < MATH_EPSILON &&
		fabs(m1[8] - m2[8])   < MATH_EPSILON &&
		fabs(m1[9] - m2[9])   < MATH_EPSILON &&
		fabs(m1[10] - m2[10]) < MATH_EPSILON &&
		fabs(m1[11] - m2[11]) < MATH_EPSILON &&
		fabs(m1[12] - m2[12]) < MATH_EPSILON &&
		fabs(m1[13] - m2[13]) < MATH_EPSILON &&
		fabs(m1[14] - m2[14]) < MATH_EPSILON &&
		fabs(m1[15] - m2[15]) < MATH_EPSILON);

}

void mat4_set_identity(mat4_t out)
{
	out[0] = 1.0f; out[4] = 0.0f; out[8]  = 0.0f; out[12] = 0.0f;
	out[1] = 0.0f; out[5] = 1.0f; out[9]  = 0.0f; out[13] = 0.0f;
	out[2] = 0.0f; out[6] = 0.0f; out[10] = 1.0f; out[14] = 0.0f;
	out[3] = 0.0f; out[7] = 0.0f; out[11] = 0.0f; out[15] = 1.0f;
}

void mat4_set_zero(mat4_t out)
{
	out[0] = 0.0f; out[4] = 0.0f; out[8]  = 0.0f; out[12] = 0.0f;
	out[1] = 0.0f; out[5] = 0.0f; out[9]  = 0.0f; out[13] = 0.0f;
	out[2] = 0.0f; out[6] = 0.0f; out[10] = 0.0f; out[14] = 0.0f;
	out[3] = 0.0f; out[7] = 0.0f; out[11] = 0.0f; out[15] = 0.0f;
}

bool mat4_is_identity(const mat4_t m)
{
	return (fabs(m[0] - 1.0f)  < MATH_EPSILON &&
		fabs(m[1])         < MATH_EPSILON &&
		fabs(m[2])         < MATH_EPSILON &&
		fabs(m[3])         < MATH_EPSILON &&
		fabs(m[4])         < MATH_EPSILON &&
		fabs(m[5] - 1.0f)  < MATH_EPSILON &&
		fabs(m[6])         < MATH_EPSILON &&
		fabs(m[7])         < MATH_EPSILON &&
		fabs(m[8])         < MATH_EPSILON &&
		fabs(m[9])         < MATH_EPSILON &&
		fabs(m[10] - 1.0f) < MATH_EPSILON &&
		fabs(m[11])        < MATH_EPSILON &&
		fabs(m[12])        < MATH_EPSILON &&
		fabs(m[13])        < MATH_EPSILON &&
		fabs(m[14])        < MATH_EPSILON &&
		fabs(m[15] - 1.0f) < MATH_EPSILON);
}

float mat4_determinant(const mat4_t m)
{
	float d;
	d =  m[0]  * (m[5] * (m[10] * m[15] - m[14] * m[11]) - m[9] * (m[6] * m[15] - m[14] * m[7]) + m[13] * (m[6] * m[11] - m[10] * m[7]));
	d -= m[4]  * (m[1] * (m[10] * m[15] - m[14] * m[11]) - m[9] * (m[2] * m[15] - m[14] * m[3]) + m[13] * (m[2] * m[11] - m[10] * m[3]));
	d += m[8]  * (m[1] * (m[6]  * m[15] - m[14] * m[7])  - m[5] * (m[2] * m[15] - m[14] * m[3]) + m[13] * (m[2] * m[7]  - m[6]  * m[3]));
	d -= m[12] * (m[1] * (m[6]  * m[11] - m[10] * m[7])  - m[5] * (m[2] * m[11] - m[10] * m[3]) + m[9]  * (m[2] * m[7]  - m[6]  * m[3]));
	return d;
}

void mat4_inverse(mat4_t out, const mat4_t m)
{
	float d, id;

	d = mat4_determinant(m);
	if (fabs(d) < MATH_EPSILON)
		return;
	id = 1.0f / d;
	out[0]  =  (m[5] * (m[10] * m[15] - m[14] * m[11]) - m[9] * (m[6] * m[15] - m[14] * m[7]) + m[13] * (m[6] * m[11] - m[10] * m[7])) * id;
	out[1]  = -(m[1] * (m[10] * m[15] - m[14] * m[11]) - m[9] * (m[2] * m[15] - m[14] * m[3]) + m[13] * (m[2] * m[11] - m[10] * m[3])) * id;
	out[2]  =  (m[1] * (m[6]  * m[15] - m[14] * m[7])  - m[5] * (m[2] * m[15] - m[14] * m[3]) + m[13] * (m[2] * m[7]  - m[6]  * m[3])) * id;
	out[3]  = -(m[1] * (m[6]  * m[11] - m[10] * m[7])  - m[5] * (m[2] * m[11] - m[10] * m[3]) + m[9]  * (m[2] * m[7]  - m[6]  * m[3])) * id;
	out[4]  = -(m[4] * (m[10] * m[15] - m[14] * m[11]) - m[8] * (m[6] * m[15] - m[14] * m[7]) + m[12] * (m[6] * m[11] - m[10] * m[7])) * id;
	out[5]  =  (m[0] * (m[10] * m[15] - m[14] * m[11]) - m[8] * (m[2] * m[15] - m[14] * m[3]) + m[12] * (m[2] * m[11] - m[10] * m[3])) * id;
	out[6]  = -(m[0] * (m[6]  * m[15] - m[14] * m[7])  - m[4] * (m[2] * m[15] - m[14] * m[3]) + m[12] * (m[2] * m[7]  - m[6]  * m[3])) * id;
	out[7]  =  (m[0] * (m[6]  * m[11] - m[10] * m[7])  - m[4] * (m[2] * m[11] - m[10] * m[3]) + m[8]  * (m[2] * m[7]  - m[6]  * m[3])) * id;
	out[8]  =  (m[4] * (m[9]  * m[15] - m[13] * m[11]) - m[8] * (m[5] * m[15] - m[13] * m[7]) + m[12] * (m[5] * m[11] - m[9]  * m[7])) * id;
	out[9]  = -(m[0] * (m[9]  * m[15] - m[13] * m[11]) - m[8] * (m[1] * m[15] - m[13] * m[3]) + m[12] * (m[1] * m[11] - m[9]  * m[3])) * id;
	out[10] =  (m[0] * (m[5]  * m[15] - m[13] * m[7])  - m[4] * (m[1] * m[15] - m[13] * m[3]) + m[12] * (m[1] * m[7]  - m[5]  * m[3])) * id;
	out[11] = -(m[0] * (m[5]  * m[11] - m[9]  * m[7])  - m[4] * (m[1] * m[11] - m[9]  * m[3]) + m[8]  * (m[1] * m[7]  - m[5]  * m[3])) * id;
	out[12] = -(m[4] * (m[9]  * m[14] - m[13] * m[10]) - m[8] * (m[5] * m[14] - m[13] * m[6]) + m[12] * (m[5] * m[10] - m[9]  * m[6])) * id;
	out[13] =  (m[0] * (m[9]  * m[14] - m[13] * m[10]) - m[8] * (m[1] * m[14] - m[13] * m[2]) + m[12] * (m[1] * m[10] - m[9]  * m[2])) * id;
	out[14] = -(m[0] * (m[5]  * m[14] - m[13] * m[6])  - m[4] * (m[1] * m[14] - m[13] * m[2]) + m[12] * (m[1] * m[6]  - m[5]  * m[2])) * id;
	out[15] =  (m[0] * (m[5]  * m[10] - m[9]  * m[6])  - m[4] * (m[1] * m[10] - m[9]  * m[2]) + m[8]  * (m[1] * m[6]  - m[5]  * m[2])) * id;
}

void mat4_set_rotate(mat4_t out, const vec3_t axis, float angle)
{
	vec3_t v;
	float rad = angle * MATH_DEG_TO_RAD;
	float c = cosf(rad);
	float s = sinf(rad);
	vec3_copy(v, axis);
	vec3_normalize(v);
	float xx = v[0] * v[0];
	float yy = v[1] * v[1];
	float zz = v[2] * v[2];
	float xy = v[0] * v[1];
	float yz = v[1] * v[2];
	float zx = v[2] * v[0];
	float xs = v[0] * s;
	float ys = v[1] * s;
	float zs = v[2] * s;
	out[0] = (1.0f - c) * xx + c;  out[4] = (1.0f - c) * xy - zs; out[8]  = (1.0f - c) * zx + ys; out[12] = 0.0f;
	out[1] = (1.0f - c) * xy + zs; out[5] = (1.0f - c) * yy + c;  out[9]  = (1.0f - c) * yz - xs; out[13] = 0.0f;
	out[2] = (1.0f - c) * zx - ys; out[6] = (1.0f - c) * yz + xs; out[10] = (1.0f - c) * zz + c;  out[14] = 0.0f;
	out[3] = 0.0f;                 out[7] = 0.0f;                 out[11] = 0.0f;                 out[15] = 1.0f;
}

void mat4_set_rotate_x(mat4_t out, float angle)
{
	float rad = angle * MATH_DEG_TO_RAD;
	float c = cosf(rad);
	float s = sinf(rad);
	out[0] = 1.0f; out[4] = 0.0f; out[8]  = 0.0f; out[12] = 0.0f;
	out[1] = 0.0f; out[5] = c;    out[9]  = -s;   out[13] = 0.0f;
	out[2] = 0.0f; out[6] = s;    out[10] = c;    out[14] = 0.0f;
	out[3] = 0.0f; out[7] = 0.0f; out[11] = 0.0f; out[15] = 1.0f;
}

void mat4_set_rotate_y(mat4_t out, float angle)
{
	float rad = angle * MATH_DEG_TO_RAD;
	float c = cosf(rad);
	float s = sinf(rad);
	out[0] = c;    out[4] = 0.0f; out[8]  = s;    out[12] = 0.0f;
	out[1] = 0.0f; out[5] = 1.0f; out[9]  = 0.0f; out[13] = 0.0f;
	out[2] = -s;   out[6] = 0.0f; out[10] = c;    out[14] = 0.0f;
	out[3] = 0.0f; out[7] = 0.0f; out[11] = 0.0f; out[15] = 1.0f;
}

void mat4_set_rotate_z(mat4_t out, float angle)
{
	float rad = angle * MATH_DEG_TO_RAD;
	float c = cosf(rad);
	float s = sinf(rad);
	out[0] = c;    out[4] = -s;   out[8]  = 0.0f; out[12] = 0.0f;
	out[1] = s;    out[5] = c;    out[9]  = 0.0f; out[13] = 0.0f;
	out[2] = 0.0f; out[6] = 0.0f; out[10] = 1.0f; out[14] = 0.0f;
	out[3] = 0.0f; out[7] = 0.0f; out[11] = 0.0f; out[15] = 1.0f;
}

void mat4_set_scale(mat4_t out, const vec3_t scale)
{
	out[0] = scale[0]; out[4] = 0.0f;     out[8]  = 0.0f;     out[12] = 0.0f;
	out[1] = 0.0f;     out[5] = scale[1]; out[9]  = 0.0f;     out[13] = 0.0f;
	out[2] = 0.0f;     out[6] = 0.0f;     out[10] = scale[2]; out[14] = 0.0f;
	out[3] = 0.0f;     out[7] = 0.0f;     out[11] = 0.0f;     out[15] = 1.0f;
}

void mat4_set_translate(mat4_t out, const vec3_t translate)
{
	out[0] = 1.0f; out[4] = 0.0f; out[8]  = 0.0f; out[12] = translate[0];
	out[1] = 0.0f; out[5] = 1.0f; out[9]  = 0.0f; out[13] = translate[1];
	out[2] = 0.0f; out[6] = 0.0f; out[10] = 1.0f; out[14] = translate[2];
	out[3] = 0.0f; out[7] = 0.0f; out[11] = 0.0f; out[15] = 1.0f;
}

void mat4_set_perspective(mat4_t out, float fov, float aspect,
			  float znear, float zfar)
{
	if (fabs(fov - 90.0f) < MATH_EPSILON)
		fov = 89.9f;

	float y = tanf(fov * MATH_PI / 360.0f);
	float x = y * aspect;

	out[0] = 1.0f / x;
	out[1] = 0.0f;
	out[2] = 0.0f;
	out[3] = 0.0f;

	out[4] = 0.0f;
	out[5] = 1.0f / y;
	out[6] = 0.0f;
	out[7] = 0.0f;

	out[8] = 0.0f;
	out[9] = 0.0f;
	out[10] = -(zfar + znear) / (zfar - znear);
	out[11] = -1.0f;

	out[12] = 0.0f;
	out[13] = 0.0f;
	out[14] = -(2.0f * zfar * znear) / (zfar - znear);
	out[15] = 0.0f;
}

void mat4_set_ortho(mat4_t out, float left, float right,
		    float bottom, float top, float znear, float zfar)
{
	float x = 2.0f / (right - left);
	float y = 2.0f / (top - bottom);
	float z = -2.0f / (zfar - znear);
	float tx = - ((right + left) / (right - left));
	float ty = - ((top + bottom) / (top - bottom));
	float tz = - ((zfar + znear) / (zfar - znear));

	out[0] = x;
	out[1] = 0.0f;
	out[2] = 0.0f;
	out[3] = 0.0f;

	out[4] = 0.0f;
	out[5] = y;
	out[6] = 0.0f;
	out[7] = 0.0f;

	out[8] = 0.0f;
	out[9] = 0.0f;
	out[10] = z;
	out[11] = 0.0f;

	out[12] = tx;
	out[13] = ty;
	out[14] = tz;
	out[15] = 1.0f;
}

void mat4_set_look_at(mat4_t out, const vec3_t eye,
		      const vec3_t center, const vec3_t up)
{
	vec3_t n,u,s,neye;
	mat4_t translate, tmp;

	vec3_sub(n, eye, center);
	vec3_normalize(n);

	vec3_cross(s, up, n);
	vec3_normalize(s);

	vec3_cross(u, n, s);
	vec3_normalize(u);

	tmp[0] = s[0];  tmp[4] = s[1];  tmp[8]  = s[2];  tmp[12] = 0.0f;
	tmp[1] = u[0];  tmp[5] = u[1];  tmp[9]  = u[2];  tmp[13] = 0.0f;
	tmp[2] = n[0];  tmp[6] = n[1];  tmp[10] = n[2];  tmp[14] = 0.0f;
	tmp[3] = 0.0f;  tmp[7] = 0.0f;  tmp[11] = 0.0f;  tmp[15] = 1.0f;

	vec3_neg(neye, eye);
	mat4_set_translate(translate, neye);
	mat4_mul(out, tmp, translate);
}

/**************************************************************************
  quat: quaterion representing rotation
**************************************************************************/

void quat_from_mat4(quat_t out, const mat4_t m)
{
	float trace = m[0] + m[5] + m[10];
	if (trace > 0.0f) {
		float s = sqrtf(trace + 1.0f);
		out[3] = 0.5f * s;
		s = 0.5f / s;
		out[0] = (m[6] - m[9]) * s;
		out[1] = (m[8] - m[2]) * s;
		out[2] = (m[1] - m[4]) * s;
	} else {
		static int next[3] = { 1, 2, 0 };
		int i = 0;
		if (m[5] > m[0]) i = 1;
		if (m[10] > m[4 * i + i]) i = 2;
		int j = next[i];
		int k = next[j];
		float s = sqrtf(m[4 * i + i] -
				m[4 * j + j] -
				m[4 * k + k] + 1.0f);
		out[i] = 0.5f * s;
		if (s != 0) s = 0.5f / s;
		out[3] = (m[4 * j + k] - m[4 * k + j]) * s;
		out[j] = (m[4 * i + j] + m[4 * j + i]) * s;
		out[k] = (m[4 * i + k] + m[4 * k + i]) * s;
	}
}

void quat_mul(quat_t out, const quat_t q1, const quat_t q2)
{
	out[0] = q1[3] * q2[0] + q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1];
	out[1] = q1[3] * q2[1] + q1[1] * q2[3] + q1[2] * q2[0] - q1[0] * q2[2];
	out[2] = q1[3] * q2[2] + q1[2] * q2[3] + q1[0] * q2[1] - q1[1] * q2[0];
	out[3] = q1[3] * q2[3] - q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2];
}

void quat_to_mat4(mat4_t out, const quat_t q)
{
	float x2 = q[0] + q[0];
	float y2 = q[1] + q[1];
	float z2 = q[2] + q[2];
	float xx = q[0] * x2;
	float yy = q[1] * y2;
	float zz = q[2] * z2;
	float xy = q[0] * y2;
	float yz = q[1] * z2;
	float xz = q[2] * x2;
	float wx = q[3] * x2;
	float wy = q[3] * y2;
	float wz = q[3] * z2;
	out[0] = 1.0f - (yy + zz); out[4] = xy - wz;          out[8]  = xz + wy;          out[12] = 0.0f;
	out[1] = xy + wz;          out[5] = 1.0f - (xx + zz); out[9]  = yz - wx;          out[13] = 0.0f;
	out[2] = xz - wy;          out[6] = yz + wx;          out[10] = 1.0f - (xx + yy); out[14] = 0.0f;
	out[3] = 0.0f;             out[7] = 0.0f;             out[11] = 0.0f;             out[15] = 1.0f;
}

void quat_slerp(quat_t out, const quat_t q1, const quat_t q2, float t)
{
	float k0,k1,cosomega = q1[0] * q2[0] + q1[1] * q2[1] +
			       q1[2] * q2[2] + q1[3] * q2[3];
	quat_t q;
	if (cosomega < 0.0f) {
		cosomega = -cosomega;
		q[0] = -q1[0];
		q[1] = -q1[1];
		q[2] = -q1[2];
		q[3] = -q1[3];
	} else {
		q[0] = q1[0];
		q[1] = q1[1];
		q[2] = q1[2];
		q[3] = q1[3];
	}
	if (1.0f - cosomega > MATH_EPSILON) {
		float omega = acosf(cosomega);
		float sinomega = sinf(omega);
		k0 = sinf((1.0f - t) * omega) / sinomega;
		k1 = sinf(t * omega) / sinomega;
	} else {
		k0 = 1.0f - t;
		k1 = t;
	}

	out[0] = q1[0] * k0 + q[0] * k1;
	out[1] = q1[1] * k0 + q[1] * k1;
	out[2] = q1[2] * k0 + q[2] * k1;
	out[3] = q1[3] * k0 + q[3] * k1;
}

void quat_from_angle(quat_t out, const vec3_t dir, float angle)
{
	out[0] = out[1] = out[2] = 0.0f;
	out[3] = 1.0f;

	float length = vec3_length(dir);
	if (length != 0.0f)
	{
		float halfangle = angle * MATH_DEG_TO_RAD / 2.0f;
		length = 1.0f / length;
		float sinangle = sinf(halfangle);
		out[0] = dir[0] * length * sinangle;
		out[1] = dir[1] * length * sinangle;
		out[2] = dir[2] * length * sinangle;
		out[3] = cosf(halfangle);
	}
}

/**************************************************************************
  aabb: axis-aligned bounding box
**************************************************************************/

void aabb_add_point(aabb_t aabb, const vec3_t point)
{
	if (AABB_MIN_X(aabb) > VEC_X(point))
		AABB_MIN_X(aabb) = VEC_X(point);
	if (AABB_MIN_Y(aabb) > VEC_Y(point))
		AABB_MIN_Y(aabb) = VEC_Y(point);
	if (AABB_MIN_Z(aabb) > VEC_Z(point))
		AABB_MIN_Z(aabb) = VEC_Z(point);

	if (AABB_MAX_X(aabb) < VEC_X(point))
		AABB_MAX_X(aabb) = VEC_X(point);
	if (AABB_MAX_Y(aabb) < VEC_Y(point))
		AABB_MAX_Y(aabb) = VEC_Y(point);
	if (AABB_MAX_Z(aabb) < VEC_Z(point))
		AABB_MAX_Z(aabb) = VEC_Z(point);
}

void aabb_add_aabb(aabb_t aabb1, const aabb_t aabb2)
{
	aabb_add_point(aabb1, AABB_MIN(aabb2));
	aabb_add_point(aabb1, AABB_MAX(aabb2));
}

/**************************************************************************
  plane: three-dimensional plane
**************************************************************************/

enum plane_side plane_side_point(const plane_t plane, const vec3_t point)
{
	if (vec3_dot(plane, point) + PLANE_D(plane) >= 0.0f)
		return PLANE_FRONT;
	return PLANE_BACK;
}

enum plane_side plane_side_aabb(const plane_t plane, const aabb_t aabb)
{
	vec3_t near, far;
	vec3_copy(near, AABB_MAX(aabb));
	vec3_copy(far, AABB_MIN(aabb));

	if (PLANE_NX(plane) > 0) {
		VEC_X(near) = AABB_MIN_X(aabb);
		VEC_X(far) = AABB_MAX_X(aabb);
	}
	if (PLANE_NY(plane) > 0) {
		VEC_Y(near) = AABB_MIN_Y(aabb);
		VEC_Y(far) = AABB_MAX_Y(aabb);
	}
	if (PLANE_NZ(plane) > 0) {
		VEC_Z(near) = AABB_MIN_Z(aabb);
		VEC_Z(far) = AABB_MAX_Z(aabb);
	}

	if (vec3_dot(plane, near) + PLANE_D(plane) > 0)
		return PLANE_FRONT;
	if (vec3_dot(plane, far) + PLANE_D(plane) > 0)
		return PLANE_BOTH;
	return PLANE_BACK;
}

/**************************************************************************
  frustum
**************************************************************************/

void frustum_set(frustum_t out, const mat4_t m)
{
	int i;

	/* left clipping plane */
	PLANE_NX(FRUSTUM_PLANE(out, FRUSTUM_PLANE_LEFT))   = m[3]  + m[0];
	PLANE_NY(FRUSTUM_PLANE(out, FRUSTUM_PLANE_LEFT))   = m[7]  + m[4];
	PLANE_NZ(FRUSTUM_PLANE(out, FRUSTUM_PLANE_LEFT))   = m[11] + m[8];
	PLANE_D(FRUSTUM_PLANE(out, FRUSTUM_PLANE_LEFT))    = m[15] + m[12];

	/* right clipping plane */
	PLANE_NX(FRUSTUM_PLANE(out, FRUSTUM_PLANE_RIGHT))  = m[3]  - m[0];
	PLANE_NY(FRUSTUM_PLANE(out, FRUSTUM_PLANE_RIGHT))  = m[7]  - m[4];
	PLANE_NZ(FRUSTUM_PLANE(out, FRUSTUM_PLANE_RIGHT))  = m[11] - m[8];
	PLANE_D(FRUSTUM_PLANE(out, FRUSTUM_PLANE_RIGHT))   = m[15] - m[12];

	/* top clipping plane */
	PLANE_NX(FRUSTUM_PLANE(out, FRUSTUM_PLANE_TOP))    = m[3]  - m[1];
	PLANE_NY(FRUSTUM_PLANE(out, FRUSTUM_PLANE_TOP))    = m[7]  - m[5];
	PLANE_NZ(FRUSTUM_PLANE(out, FRUSTUM_PLANE_TOP))    = m[11] - m[9];
	PLANE_D(FRUSTUM_PLANE(out, FRUSTUM_PLANE_TOP))     = m[15] - m[13];

	/* bottom clipping plane */
	PLANE_NX(FRUSTUM_PLANE(out, FRUSTUM_PLANE_BOTTOM)) = m[3]  + m[1];
	PLANE_NY(FRUSTUM_PLANE(out, FRUSTUM_PLANE_BOTTOM)) = m[7]  + m[5];
	PLANE_NZ(FRUSTUM_PLANE(out, FRUSTUM_PLANE_BOTTOM)) = m[11] + m[9];
	PLANE_D(FRUSTUM_PLANE(out, FRUSTUM_PLANE_BOTTOM))  = m[15] + m[13];

	/* far clipping plane */
	PLANE_NX(FRUSTUM_PLANE(out, FRUSTUM_PLANE_FAR))    = m[3]  - m[2];
	PLANE_NY(FRUSTUM_PLANE(out, FRUSTUM_PLANE_FAR))    = m[7]  - m[6];
	PLANE_NZ(FRUSTUM_PLANE(out, FRUSTUM_PLANE_FAR))    = m[11] - m[10];
	PLANE_D(FRUSTUM_PLANE(out, FRUSTUM_PLANE_FAR))     = m[15] - m[14];

	/* near clipping plane */
	PLANE_NX(FRUSTUM_PLANE(out, FRUSTUM_PLANE_NEAR))   = m[3]  + m[2];
	PLANE_NY(FRUSTUM_PLANE(out, FRUSTUM_PLANE_NEAR))   = m[7]  + m[6];
	PLANE_NZ(FRUSTUM_PLANE(out, FRUSTUM_PLANE_NEAR))   = m[11] + m[10];
	PLANE_D(FRUSTUM_PLANE(out, FRUSTUM_PLANE_NEAR))    = m[15] + m[14];

	for (i = 0; i < 6; ++i) {
		const float ilen = 1.0f / vec3_length(FRUSTUM_PLANE(out, i));
		vec3_smula(FRUSTUM_PLANE(out, i), ilen);
		PLANE_D(FRUSTUM_PLANE(out, i)) *= ilen;
	}
}

enum frustum_side frustum_side_aabb(const frustum_t frustum, const aabb_t aabb)
{
	int i;
	enum frustum_side result = FRUSTUM_OUTSIDE;
	for (i = 0; i < 6; ++i) {
		enum plane_side ps = plane_side_aabb(FRUSTUM_PLANE(frustum, i), aabb);
		switch (ps) {
		case PLANE_FRONT:
			if (result != FRUSTUM_BOTH)
				result = FRUSTUM_INSIDE;
			break;
		case PLANE_BOTH:
			result = FRUSTUM_BOTH;
			break;
		case PLANE_BACK:
			return FRUSTUM_OUTSIDE;
		}
	}
	return result;
}

/**************************************************************************
  intersections
**************************************************************************/

static bool aabb_is_point_inside(const aabb_t aabb, const vec3_t point)
{
	return (AABB_MIN_X(aabb) <= VEC_X(point) &&
		AABB_MIN_Y(aabb) <= VEC_Y(point) &&
		AABB_MIN_Z(aabb) <= VEC_Z(point) &&
		AABB_MAX_X(aabb) >= VEC_X(point) &&
		AABB_MAX_Y(aabb) >= VEC_Y(point) &&
		AABB_MAX_Z(aabb) >= VEC_Z(point));
}

bool aabb_clip_ray(const aabb_t aabb, const vec3_t orig, const vec3_t dir,
		   float *t_min, float *t_max)
{
	vec3_t inv_dir;
	vec3_inv(inv_dir, dir);

	unsigned int a;
	if (aabb_is_point_inside(aabb, orig)) {
		*t_min = 0.0f;
		float t_max_min = FLT_MAX;
		for (a = 0; a < 3; ++a) {
			float tmin = (AABB_MIN(aabb)[a] - orig[a]) * inv_dir[a];
			float tmax = (AABB_MAX(aabb)[a] - orig[a]) * inv_dir[a];
			if (tmin > 0.0f)
				t_max_min = MIN(t_max_min, tmin);
			if (tmax > 0.0f)
				t_max_min = MIN(t_max_min, tmax);
		}
		*t_max = t_max_min;
		return true;
	}

	float t_min_max = 0.0f;
	float t_max_min = FLT_MAX;

	for (a = 0; a < 3; ++a) {
		float tmin = (AABB_MIN(aabb)[a] - orig[a]) * inv_dir[a];
		float tmax = (AABB_MAX(aabb)[a] - orig[a]) * inv_dir[a];

		if (AABB_MIN(aabb)[a] > orig[a]) {
			t_min_max = MAX(t_min_max, tmin);
			t_max_min = MIN(t_max_min, tmax);
		} else if (AABB_MAX(aabb)[a] < orig[a]) {
			t_min_max = MAX(t_min_max, tmax);
			t_max_min = MIN(t_max_min, tmin);
		}
	}

	if (t_min_max > t_max_min) {
		return false;
	}

	*t_min = t_min_max;
	*t_max = t_max_min;
	return true;
}

void basis_to_mat4(mat4_t out, const vec3_t origin, const vec3_t d)
{
	vec3_t u, v;
	if (fabs(d[0]) >= fabs(d[1])) {
		float f = 1.0f / sqrtf(d[0]*d[0] + d[2]*d[2]);
		u[0] = -d[2]*f;
		u[1] = 0;
		u[2] = d[0]*f;
	} else {
		float f = 1.0f / sqrtf(d[1]*d[1] + d[2]*d[2]);
		u[0] = 0;
		u[1] = d[2]*f;
		u[2] = -d[1]*f;
	}
	vec3_cross(v, u, d);
	out[0] = v[0]; out[4] = d[0]; out[8]  = u[0]; out[12] = origin[0];
	out[1] = v[1]; out[5] = d[1]; out[9]  = u[1]; out[13] = origin[1];
	out[2] = v[2]; out[6] = d[2]; out[10] = u[2]; out[14] = origin[2];
	out[3] = 0.0f; out[7] = 0.0f; out[11] = 0.0f; out[15] = 1.0f;
}

void cylinder_to_obb(mat4_t basis_out, aabb_t extents_out,
		     const vec3_t origin, const vec3_t dir,
		     float height, float radius)
{
	float halfh = height * 0.5f;
	basis_to_mat4(basis_out, origin, dir);
	AABB_MIN_X(extents_out) = -radius;
	AABB_MIN_Y(extents_out) = -halfh;
	AABB_MIN_Z(extents_out) = -radius;
	AABB_MAX_X(extents_out) = radius;
	AABB_MAX_Y(extents_out) = halfh;
	AABB_MAX_Z(extents_out) = radius;
}

bool aabb_obb_intersection(const aabb_t aabb, const mat4_t basis,
			   const aabb_t obb)
{
	vec3_t a, b, aabb_center, T;
	float ra, rb, t;

	vec3_sub(a, AABB_MAX(aabb), AABB_MIN(aabb));
	vec3_smula(a, 0.5f);
	vec3_sub(b, AABB_MAX(obb), AABB_MIN(obb));
	vec3_smula(b, 0.5f);

	vec3_add(aabb_center, AABB_MIN(aabb), AABB_MAX(aabb));
	vec3_smula(aabb_center, 0.5f);
	T[0] = basis[12] - aabb_center[0];
	T[1] = basis[13] - aabb_center[1];
	T[2] = basis[14] - aabb_center[2];

	for (unsigned int i = 0; i < 3; ++i) {
		ra = a[i];
		rb = b[0] * fabs(MAT4_RC(basis, 0, i)) +
		     b[1] * fabs(MAT4_RC(basis, 1, i)) +
		     b[2] * fabs(MAT4_RC(basis, 2, i));
		t  = fabs(T[i]);
		if (t > ra + rb)
			return false;
	}
	for (unsigned int i = 0; i < 3; ++i) {
		ra = a[0] * fabs(MAT4_RC(basis, i, 0)) +
		     a[1] * fabs(MAT4_RC(basis, i, 1)) +
		     a[2] * fabs(MAT4_RC(basis, i, 2));
		rb = b[i];
		t  = fabs(T[0] * MAT4_RC(basis, i, 0) +
			  T[1] * MAT4_RC(basis, i, 1) +
			  T[2] * MAT4_RC(basis, i, 2));
		if (t > ra + rb)
			return false;
	}

	/* 9 cross products */

	/* L = A0 x B0 */
	ra = a[1] * fabs(MAT4_RC(basis, 0, 2)) + a[2] * fabs(MAT4_RC(basis, 0, 1));
	rb = b[1] * fabs(MAT4_RC(basis, 2, 0)) + b[2] * fabs(MAT4_RC(basis, 1, 0));
	t = fabs(T[2] * MAT4_RC(basis, 0, 1) - T[1] * MAT4_RC(basis, 0, 2));
	if (t > ra + rb)
		return false;

	/* L = A0 x B1 */
	ra = a[1] * fabs(MAT4_RC(basis, 1, 2)) + a[2] * fabs(MAT4_RC(basis, 1, 1));
	rb = b[0] * fabs(MAT4_RC(basis, 2, 0)) + b[2] * fabs(MAT4_RC(basis, 0, 0));
	t = fabs(T[2] * MAT4_RC(basis, 1, 1) - T[1] * MAT4_RC(basis, 1, 2));
	if (t > ra + rb)
		return false;

	/* L = A0 x B2 */
	ra = a[1] * fabs(MAT4_RC(basis, 2, 2)) + a[2] * fabs(MAT4_RC(basis, 2, 1));
	rb = b[0] * fabs(MAT4_RC(basis, 1, 0)) + b[1] * fabs(MAT4_RC(basis, 0, 0));
	t = fabs(T[2] * MAT4_RC(basis, 2, 1) - T[1] * MAT4_RC(basis, 2, 2));
	if (t > ra + rb)
		return false;

	/* L = A1 x B0 */
	ra = a[0] * fabs(MAT4_RC(basis, 0, 2)) + a[2] * fabs(MAT4_RC(basis, 0, 0));
	rb = b[1] * fabs(MAT4_RC(basis, 2, 1)) + b[2] * fabs(MAT4_RC(basis, 1, 1));
	t = fabs(T[0] * MAT4_RC(basis, 0, 2) - T[2] * MAT4_RC(basis, 0, 0));
	if (t > ra + rb)
		return false;

	/* L = A1 x B1 */
	ra = a[0] * fabs(MAT4_RC(basis, 1, 2)) + a[2] * fabs(MAT4_RC(basis, 1, 0));
	rb = b[0] * fabs(MAT4_RC(basis, 2, 1)) + b[2] * fabs(MAT4_RC(basis, 0, 1));
	t = fabs(T[0] * MAT4_RC(basis, 1, 2) - T[2] * MAT4_RC(basis, 1, 0));
	if (t > ra + rb)
		return false;

	/* L = A1 x B2 */
	ra = a[0] * fabs(MAT4_RC(basis, 2, 2)) + a[2] * fabs(MAT4_RC(basis, 2, 0));
	rb = b[0] * fabs(MAT4_RC(basis, 1, 1)) + b[1] * fabs(MAT4_RC(basis, 0, 1));
	t = fabs(T[0] * MAT4_RC(basis, 2, 2) - T[2] * MAT4_RC(basis, 2, 0));
	if (t > ra + rb)
		return false;

	/* L = A2 x B0 */
	ra = a[0] * fabs(MAT4_RC(basis, 0, 1)) + a[1] * fabs(MAT4_RC(basis, 0, 0));
	rb = b[1] * fabs(MAT4_RC(basis, 2, 2)) + b[2] * fabs(MAT4_RC(basis, 1, 2));
	t = fabs(T[1] * MAT4_RC(basis, 0, 0) - T[0] * MAT4_RC(basis, 0, 1));
	if (t > ra + rb)
		return false;

	/* L = A2 x B1 */
	ra = a[0] * fabs(MAT4_RC(basis, 1, 1)) + a[1] * fabs(MAT4_RC(basis, 1, 0));
	rb = b[0] * fabs(MAT4_RC(basis, 2, 2)) + b[2] * fabs(MAT4_RC(basis, 0, 2));
	t = fabs(T[1] * MAT4_RC(basis, 1, 0) - T[0] * MAT4_RC(basis, 1, 1));
	if (t > ra + rb)
		return false;

	/* L = A2 x B2 */
	ra = a[0] * fabs(MAT4_RC(basis, 2, 1)) + a[1] * fabs(MAT4_RC(basis, 2, 0));
	rb = b[0] * fabs(MAT4_RC(basis, 1, 2)) + b[1] * fabs(MAT4_RC(basis, 0, 2));
	t = fabs(T[1] * MAT4_RC(basis, 2, 0) - T[0] * MAT4_RC(basis, 2, 1));
	if (t > ra + rb)
		return false;

	return true;
}

/**************************************************************************
		   SSSS    SSSS   EEEEEE
		  S    S  S    S  E
		  S       S       E
		   SSSS    SSSS   EEEEEE
		       S       S  E
		  S    S  S    S  E
		   SSSS    SSSS   EEEEEE
**************************************************************************/

#if 0
void mat4_sse_mul(mat4_sse_t out, const mat4_sse_t m1, const mat4_sse_t m2)
{
	register __m128 x0, x1, x2, x3, x4, x5, x6, x7;

	x0 = _mm_load_ps(&m1[0]);
	x1 = _mm_load_ps(&m1[4]);
	x2 = _mm_load_ps(&m1[8]);
	x3 = _mm_load_ps(&m1[12]);

	x4 = _mm_load_ps1(&m2[0]);
	x5 = _mm_load_ps1(&m2[1]);
	x6 = _mm_load_ps1(&m2[2]);
	x7 = _mm_load_ps1(&m2[3]);

	x4 = _mm_mul_ps(x4, x0);
	x5 = _mm_mul_ps(x5, x1);
	x6 = _mm_mul_ps(x6, x2);
	x7 = _mm_mul_ps(x7, x3);

	x4 = _mm_add_ps(x4, x5);
	x6 = _mm_add_ps(x6, x7);
	x4 = _mm_add_ps(x4, x6);

	x5 = _mm_load_ps1(&m2[4]);
	x6 = _mm_load_ps1(&m2[5]);
	x7 = _mm_load_ps1(&m2[6]);
	x5 = _mm_mul_ps(x5, x0);
	x6 = _mm_mul_ps(x6, x1);
	x7 = _mm_mul_ps(x7, x2);
	x5 = _mm_add_ps(x5, x6);
	x5 = _mm_add_ps(x5, x7);
	x6 = _mm_load_ps1(&m2[7]);
	x6 = _mm_mul_ps(x6, x3);
	x5 = _mm_add_ps(x5, x6);

	x6 = _mm_load_ps1(&m2[8]);
	x7 = _mm_load_ps1(&m2[9]);
	x6 = _mm_mul_ps(x6, x0);
	x7 = _mm_mul_ps(x7, x1);
	x6 = _mm_add_ps(x6, x7);
	x7 = _mm_load_ps1(&m2[10]);
	x7 = _mm_mul_ps(x7, x2);
	x6 = _mm_add_ps(x6, x7);
	x7 = _mm_load_ps1(&m2[11]);
	x7 = _mm_mul_ps(x7, x3);
	x6 = _mm_add_ps(x6, x7);

	x7 = _mm_load_ps1(&m2[12]);
	x0 = _mm_mul_ps(x0, x7);
	x7 = _mm_load_ps1(&m2[13]);
	x1 = _mm_mul_ps(x1, x7);
	x7 = _mm_load_ps1(&m2[14]);
	x2 = _mm_mul_ps(x2, x7);
	x7 = _mm_load_ps1(&m2[15]);

	x3 = _mm_mul_ps(x3, x7);
	x0 = _mm_add_ps(x0, x1);
	x2 = _mm_add_ps(x2, x3);
	x0 = _mm_add_ps(x0, x2);

	/* store result */
	_mm_store_ps(&out[0], x4);
	_mm_store_ps(&out[4], x5);
	_mm_store_ps(&out[8], x6);
	_mm_store_ps(&out[12], x0);
}

#define vec4_swizzle(v, x, y, z, w)	\
	_mm_shuffle_ps((v),(v), _MM_SHUFFLE(w, z, y, x))

static const uint32_t change_w_sign_mask_i[4] __attribute__((aligned(16))) = {
	0, 0, 0, 0x80000000
};

void quat_sse_mul(quat_sse_t out, const quat_sse_t q1, const quat_sse_t q2)
{
	/*
	             mul0            mul1            mul2            mul3
	out[0] = q1[3] * q2[0] + q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1];
	out[1] = q1[3] * q2[1] + q1[1] * q2[3] + q1[2] * q2[0] - q1[0] * q2[2];
	out[2] = q1[3] * q2[2] + q1[2] * q2[3] + q1[0] * q2[1] - q1[1] * q2[0];
	out[3] = q1[3] * q2[3] - q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2];
	*/

	__m128 a = _mm_load_ps(q1);
	__m128 b = _mm_load_ps(q2);

	__m128 s0 = vec4_swizzle(a, 3, 3, 3, 3);

	__m128 s1 = vec4_swizzle(a, 0, 1, 2, 0);
	__m128 s2 = vec4_swizzle(b, 3, 3, 3, 0);

	__m128 s3 = vec4_swizzle(a, 1, 2, 0, 1);
	__m128 s4 = vec4_swizzle(b, 2, 0, 1, 1);

	__m128 s5 = vec4_swizzle(a, 2, 0, 1, 2);
	__m128 s6 = vec4_swizzle(b, 1, 2, 0, 2);

	__m128 mul0 = _mm_mul_ps(s0, b);
	__m128 mul1 = _mm_mul_ps(s1, s2);
	__m128 mul2 = _mm_mul_ps(s3, s4);
	__m128 mul3 = _mm_mul_ps(s5, s6);

	__m128 change_w_sign_mask = _mm_load_ps((float*)change_w_sign_mask_i);

	/* flip w sign for mul1 and mul2 */
	mul1 = _mm_xor_ps(mul1, change_w_sign_mask);
	mul2 = _mm_xor_ps(mul2, change_w_sign_mask);

	__m128 mul2_m_mul3 = _mm_sub_ps(mul2, mul3);
	__m128 mul1_p_mul2m3 = _mm_add_ps(mul1, mul2_m_mul3);
	__m128 result = _mm_add_ps(mul0, mul1_p_mul2m3);
	_mm_store_ps(out, result);
}
#endif
