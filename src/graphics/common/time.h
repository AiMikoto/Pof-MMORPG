#pragma once

namespace engine {
	struct Time {
		//graphics times
		static double deltaTime, time, unscaledDeltaTime, unscaledTime;
		static double timeScale; //a number between 0-1
		//physics times
		static double fixedDeltaTime, fixedTime, fixedUnscaledDeltaTime, fixedUnscaledTime;
		static double maxDeltaTime; //how much time the physics engine is allowed to spend on a frame on the client side

		static void updateTimes();
	private:
		static void updateGraphicsTimes();
	};
}