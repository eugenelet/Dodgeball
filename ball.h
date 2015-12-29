#ifndef BALL_H
#define BALL_H

class Ball{
	public:
		Ball();
		~Ball();
		const static int BOX_WIDTH = 320;//800;
		const static int BOX_HEIGHT = 240;//480;
		float ballRadius;
		float ballX;
		float ballY;
		float ballSpeedX;
		float ballSpeedY;	
		void setLocation();
		void ballHit();

};
#endif
