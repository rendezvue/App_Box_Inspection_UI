#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //showMaximized();
    setWindowState(Qt::WindowFullScreen);

    ui->setupUi(this);

	ui->label_image_top_bg->setStyleSheet("QLabel { background-color : black; }");
    ui->label_image_bottom_bg->setStyleSheet("QLabel { background-color : black; }");

	m_pEnsemble[0] = new CEnsemble(this) ;
	m_pEnsemble[1] = new CEnsemble(this) ;

	connect(m_pEnsemble[0], SIGNAL(Done(cv::Mat)), this, SLOT(updatePicture_Top(cv::Mat))) ;
	connect(m_pEnsemble[1], SIGNAL(Done(cv::Mat)), this, SLOT(updatePicture_Bottom(cv::Mat))) ;
	

	m_pEnsemble[0]->SetIP("192.168.56.102") ;
	m_pEnsemble[1]->SetIP("192.168.56.102") ;

	m_pEnsemble[0]->SetPort(4000) ;
	m_pEnsemble[1]->SetPort(4001) ;
	
	m_pEnsemble[0]->m_thread_run = true ;
	m_pEnsemble[1]->m_thread_run = true ;
	
	m_pEnsemble[0]->start() ;
	m_pEnsemble[1]->start() ;
}

MainWindow::~MainWindow()
{
	m_pEnsemble[0]->m_thread_run = false ;
	m_pEnsemble[1]->m_thread_run = false ;

	delete m_pEnsemble[0] ;
	delete m_pEnsemble[1] ;

	m_pEnsemble[0] = NULL ;
	m_pEnsemble[1] = NULL ;
	
    delete ui;
}

void MainWindow::updatePicture_Top(cv::Mat image)
{
    QLabel *p_image_label_bg = ui->label_image_top_bg ;
    QLabel *p_image_label = ui->label_image_top ;

    if( p_image_label && p_image_label_bg )
    {
        const int draw_width = p_image_label_bg->width();
        const int draw_height = p_image_label_bg->height();

        float rescale_w = (float)draw_width / (float)image.cols ;
        float rescale_h = (float)draw_height / (float)image.rows ;

        float min_rescale = std::fmin(rescale_w, rescale_h) ;
        //if( min_rescale < 1.0 )
        {
            cv::resize(image, image, cv::Size(), min_rescale, min_rescale) ;
        }

        //fit image label by image isze
        int pos_x = (int)((float)p_image_label_bg->x() + (float)(draw_width - image.cols)/2.0) ;
        int pos_y = (int)((float)p_image_label_bg->y() + (float)(draw_height - image.rows)/2.0) ;

        p_image_label->setGeometry(pos_x, pos_y, image.cols, image.rows);

		CMat2QImage cls_mat_2_qimage ;
		QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(image, p_image_label->width(), p_image_label->height()) ;
        p_image_label->setPixmap(QPixmap::fromImage(qt_display_image));
    }
}


void MainWindow::updatePicture_Bottom(cv::Mat image)
{
    QLabel *p_image_label_bg = ui->label_image_bottom_bg ;
    QLabel *p_image_label = ui->label_image_bottom ;

    if( p_image_label && p_image_label_bg )
    {
        const int draw_width = p_image_label_bg->width();
        const int draw_height = p_image_label_bg->height();

        float rescale_w = (float)draw_width / (float)image.cols ;
        float rescale_h = (float)draw_height / (float)image.rows ;

        float min_rescale = std::fmin(rescale_w, rescale_h) ;
        //if( min_rescale < 1.0 )
        {
            cv::resize(image, image, cv::Size(), min_rescale, min_rescale) ;
        }

        //fit image label by image isze
        int pos_x = (int)((float)p_image_label_bg->x() + (float)(draw_width - image.cols)/2.0) ;
        int pos_y = (int)((float)p_image_label_bg->y() + (float)(draw_height - image.rows)/2.0) ;

        p_image_label->setGeometry(pos_x, pos_y, image.cols, image.rows);

		CMat2QImage cls_mat_2_qimage ;
		QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(image, p_image_label->width(), p_image_label->height()) ;
        p_image_label->setPixmap(QPixmap::fromImage(qt_display_image));
    }
}

