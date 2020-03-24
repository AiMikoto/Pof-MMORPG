#include "core/time_values.h"
#include "include/glad.h"
#include "include/glfw3.h"

double engine::Time::time = 0;
double engine::Time::deltaTime = 0;
double engine::Time::timeScale = 1.0;
double engine::Time::unscaledTime = 0;
double engine::Time::unscaledDeltaTime = 0;

void engine::Time::updateTimes() {
	updateGraphicsTimes();
}

void engine::Time::updateGraphicsTimes() {
	double unscaledLastTime = unscaledTime;
	unscaledTime = glfwGetTime();
	unscaledDeltaTime = unscaledTime - unscaledLastTime;
	deltaTime = timeScale * unscaledDeltaTime;
	time = timeScale * time;
}