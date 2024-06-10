#pragma once

#include"Vector3.h"
#include"Matrix4x4.h"

Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
