#include "ball.h"


//	ctor
Ball::Ball(){
//	BOX_WIDTH = 800;
//	BOX_HEIGHT = 480;
	ballRadius = 30;
        ballX = ballRadius + 10;
        ballY = ballRadius + 20;
        ballSpeedX = 15;
        ballSpeedY = 10;
}

Ball::~Ball(){}

void Ball::ballSpeed(){
	ballSpeedX += 5;
	ballSpeedY += 3;
}

void Ball::setLocation(){
	ballX += ballSpeedX;
	ballY += ballSpeedY;

               // Check if the ball moves over the bounds
               // If so, adjust the position and speed.
               if (ballX - ballRadius < 0) {
                  ballSpeedX = -ballSpeedX; // Reflect along normal
                  ballX = ballRadius; // Re-position the ball at the edge
               } else if (ballX + ballRadius > BOX_WIDTH) {
                  ballSpeedX = -ballSpeedX;
                  ballX = BOX_WIDTH - ballRadius;
               }
               // May cross both x and y bounds
               if (ballY - ballRadius < 0) {
                  ballSpeedY = -ballSpeedY;
                  ballY = ballRadius;
               } else if (ballY + ballRadius > BOX_HEIGHT) {
                  ballSpeedY = -ballSpeedY;
                  ballY = BOX_HEIGHT - ballRadius;
               }
}

void Ball::ballHit(){
	ballSpeedX = -ballSpeedX; // Reflect along normal
        ballSpeedY = -ballSpeedY;
}

