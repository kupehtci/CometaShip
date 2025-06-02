
#ifndef COMETA_TIME_H
#define COMETA_TIME_H

#include "./Singleton.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"

class Time : public SingletonManager<Time> {

private: 
	float _deltaTime;
	float _timeScale;


	// utils
	float _lastFrameTime;	// Used to store last frame time to calculate _deltaTime

public: 
	Time();
	~Time() override = default;

	void Init() override;
	void Update() override; 
	void Close() override; 


private: 
	void UpdateDeltaTime(); 

	// --------- GETTERS AND SETTERS ---------
public: 
	float inline static GetDeltaTime() { return Time::GetInstancePtr()->_deltaTime; }

	float inline static GetTimeScale() { return Time::GetInstancePtr()->_timeScale; }
};


#endif // COMETA_TIME_H