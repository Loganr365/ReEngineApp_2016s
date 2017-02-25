#include "MyPrimitive.h"
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}

void MyPrimitive::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}

void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	std::vector<vector3> point;

	point.push_back(vector3(0.0, 0.0, 0.0 - a_fHeight /2));

	/*vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3*/

	float theta = 0.0f;
	float steps = 2 * PI / static_cast<float>(a_nSubdivisions);

	//populate points
	for (int i = 0; i < a_nSubdivisions; i++) {
		vector3 temp(cos(theta) * a_fRadius, sin(theta) * a_fRadius, 0.0 - a_fHeight / 2);
		point.push_back(temp);
		theta += steps;
	}

	//draw base
	for (int i = 1; i < a_nSubdivisions; i++) {
		AddTri(point[0], point[i + 1], point[i]);
		//AddTri(point[0], point[i], point[i + 1]); Swap Normals
	}

	//draw last triangle of base
	AddTri(point[0], point[1], point[a_nSubdivisions]);

	//draw sides
	vector3 highPoint(point[0].x, point[0].y, a_fHeight/2);

	for (int i = 1; i < a_nSubdivisions; i++) {
		AddTri(highPoint, point[i], point[i + 1]);
	}
	AddTri(highPoint, point[a_nSubdivisions], point[1]);

	//AddTri(point0, point1, point2);


	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	std::vector<vector3> bottomBase;
	std::vector<vector3> topBase;

	bottomBase.push_back(vector3(0.0, 0.0, 0 - a_fHeight/2));
	topBase.push_back(vector3(0, 0, 0 + a_fHeight/2));


	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float theta = 0.0f;
	float steps = 2 * PI / static_cast<float>(a_nSubdivisions);

	//=====Bottom Base=====
	//populate points
	for (int i = 0; i < a_nSubdivisions; i++) {
		vector3 temp(cos(theta) * a_fRadius, sin(theta) * a_fRadius, 0 - a_fHeight/2);
		bottomBase.push_back(temp);
		theta += steps;
	}

	//draw base
	for (int i = 1; i < a_nSubdivisions; i++) {
		AddTri(bottomBase[0], bottomBase[i + 1], bottomBase[i]);
		//AddTri(point[0], point[i], point[i + 1]); Swap Normals
	}
	//draw last triangle of base

	AddTri(bottomBase[0], bottomBase[1], bottomBase[a_nSubdivisions]);
	//=====Bottom Base END=====

	//=====Top Base=====

	//reset theta
	theta = 0;

	//populate points
	for (int i = 0; i < a_nSubdivisions; i++) {
		vector3 temp(cos(theta) * a_fRadius, sin(theta) * a_fRadius, 0 + a_fHeight/2);
		topBase.push_back(temp);
		theta += steps;
	}

	//draw base
	for (int i = 1; i < a_nSubdivisions; i++) {
		AddTri(topBase[0], topBase[i], topBase[i + 1]);
		//AddTri(point[0], point[i], point[i + 1]); Swap Normals
	}
	//draw last triangle of base

	AddTri(topBase[0], topBase[a_nSubdivisions], topBase[1]);
	//=====Bottom Base END=====

	//=====Draw Sides=====
	for (int i = 0; i < a_nSubdivisions; i++) {
		AddQuad(bottomBase[i], bottomBase[i + 1], topBase[i], topBase[i + 1]);
	}
	AddQuad(bottomBase[a_nSubdivisions], bottomBase[1], topBase[a_nSubdivisions], topBase[1]);
	//=====Draw Sides End=====

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	std::vector<vector3> topInner;
	std::vector<vector3> topOuter;
	std::vector<vector3> bottomOuter;
	std::vector<vector3> BottomInner;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();


	float theta = 0.0f;
	float steps = 2 * PI / static_cast<float>(a_nSubdivisions);

	//populate top
	for (int i = 0; i < a_nSubdivisions; i++) {
		vector3 temp(cos(theta) * a_fInnerRadius, sin(theta) * a_fInnerRadius, a_fHeight / 2);
		vector3 temp2(cos(theta) * a_fOuterRadius, sin(theta) * a_fOuterRadius, a_fHeight / 2);
		topInner.push_back(temp);
		topOuter.push_back(temp2);
		theta += steps;
	}
	theta = 0.0f;
	//populate bottom
	for (int i = 0; i < a_nSubdivisions; i++) {
		vector3 temp(cos(theta) * a_fInnerRadius, sin(theta) * a_fInnerRadius, 0 - a_fHeight / 2);
		vector3 temp2(cos(theta) * a_fOuterRadius, sin(theta) * a_fOuterRadius, 0 - a_fHeight / 2);
		BottomInner.push_back(temp);
		bottomOuter.push_back(temp2);
		theta += steps;
	}

	//draw top
	for (int i = 0; i < a_nSubdivisions -1; i++) {
		AddQuad(topInner[i + 1], topInner[i], topOuter[i + 1], topOuter[i]);
	}
	AddQuad(topInner[0], topInner[a_nSubdivisions - 1], topOuter[0], topOuter[a_nSubdivisions - 1]);

	//draw bottom
	for (int i = 0; i < a_nSubdivisions - 1; i++) {
		AddQuad(BottomInner[i], BottomInner[i + 1], bottomOuter[i], bottomOuter[i + 1]);
	}
	AddQuad(BottomInner[a_nSubdivisions - 1], BottomInner[0], bottomOuter[a_nSubdivisions - 1], bottomOuter[0]);

	//draw inner side
	for (int i = 0; i < a_nSubdivisions - 1; i++) {
		AddQuad(BottomInner[i + 1], BottomInner[i], topInner[i + 1], topInner[i]);
	}
	AddQuad(BottomInner[0], BottomInner[a_nSubdivisions - 1], topInner[0], topInner[a_nSubdivisions - 1]);

	//draw outter side
	for (int i = 0; i < a_nSubdivisions - 1; i++) {
		AddQuad(bottomOuter[i], bottomOuter[i + 1], topOuter[i], topOuter[i + 1]);
	}
	AddQuad(bottomOuter[a_nSubdivisions - 1], bottomOuter[0], topOuter[a_nSubdivisions - 1], topOuter[0]);

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	AddQuad(point0, point1, point3, point2);

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	std::vector<vector3> points;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
/*	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;*/

	Release();
	Init();

	//Your code starts here

	float theta = 0.0f;
	float steps = 2 * PI / static_cast<float>(a_nSubdivisions);
	float phiSteps = PI / static_cast<float>(a_nSubdivisions);
	float phi = 0.0f;

	for (int i = 0; i < a_nSubdivisions; i++) {
		vector3 temp(a_fRadius * cos(theta) * sin(phi), a_fRadius * sin(theta) * sin(phi), a_fRadius * cos(phi));
		points.push_back(temp);
		theta += steps;
		phi += phiSteps;
	}

	//generate points
	/*float startU = 0.0f;
	float startV = 0.0f;
	float endU = PI * 2;
	float endV = PI;
	float stepU = (endU - startU) / (a_fRadius / 2);
	float stepV = (endV - startV) / (a_fRadius / 2);
	
	for (int i = 0; i < a_nSubdivisions; i++) {
		for (int j = 0; j < a_nSubdivisions; i++) {
			float u = i * stepU + startU;
			float v = j * stepV + startV;
			float un = (i + 1) * stepU + startU;
			float vn = (j + 1) * stepV + startV;


		}
	}*/

	for (int i = 0; i < a_nSubdivisions - 3; i++) {
	AddQuad(points[i], points[i + 1], points[i + 2], points[i + 3]);
	}
	/*AddQuad(points[a_nSubdivisions - 1], points[a_nSubdivisions - 2], points[a_nSubdivisions - 3], points[0]);
	AddQuad(points[a_nSubdivisions - 2], points[a_nSubdivisions - 3], points[0], points[1]);
	AddQuad(points[a_nSubdivisions - 3], points[0], points[1], points[2]);*/

	/*float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	AddQuad(point0, point1, point3, point2);*/

	//Your code ends here
	CompileObject(a_v3Color);
}