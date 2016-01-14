#include <QtGui/QApplication>
#include <QtGui/QPushButton>
#include <QtGui/QFont>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QTimer>
#include <QImage>
#include "W.h"

using namespace std;

int main(int argc, char *argv[]){
	IplImage *ball;
	IplImage *dodgeball;
	IplImage *gameover;
	ball = cvLoadImage("ball.jpg");	
	ball = cvLoadImage("ball.jpg");	
	dodgeball = cvLoadImage("dodge.jpg", 1);
	gameover = cvLoadImage("gameover.jpg", 1);
	QApplication app(argc,argv);
	PainterWidget* widget = new PainterWidget();
	widget->addBall(dodgeball, gameover);
	widget->show();

	return app.exec();

}
