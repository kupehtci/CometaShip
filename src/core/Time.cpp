#include "Time.h"


Time::Time() {
	_deltaTime = 0;
	_timeScale = 1.0f; 
	_lastFrameTime = 0.0f;
}

void Time::Init() {
	COMETA_ASSERT("Initialized Time singleton correctly");
	_lastFrameTime = (float)glfwGetTime();
}

void Time::Update() {

	UpdateDeltaTime(); 
}

void Time::Close() {
	COMETA_ASSERT("Close Time singleton correctly"); 
}


// Private Methods
void Time::UpdateDeltaTime() {
	float currentFrameTime = (float)glfwGetTime();
	_deltaTime = currentFrameTime - _lastFrameTime;
	_lastFrameTime = currentFrameTime;
}