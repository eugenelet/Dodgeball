#include "W.h"
#include <QtGui/QApplication>
#include <QtGui/QPushButton>
#include <QtGui/QFont>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QTimer>
#include <QImage>
#include <QString>

//L4V2 Lib
#include "camera.h"
#include "image_process.h"
using namespace cv;
using namespace std;
//implement your member function here
#include "ball.h"
#include <pthread.h>
int fd;
camera_buffer camera_buffers;
void *ori_img;
unsigned int img[CAM_WIDTH*CAM_HEIGHT*sizeof(unsigned int)];


static CvMemStorage * storage = 0;
static CvHaarClassifierCascade * cascade = 0;


QImage *IplImageToQImage(IplImage* cvimage);
IplImage *QImageToIplImage( QImage * qImage);
Scalar touch = 0;
Scalar touchUpper = 0;
Scalar touchLower = 0;

QImage& cvxCopyIplImage(const IplImage* pIplImage, QImage &qImage)
{   //implement the conversion function here
        if(!pIplImage) return qImage;

        if(qImage.isNull())
        {
                int w = pIplImage->width;
                int h = pIplImage->height;
                qImage = QImage(w, h, QImage::Format_RGB32);
        }

        int x, y;
        for(x = 0; x < pIplImage->width; ++x)
        {
           for(y = 0; y < pIplImage->height; ++y)
           {
              CvScalar color = cvGet2D(pIplImage, y, x);

              int r = (int)color.val[2];
              int g = (int)color.val[1];
              int b = (int)color.val[0];
	
              qImage.setPixel(x, y, qRgb(r,g,b));
           }
        }
   return qImage;
}
//	ctor
PainterWidget::PainterWidget(QWidget *parent):QWidget(){
	startGame = 0;	
        timer = new QTimer(this);
	detect=0;
	lowerTouch = 0;
	upperTouch = 0;
	passTouch = 0;
	camera_init(&fd,&camera_buffers,1);


	Health_text = new QLabel(this);
	Health_text->setText("Health");
	//Health_text->setGeometry(50,50,150,60);
	Health_text->setGeometry(1000,1000,150,60);
	Health_text->setFont(QFont("Courier",15,QFont::Bold));

	health_number=50;
	Health_number = new QLabel(this);
	Health_number->setNum(health_number);
	//Health_number->setGeometry(50,120,80,60);
	Health_number->setGeometry(1000,1000,80,60);
	Health_number->setFont(QFont("Courier",18,QFont::Bold));

	Score_text = new QLabel(this);
	Score_text->setText("Score");
	//Score_text->setGeometry(650,50,120,60);
	Score_text->setGeometry(1000,1000,120,60);
	Score_text->setFont(QFont("Courier",15,QFont::Bold));



	score_number=0;
	Score_number = new QLabel(this);
	Score_number->setNum(score_number);
	//Score_number->setGeometry(650,120,80,60);
	Score_number->setGeometry(1000,1000,80,60);
	Score_number->setFont(QFont("Courier",18,QFont::Bold));

	Play = new QPushButton(this);
	Play->setText("Play");
	Play->setGeometry(300,180,200,100);
	Play->setFont(QFont("Courier",15,QFont::Bold));
	connect( Play, SIGNAL(clicked()), this, SLOT(setStart()));
//	QObject::connect( Play, SIGNAL(clicked()),this,SLOT( new_window() ) );



	myBall.setLocation();
        
	  touchText= new QLabel(this);
          touchText->setNum(0);
          touchText->setFont(QFont("Courier", 18, QFont::Bold));
          touchText->setGeometry(600,300,110,50);

     	btnDetect= new QPushButton(this);
     	btnDetect->setText("Detect");
     	btnDetect->setFont(QFont("Courier", 18, QFont::Bold));
    	btnDetect->setGeometry(580,100,110,50);
	connect( btnDetect, SIGNAL(clicked()), this, SLOT(setDetect()));
	

	connect(timer, SIGNAL(timeout()), this, SLOT(update()));

	
	//timer->start(30);
}


/*
void PainterWidget::new_window(){



        dialog= new QDialog;

        dialog->setModal(false);

        dialog->show();

}*/

IplImage* img_resize(IplImage* src_img, int new_width,int new_height)
{
    IplImage* des_img;
    des_img=cvCreateImage(cvSize(new_width,new_height),src_img->depth,src_img->nChannels);
    cvResize(src_img,des_img,CV_INTER_LINEAR);
    return des_img;
}




