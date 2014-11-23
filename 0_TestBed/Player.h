#pragma once
class Player
{
public:
	Player(void);
	~Player(void);
	int GetScore();
	void SetScore(int addit);
	float GetRotateAngle();
	void SetRotateAngle(float angle);
private:
	int score;
	float rotateAngle;
};

