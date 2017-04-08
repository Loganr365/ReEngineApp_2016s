#include "AppClass.h"

void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("E10_Projections"); // Window Name
}
void AppClass::InitVariables(void)
{
	//Generate the Cone
	
	theCam->GetInstance();

	m_pCone = new PrimitiveClass();
	m_pCone->GenerateCone(1.0f, 1.0f, 10, RERED);

	//Generate the Cylinder
	m_pCylinder = new PrimitiveClass();
	m_pCylinder->GenerateCylinder(1.0f, 1.0f, 10, REGREEN);

	//Calculate the first projections
	

	//m_m4Projection = glm::perspective(45.0f, 1080.0f / 768.0f, 0.01f, 1000.0f);
	//m_m4Projection = glm::ortho(-10.0f, 10.0f, -5.0f, 5.0f, 0.01f, 1000.0f);

	//TEMPARY LOOK AT
//	m_m4View = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 14.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Testing code
	
	/* test SetPosition PASSED
	theCam->SetPosition(glm::vec3(0.0f, -5.0f, 0.0f)); //Test the SetPosition function by raising the camera up 
	theCam->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f)); //Test the SetPosition function by raising the camera and then down
	*/

	//test SetTarget PASSED
	//theCam->SetTarget(glm::vec3(0.0f, -5.0f, 1.0f));
	
	//test Movement PASSED
	//theCam->MoveVertical(5.0f);
	//theCam->MoveSideways(5.0f);
	//theCam->MoveForward(5.0f);
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	ProcessKeyboard();
	//this.AppClass.ProcessKeyboard();
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	
	//Render the grid
	m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XY);

	//Render the cone
	m_pCone->Render(theCam->getProjection(true), theCam->GetView(), IDENTITY_M4);

	//Render the cylinder
	m_pCylinder->Render(theCam->getProjection(true), theCam->GetView(), glm::translate(IDENTITY_M4, REAXISZ * -3.0f));

	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}



void AppClass::Release(void)
{
	//Release the memory of the member fields
	SafeDelete(m_pCone);
	SafeDelete(m_pCylinder);

	//Release the memory of the inherited fields
	super::Release(); 
}