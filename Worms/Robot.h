#pragma once
#include "Model.h"
class Robot
{
public:
	Model body, left_arm, right_arm, ball, eyes, missile;
	bool isTurnRight = false;
	bool onGround = true;
	bool isShooting = false;
	bool isAdjustingMissileSpeed = false;
	int  direction = 1;
	float altitude = 0.0f;
	float verticalSpeed = 0.0f;
	float horizontalSpeed = 0.0f;
	float missile_speed = 0.0f;
	float gravity = -9.8f;
	float arm_angle = 0.0f;
	float currentHealth = 100.0f;
	glm::mat4 M = glm::mat4(1.0);

	int missileFlyTime = 0;
	float missileX = 0, missileY = 0; //current position
	float missile_translate_y = 0.77206f; //position in model world
	float missile_translate_x = 0.0f;
	float rememberAngle;
	bool shootingRight = false;

	void turnFaceSide();
	void jump(float distance);
	void calculateGravity(int time);
	void calculateShot(int time, float windspeed);
	void Draw(glm::mat4 &view);
	void Shot(unsigned int power);

	Robot();
	~Robot();
};

