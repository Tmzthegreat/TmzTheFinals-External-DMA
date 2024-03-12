#pragma once

#include <d3d9types.h>
//#include <d3dx9math.h>
#include <d3d9.h>
//#include <d3dx9tex.h>
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#define M_PI 3.14159265358979323846264338327950288419716939937510

typedef struct entitylist
{
	int i[10000];
	int sum = 0;
};



class Vec2
{
public:
	double x, y;

	Vec2() : x(0.0), y(0.0) {}
	Vec2(double x_, double y_) : x(x_), y(y_) {}
	Vec2(ImVec2 ImVec2_) : x(static_cast<double>(ImVec2_.x)), y(static_cast<double>(ImVec2_.y)) {}

	Vec2& operator=(const ImVec2& ImVec2_)
	{
		x = static_cast<double>(ImVec2_.x);
		y = static_cast<double>(ImVec2_.y);
		return *this;
	}

	Vec2 operator+(const Vec2& Vec2_)
	{
		return { x + Vec2_.x, y + Vec2_.y };
	}

	Vec2 operator-(const Vec2& Vec2_)
	{
		return { x - Vec2_.x, y - Vec2_.y };
	}

	Vec2 operator*(const Vec2& Vec2_)
	{
		return { x * Vec2_.x, y * Vec2_.y };
	}

	Vec2 operator/(const Vec2& Vec2_)
	{
		return { x / Vec2_.x, y / Vec2_.y };
	}

	Vec2 operator*(double n)
	{
		return { x * n, y * n };
	}

	Vec2 operator/(double n)
	{
		return { x / n, y / n };
	}

	bool operator==(const Vec2& Vec2_)
	{
		return x == Vec2_.x && y == Vec2_.y;
	}

	bool operator!=(const Vec2& Vec2_)
	{
		return x != Vec2_.x || y != Vec2_.y;
	}

	ImVec2 ToImVec2() const
	{
		return ImVec2(static_cast<float>(x), static_cast<float>(y));
	}

	double Length()
	{
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	double DistanceTo(const Vec2& Pos)
	{
		return sqrt(pow(Pos.x - x, 2) + pow(Pos.y - y, 2));
	}

	bool IsZero() const {
		return x == 0.0 && y == 0.0;
	}
};


class Vec3
{
public:
	double x, y, z;

	Vec3() : x(0.0), y(0.0), z(0.0) {}
	Vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

	Vec3 operator+(const Vec3& Vec3_)
	{
		return { x + Vec3_.x, y + Vec3_.y, z + Vec3_.z };
	}

	Vec3 operator-(const Vec3& Vec3_)
	{
		return { x - Vec3_.x, y - Vec3_.y, z - Vec3_.z };
	}

	Vec3 operator*(const Vec3& Vec3_)
	{
		return { x * Vec3_.x, y * Vec3_.y, z * Vec3_.z };
	}

	Vec3 operator/(const Vec3& Vec3_)
	{
		return { x / Vec3_.x, y / Vec3_.y, z / Vec3_.z };
	}

	Vec3 operator*(double n)
	{
		return { x * n, y * n, z * n };
	}

	Vec3 operator/(double n)
	{
		return { x / n, y / n, z / n };
	}

	bool operator==(const Vec3& Vec3_)
	{
		return x == Vec3_.x && y == Vec3_.y && z == Vec3_.z;
	}

	bool operator!=(const Vec3& Vec3_)
	{
		return x != Vec3_.x || y != Vec3_.y || z != Vec3_.z;
	}

	double Length()
	{
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	double DistanceTo(const Vec3& Pos)
	{
		return sqrt(pow(Pos.x - x, 2) + pow(Pos.y - y, 2) + pow(Pos.z - z, 2));
	}
};


typedef struct _MAT4X4
{
	double c[4][4];
}MAT4X4, * PMAT4X4;
class Transform {
public:
	double Matrix[4][4]{};
	MAT4X4 viewMatrix1;
	double screenWidth2 = GetSystemMetrics(SM_CXSCREEN);
	double screenHeight2 = GetSystemMetrics(SM_CYSCREEN);



	bool WorldToScreen(const Vec3& pos, Vec3& screenPos) {
		double cameraZ = Matrix[0][3] * pos.x + Matrix[1][3] * pos.y + Matrix[2][3] * pos.z + Matrix[3][3];
		if (cameraZ < 0.01)
			return false;

		double screenCenterX = screenWidth2 / 2.0;
		double screenCenterY = screenHeight2 / 2.0;
		double ratio = 1 / cameraZ;

		screenPos.x = screenCenterX + (Matrix[0][0] * pos.x + Matrix[1][0] * pos.y + Matrix[2][0] * pos.z + Matrix[3][0]) * ratio * screenCenterX;
		screenPos.y = screenCenterY - (Matrix[0][1] * pos.x + Matrix[1][1] * pos.y + Matrix[2][1] * (pos.z + 120) + Matrix[3][1]) * ratio * screenCenterY;
		screenPos.z = screenCenterY - (Matrix[0][1] * pos.x + Matrix[1][1] * pos.y + Matrix[2][1] * (pos.z - 120) + Matrix[3][1]) * ratio * screenCenterY;

		return true;
	}


