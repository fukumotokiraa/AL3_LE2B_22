#pragma once

#include"Vector3.h"
#include"Matrix4x4.h"

Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Vector3 Multiply(float scalar, const Vector3& v);

float Length(const Vector3& v);

Vector3 Normalize(const Vector3& v);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Matrix4x4 Multiply4x4(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 Inverse(const Matrix4x4& m); // 逆行列
