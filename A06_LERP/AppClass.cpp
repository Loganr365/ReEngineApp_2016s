#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;

	points = {
		vector3(-4.0f, -2.0f, 5.0f),
		vector3(1.0f, -2.0f, 5.0f),
		vector3(-3.0f, -1.0f, 3.0f),
		vector3(2.0f, -1.0f, 3.0f),
		vector3(-2.0f, 0.0f, 0.0f),
		vector3(3.0f, 0.0f, 0.0f),
		vector3(-1.0f, 1.0f, -3.0f),
		vector3(4.0f, 1.0f, -3.0f),
		vector3(0.0f, 2.0f, -5.0f),
		vector3(5.0f, 2.0f, -5.0f),
		vector3(1.0f, 3.0f, -5.0f)
	};
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	//m_pMeshMngr->SetModelMatrix(IDENTITY_M4, "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	int rounds = 0;
	const float ROUND_TIME = 55.0f;
	const float TRAVEL_TIME = 5.0f;
	static bool completed = false;

	//Print info on the screen
	static DWORD startTimeSystem = GetTickCount();
	static int currentPoint = 0;
	DWORD timeApplication = GetTickCount() - startTimeSystem;
	float timer = timeApplication / 1000.0f;

	for (int i = 0; i < points.size(); i++) {
		matrix4 m4Location = IDENTITY_M4;
		m4Location = glm::scale(0.5f, 0.5f, 0.5f);
		m4Location = glm::translate(points[i]);
		m_pMeshMngr->AddSphereToRenderList(m4Location, vector3(0.5f, 0.0f, 0.0f), 1);
	}

	m_pMeshMngr->PrintLine("");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->PrintLine("Time is: " + std::to_string(timer));
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);

	matrix4 m4WallEye;
	vector3 v3Lerp;
	
	if (!completed) 
	{
		float timerMapped = MapValue(timer, 0.0f + (currentPoint * TRAVEL_TIME + (ROUND_TIME * rounds)), 5.0f + (currentPoint * TRAVEL_TIME + (ROUND_TIME * rounds)), 0.0f, 1.0f);
		if (timerMapped > 1.0f) {
			timerMapped = 0.0f;
			currentPoint++;
		}

		if (currentPoint >= points.size()) { currentPoint = 0; rounds++; completed = true; }
		else if (currentPoint < points.size() - 1) { v3Lerp = glm::lerp(points[currentPoint], points[currentPoint + 1], timerMapped); }
		else if (currentPoint == points.size() - 1) { v3Lerp = glm::lerp(points[currentPoint], points[0], timerMapped); }
	}

	//m4WallEye = glm::translate(vector3(1.0f, 0.0f, 0.0f));
	m4WallEye = glm::translate(v3Lerp);
	m_pMeshMngr->SetModelMatrix(m4WallEye, "WallEye");
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}