	bool bone2s(Vec3& Ô¤ÅÐ, Vec3* ×ø±ê) {
		float ViewW, BoxX, BoxY, BoxY1, BoxH;
		ViewW = viewMatrix1.c[1 - 1][4 - 1] * Ô¤ÅÐ.x + viewMatrix1.c[2 - 1][4 - 1] * Ô¤ÅÐ.y + viewMatrix1.c[3 - 1][4 - 1] * Ô¤ÅÐ.z + viewMatrix1.c[4 - 1][4 - 1];

		if (ViewW < 0.01)
			return false;
		if (ViewW > 0.01) {
			ViewW = 1 / ViewW;
			BoxX = screenWidth2 / 2 + (viewMatrix1.c[1 - 1][1 - 1] * Ô¤ÅÐ.x + viewMatrix1.c[2 - 1][1 - 1] * Ô¤ÅÐ.y + viewMatrix1.c[3 - 1][1 - 1] * Ô¤ÅÐ.z + viewMatrix1.c[4 - 1][1 - 1]) * ViewW * screenWidth2 / 2;
			BoxY = screenHeight2 / 2 - (viewMatrix1.c[1 - 1][2 - 1] * Ô¤ÅÐ.x + viewMatrix1.c[2 - 1][2 - 1] * Ô¤ÅÐ.y + viewMatrix1.c[3 - 1][2 - 1] * Ô¤ÅÐ.z + viewMatrix1.c[4 - 1][2 - 1]) * ViewW * screenHeight2 / 2;
			float BoxZ = screenHeight2 / 2 - (viewMatrix1.c[1 - 1][2 - 1] * Ô¤ÅÐ.x + viewMatrix1.c[2 - 1][2 - 1] * Ô¤ÅÐ.y + viewMatrix1.c[3 - 1][2 - 1] * (Ô¤ÅÐ.z - 120) + viewMatrix1.c[4 - 1][2 - 1]) * ViewW * screenHeight2 / 2;
			
			×ø±ê->x = BoxX;
			×ø±ê->y = BoxY;
			×ø±ê->z = BoxZ;
			
		}

		return true;
	}
};

struct FQuat
{
	double x;
	double y;
	double z;
	double w;
};
class FVector
{
public:
	FVector() : x(0.f), y(0.f), z(0.f)
	{

	}

	FVector(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{

	}
	~FVector()
	{

	}

	double x;
	double y;
	double z;

	inline double Dot(FVector v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline double Distance(FVector v)
	{
		float x = this->x - v.x;
		float y = this->y - v.y;
		float z = this->z - v.z;

		return sqrtf((x * x) + (y * y) + (z * z)) * 0.03048f;
	}

	FVector operator+(FVector v)
	{
		return FVector(x + v.x, y + v.y, z + v.z);
	}

	FVector operator-(FVector v)
	{
		return FVector(x - v.x, y - v.y, z - v.z);
	}

	FVector operator*(double number) const {
		return FVector(x * number, y * number, z * number);
	}

	__forceinline double Magnitude() const {
		return sqrtf(x * x + y * y + z * z);
	}

	inline double Length()
	{
		return sqrtf((x * x) + (y * y) + (z * z));
	}

	__forceinline FVector Normalize() {
		FVector vector;
		double length = this->Magnitude();

		if (length != 0) {
			vector.x = x / length;
			vector.y = y / length;
			vector.z = z / length;
		}
		else {
			vector.x = vector.y = 0.0f;
			vector.z = 1.0f;
		}
		return vector;
	}

	__forceinline FVector& operator+=(const FVector& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
};
struct FTransform
{
	FQuat rot;
	FVector translation;
	char pad[4];
	FVector scale;
	char pad1[4];

	D3DMATRIX ToMatrixWithScale()
	{
		D3DMATRIX m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;
	return pOut;
}

MAT4X4 MultiplyMatrices(const MAT4X4& a, const MAT4X4& b) {
	MAT4X4 result;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.c[i][j] = 0.0;
			for (int k = 0; k < 4; ++k) {
				result.c[i][j] += a.c[i][k] * b.c[k][j];
			}
		}
	}

	return result;
}

MAT4X4 InverseRotationMatrix(const MAT4X4& rotationMatrix) {
	MAT4X4 inverseMatrix;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			inverseMatrix.c[i][j] = rotationMatrix.c[j][i];
		}
	}

	inverseMatrix.c[3][0] = 0.0;
	inverseMatrix.c[3][1] = 0.0;
	inverseMatrix.c[3][2] = 0.0;
	inverseMatrix.c[3][3] = 1.0;

