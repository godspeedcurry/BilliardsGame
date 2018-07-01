#pragma once

class Table
{
	Ball balls[16];    // 16 balls
	int time;          // internal time for physics simulation
	double stickAngle; // current stick angle
	int player;    // current player
	int curNumber;     // current number of balls
	bool isPlay;
	bool isChange;
	int scores[2];

public:
	Table();
	void setStickAngle(double angle);
	int getPlayer();
	void switchPlayer();
	int getScores(int player);
	void setScores(int player, int offset);
	void reset();
	int visibleNumber();
	bool moving();
	void shoot();
	void update(int currentTime);
	void paintScore();
	void draw();
	void restore();
	void renew();
};