void PainterWidget::paintEvent(QPaintEvent* event){
       QPainter painter(this);

	if(!startGame){
		painter.drawImage(0,0,dodgeball_display);  //start game screen
	}

	else if(startGame ==1){
	timer->start(30);
	Health_text->setGeometry(50,50,150,60);
	Health_number->setGeometry(50,120,80,60);
	Score_text->setGeometry(650,50,120,60);
	Score_number->setGeometry(650,120,80,60);
	Play->setGeometry(1000,1000,200,100);
		startGame = 2;
	        capture(&fd,&camera_buffers,&ori_img,1,0);
       		yuyv2rgb(CAM_WIDTH,CAM_HEIGHT,ori_img,img);
		image= QImage((unsigned char *)img,CAM_WIDTH,CAM_HEIGHT,QImage::Format_RGB32);
		iplimg=QImageToIplImage(&image);	
		painter.drawImage(0,0,process_qImage);  //image after filtering
	}
	else if(startGame ==2 && passTouch){
	        capture(&fd,&camera_buffers,&ori_img,1,0);
       		yuyv2rgb(CAM_WIDTH,CAM_HEIGHT,ori_img,img);
		image= QImage((unsigned char *)img,CAM_WIDTH,CAM_HEIGHT,QImage::Format_RGB32);
		image = image.mirrored(true,false);
		iplimg=QImageToIplImage(&image);
		painter.drawImage(0,0,process_qImage);  //image after filtering
	}
	else{
	        capture(&fd,&camera_buffers,&ori_img,1,0);
       		yuyv2rgb(CAM_WIDTH,CAM_HEIGHT,ori_img,img);
		image= QImage((unsigned char *)img,CAM_WIDTH,CAM_HEIGHT,QImage::Format_RGB32);
		image = image.mirrored(true,false);
		iplimg=QImageToIplImage(&image);	
		painter.drawImage(0,0,image);  //image after filtering
}

//	painter.drawImage(0,0,image);// raw image from camera


}

void PainterWidget::addBall(IplImage *newBall){
	dodgeball = newBall;
	dodgeball = img_resize(dodgeball, 800,720);
	dodgeball_display = cvxCopyIplImage(dodgeball, dodgeball_display);
}


PainterWidget::~PainterWidget(){
	camera_uninit(&fd,&camera_buffers,1);
}

IplImage *QImageToIplImage( QImage * qImage){
	int width = qImage->width();
	int height = qImage->height();
	CvSize Size;
	Size.height = height;
	Size.width = width;
	IplImage *IplImageBuffer = cvCreateImage(Size, IPL_DEPTH_8U, 3);
	for (int y = 0; y < height; ++y){
		for (int x = 0; x < width; ++x){
			QRgb rgb = qImage->pixel(x, y);
			cvSet2D(IplImageBuffer, y, x, CV_RGB(qRed(rgb), qGreen(rgb), qBlue(rgb)));
		}
	}
	return IplImageBuffer;
}

QImage *IplImageToQImage(IplImage* cvimage){
	if (!cvimage)
		return 0;
	QImage* desImage = new QImage(cvimage->width,cvimage->height,QImage::Format_RGB32);
	for(int i = 0; i < cvimage->height; i++){
		for(int j = 0; j < cvimage->width; j++){
			int r,g,b;
		    if(3 == cvimage->nChannels){
				b=(int)CV_IMAGE_ELEM(cvimage,uchar,i,j*3+0);
				g=(int)CV_IMAGE_ELEM(cvimage,uchar,i,j*3+1);
				r=(int)CV_IMAGE_ELEM(cvimage,uchar,i,j*3+2);
			}
			else if(1 == cvimage->nChannels){
				b=(int)CV_IMAGE_ELEM(cvimage,uchar,i,j);
				g=b;
				r=b;
			}
			desImage->setPixel(j,i,qRgb(r,g,b));
		}
	}
	return desImage;
}




void PainterWidget::setStart(){//game starter
	startGame = 1;
	repaint();
}

void PainterWidget::setDetect(){//set background image
	detect = 1;
	Mat tempBackground(iplimg);
	backgroundImage = tempBackground;
}