	return inverseMatrix;
}
MAT4X4 Matrix2(FVector rot, FVector origin) {

	double radYaw = rot.y * M_PI / 180.0;
	double radPitch = rot.x * M_PI / 180.0;
	double radRoll = rot.z * M_PI / 180.0;

	MAT4X4 yawMatrix;
	yawMatrix.c[0][0] = cos(radYaw);
	yawMatrix.c[0][1] = -sin(radYaw);
	yawMatrix.c[0][2] = 0.0;
	yawMatrix.c[0][3] = 0.0;
	yawMatrix.c[1][0] = sin(radYaw);
	yawMatrix.c[1][1] = cos(radYaw);
	yawMatrix.c[1][2] = 0.0;
	yawMatrix.c[1][3] = 0.0;
	yawMatrix.c[2][0] = 0.0;
	yawMatrix.c[2][1] = 0.0;
	yawMatrix.c[2][2] = 1.0;
	yawMatrix.c[2][3] = 0.0;
	yawMatrix.c[3][0] = 0.0;
	yawMatrix.c[3][1] = 0.0;
	yawMatrix.c[3][2] = 0.0;
	yawMatrix.c[3][3] = 1.0;


	MAT4X4 pitchMatrix;
	pitchMatrix.c[0][0] = cos(radPitch);
	pitchMatrix.c[0][1] = 0.0;
	pitchMatrix.c[0][2] = sin(radPitch);
	pitchMatrix.c[0][3] = 0.0;
	pitchMatrix.c[1][0] = 0.0;
	pitchMatrix.c[1][1] = 1.0;
	pitchMatrix.c[1][2] = 0.0;
	pitchMatrix.c[1][3] = 0.0;
	pitchMatrix.c[2][0] = -sin(radPitch);
	pitchMatrix.c[2][1] = 0.0;
	pitchMatrix.c[2][2] = cos(radPitch);
	pitchMatrix.c[2][3] = 0.0;
	pitchMatrix.c[3][0] = 0.0;
	pitchMatrix.c[3][1] = 0.0;
	pitchMatrix.c[3][2] = 0.0;
	pitchMatrix.c[3][3] = 1.0;


	MAT4X4 rollMatrix;
	rollMatrix.c[0][0] = 1.0;
	rollMatrix.c[0][1] = 0.0;
	rollMatrix.c[0][2] = 0.0;
	rollMatrix.c[0][3] = 0.0;
	rollMatrix.c[1][0] = 0.0;
	rollMatrix.c[1][1] = cos(radRoll);
	rollMatrix.c[1][2] = -sin(radRoll);
	rollMatrix.c[1][3] = 0.0;
	rollMatrix.c[2][0] = 0.0;
	rollMatrix.c[2][1] = sin(radRoll);
	rollMatrix.c[2][2] = cos(radRoll);
	rollMatrix.c[2][3] = 0.0;
	rollMatrix.c[3][0] = 0.0;
	rollMatrix.c[3][1] = 0.0;
	rollMatrix.c[3][2] = 0.0;
	rollMatrix.c[3][3] = 1.0;

	MAT4X4 translationMatrix;
	translationMatrix.c[0][0] = 1.0;
	translationMatrix.c[0][1] = 0.0;
	translationMatrix.c[0][2] = 0.0;
	translationMatrix.c[0][3] = 0.0;

	translationMatrix.c[1][0] = 0.0;
	translationMatrix.c[1][1] = 1.0;
	translationMatrix.c[1][2] = 0.0;
	translationMatrix.c[1][3] = 0.0;

	translationMatrix.c[2][0] = 0.0;
	translationMatrix.c[2][1] = 0.0;
	translationMatrix.c[2][2] = 1.0;
	translationMatrix.c[2][3] = 0.0;

	translationMatrix.c[3][0] = origin.x;
	translationMatrix.c[3][1] = origin.y;
	translationMatrix.c[3][2] = origin.z;
	translationMatrix.c[3][3] = 1.0;

	MAT4X4 rotationMatrix1 = MultiplyMatrices(yawMatrix, pitchMatrix);
	MAT4X4 rotationMatrix2 = MultiplyMatrices(rotationMatrix1, rollMatrix);
	MAT4X4 rotationMatrix3 = MultiplyMatrices(rotationMatrix2, translationMatrix);

	return rotationMatrix3;
}

D3DMATRIX Matrix(FVector rot, FVector origin)
{
	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = 0.0;
	matrix.m[3][1] = 0.0;
	matrix.m[3][2] = 0.0;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

struct BoneConnection {
	int fromId;
	int toId;
};

// ¶¨Òå¹Ç÷ÀÖ®¼äµÄÁ¬½Ó
BoneConnection boneConnections[] = {
	{7, 8},
	{8, 9},
	{9, 10},
	{10, 11},
	{8, 32},
	{32, 33},
	{33, 34},
	{8, 4}, // ²±×Óµ½ÐØ²¿
	{4, 1},
	{1, 70}, // Åè¹Çµ½×ó´óÍÈ
	{70, 71}, // ×ó´óÍÈµ½×óÐ¡ÍÈ
	{1, 66}, // Åè¹Çµ½ÓÒ´óÍÈ
	{66, 67} // ÓÒ´óÍÈµ½ÓÒÐ¡ÍÈ
};




struct FMinimalViewInfo {
	FVector Locations; // 0x00(0x0c)
	FVector Rotations; // 0x0c(0x0c)
	float FOVs; // 0x18(0x04)
};


