#ifndef W_H
#define W_H
#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QTimer>
#include <QPushButton>
#include <stdio.h>
#include <QtGui/QLabel>

#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <iostream>
#include <string>

#include "ball.h"
using namespace cv;
using namespace std;
		 
class PainterWidget: public QWidget{
	Q_OBJECT
		 protected:
	     public:
		 //member functions
		 	PainterWidget(QWidget *parent = 0);
			~PainterWidget();
		 	void paintEvent(QPaintEvent*); ///gene
			void setFaceDetect(); //gene
			void addBall(IplImage*, IplImage*);	
			void processImage(IplImage*);//dog
			Ball myBall;		
			IplImage* ball;	



		 public slots:
		 //public slots
                        void setDetect();
			void update();
			void setStart();
			void setReset();
                        //void new_window(); 
		private:
		//data members
			QPushButton *btnDetect; //gene
			QImage image; //gene
			int detect; //gene
			int game_over;
                        IplImage* iplimg; //gene
                        QTimer *timer; //gene			
			QImage qImage;	//dog
			QImage process_qImage;	//dog
			QLabel *touchText;
			Mat backgroundImage;

			QPushButton* Reset;

			QPushButton* Play;

                        QLabel* Health_text;

                        QLabel* Health_number;

                        QLabel* Score_text;

                        QLabel* Score_number;
			
			QLabel* Out_of_zone_warning;			

                        int health_number;

                        int score_number;
                        

			int startGame;

			IplImage* dodgeball;
			IplImage* game_over_img
;
			QImage dodgeball_display;	
			QImage gameover_display;	

			int upperTouch;
			int lowerTouch;
			int passTouch;
//QDialog *dialog;
//			Mat src1;

};

#endif
