#include "Player.h"


Player::Player(void)
{
	score = 0;
	rotateAngle = 0.0f;
}


Player::~Player(void)
{
}

int Player::GetScore() {return score; }
void Player::SetScore(int addit){ score += addit; }
float Player::GetRotateAngle() { return rotateAngle; }
void Player::SetRotateAngle(float angle) {rotateAngle = angle; }