void PainterWidget::update(){
	if(!detect){ //backgroundImage set so that it's not NULL, Avoids crash
		Mat temp(iplimg);
		backgroundImage = temp;
	}

        IplImage *img;
	
	Mat currentImage(iplimg);
	Mat diffImage;
    	
	absdiff(backgroundImage, currentImage, diffImage);
    	Mat foregroundMask = cv::Mat::zeros(diffImage.rows, diffImage.cols, CV_8UC1);
   	float threshold = 5000.0f;
    	float dist;

    	for(int j=0; j<diffImage.rows; ++j){
        	for(int i=0; i<diffImage.cols; ++i)
        		{
       			     cv::Vec3b pix = diffImage.at<cv::Vec3b>(j,i);

            			dist = (pix[0]*pix[0] + pix[1]*pix[1] + pix[2]*pix[2]);
           			 dist = sqrt(dist);

           		 if(dist>threshold)
           		 {
               			 foregroundMask.at<unsigned char>(j,i) = 255;
            			}
        		}
		}
	//Abs diff, image not fully filtered yet

	//secound filtering using Canny edge detector
	Mat original(iplimg);

   	Mat gray, edge, draw, blur;
   	cvtColor(diffImage, gray, CV_BGR2GRAY);
	GaussianBlur(gray,blur,Size(0,0), 2); //Gaussian blur, remove background
	Canny( blur, edge, 50, 150, 3);
	edge.convertTo(draw, CV_8U);//-------------------------------------------------draw: final output camera
	
	Mat cir = Mat::zeros( CAM_HEIGHT, CAM_WIDTH, CV_8U ); //used for the backbone of the circle
	circle( cir, Point( myBall.ballX, myBall.ballY ), 32.0, Scalar( 255, 255, 255 ), -1, 8 );
	
	Mat intersection = (cir & draw);
	touch  = cv::sum(intersection);
	if(touch[0]){ //ball hits player
		myBall.ballHit();
		health_number--;	
		Health_number->setNum(health_number);
	}
	
	myBall.setLocation();//update ball location
	
	Mat upperRec = Mat::zeros( CAM_HEIGHT, CAM_WIDTH, CV_8U ); //used for the backbone of the circle
	rectangle(upperRec, Point(0,0), Point(CAM_WIDTH, CAM_HEIGHT/4), Scalar( 255, 255, 255 ), -1, 8);
	Mat lowerRec = Mat::zeros( CAM_HEIGHT, CAM_WIDTH, CV_8U ); //used for the backbone of the circle
	rectangle(lowerRec, Point(0,CAM_HEIGHT/4), Point(CAM_WIDTH, CAM_HEIGHT), Scalar( 255, 255, 255 ), -1, 8);

	
	Mat upperRec_intersect = (upperRec & draw);
	touchUpper  = cv::sum(upperRec_intersect);
	if(touchUpper[0]){ //ball hits player
		upperTouch=1;
	}
	else{
		upperTouch = 0;
	}

	Mat lowerRec_intersect = (lowerRec & draw);
	touchLower  = cv::sum(lowerRec_intersect);
	if(touchLower[0]){ //ball hits player
		lowerTouch=1;
	}
	else{
		lowerTouch = 0;
	}

	if(!upperTouch && lowerTouch){
		passTouch = 1;	
	}
	else{
		passTouch = 0;
	}


	Mat combine = Mat::zeros( CAM_HEIGHT, CAM_WIDTH, CV_8U ); // for combining both Camera and ball
	cout<<passTouch<<endl;//"Y:"<<myBall.ballY<<endl<<"X:"<<myBall.ballX<<endl;
	touchText -> setNum(touch[0]);//debugging	
		
	Mat cir2 = Mat::zeros( CAM_HEIGHT, CAM_WIDTH, original.type() ); //front end of ball
	circle( cir2, Point( myBall.ballX, myBall.ballY ), 32.0, Scalar( 255, 5, 155 ), -1, 8 );
	
	combine = cir2 | original; //combine front end of ball and original image
	IplImage copy = combine;
	IplImage* new_image = &copy;
	cvxCopyIplImage (new_image, process_qImage);
	
	timer->start(30);
        repaint();

}



//dog


//dog

/*
void PainterWidget::processImage(IplImage* pIplImage){

        IplImage* processIplImage;

                processIplImage = cvCloneImage (pIplImage);


                CvHaarClassifierCascade* cascade;
                cascade = (CvHaarClassifierCascade*)cvLoad("haarcascade_frontalface_alt.xml", 0, 0, 0 );
                CvMemStorage* storage =cvCreateMemStorage(0);

                CvSeq* faces = cvHaarDetectObjects(     processIplImage,
                                                         cascade,
                                                         storage,
                                                        1.1, 2,
                                                        CV_HAAR_DO_CANNY_PRUNING,
                                                        cvSize(20, 20),cvSize(20, 20) );

                if (faces)
                {
                        for(int k= 0; k< faces->total; k++)
                        {
                                CvPoint pt1, pt2;
                                CvRect* rectangle = (CvRect*)cvGetSeqElem(faces, k);
                                pt1.x = rectangle->x;
                                pt2.x = rectangle->x + rectangle->width;
                                pt1.y = rectangle->y;
                                pt2.y = rectangle->y + rectangle->height;
                        //Draw a red rectanngle
                                cvRectangle( processIplImage, pt1,pt2, CV_RGB(255,0,0), 3, 8, 0 );
                        }
                }

                qImage = cvxCopyIplImage (pIplImage,qImage);
                process_qImage = cvxCopyIplImage (processIplImage, process_qImage);

}

*/

