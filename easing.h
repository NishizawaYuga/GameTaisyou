#pragma once
class EASE {
public:
	static double InQuad(double change, double base, double duration, double time) { //chargeが移動量、baseが最初の地点、durationが移動時間、timeが経過時間
		time /= duration;
		return change * time * time + base;
	};
	static double OutQuad(double change, double base, double duration, double time) {
		time /= duration;
		return -change * time * (time - 2) + base;
	};
	static double InOutQuad(double change, double base, double duration, double time) {
		time /= duration / 2;
		if (time < 1) return change / 2 * time * time + base;
		return -change / 2 * ((--time) * (time - 2) - 1) + base;
	};
};