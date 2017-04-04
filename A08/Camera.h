#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#endif //_MAIN_H

#include "RE\ReEngAppClass.h"
#include <SFML\Graphics.hpp>
//#include <chrono>

using namespace ReEng; //Using ReEng namespace to use all the classes in the dll

class Camera {
public:
	static Camera* GetInstance() {
		if (instance == nullptr) {
			instance = new Camera();
		}
		return instance;
	}
	static void ReleaseInstance() {
		if (instance != nullptr) {
			delete instance;
			instance = nullptr;
		}
	}
	Camera() {
		perspective = glm::perspective(45.0f, 1080.0f / 768.0f, 0.01f, 1000.0f);
		camLocation = IDENTITY_M4;
		v3_eye = glm::vec3(0.0f, 0.0f, 15.0f); //default eye position 15 units on the Z axis  
		v3_Target = glm::vec3(0.0f, 0.0f, 14.0f);
		v3_Up = glm::vec3(0.0f, 1.0f, 0.0f); //default up in the Y direction
	};
	
	~Camera() {};
	
	matrix4 GetView(void) {
		return glm::lookAt(v3_eye, v3_Target, v3_Up);
	};

	matrix4 getProjection(bool bOrthographic) {
		return perspective * camLocation;
	};

	void SetPosition(vector3 v3Position) {
		camLocation = glm::translate(v3Position);
	};

	void SetTarget(vector3 v3Target) {
		//return out if the target is behind our eye
		if (v3_Target.x < v3Target.x) { return; }
		else if (v3_Target.y < v3Target.y) { return; }
		else if (v3_Target.z < v3Target.z) { return; }

		v3_Target = v3Target;
	};

	void SetUp(vector3 v3Up) {
		v3_Up = v3Up;
	};

private:
	matrix4 perspective;
	matrix4 camLocation;

	vector3 v3_Up;
	vector3 v3_Target;
	vector3 v3_eye;
	//for example          EYE(forward)                  TARGET                        UP
	//glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 14.0f), glm::vec3(0.0f, 1.0f, 0.0f)

	static Camera* instance;

};
Camera* Camera::instance = nullptr;