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

	ui->label_image_top_object_bg->setStyleSheet("QLabel { background-color : black; }");
    ui->label_image_bottom_object_bg->setStyleSheet("QLabel { background-color : black; }");

	m_pEnsemble[0] = new CEnsemble(this) ;
	m_pEnsemble[1] = new CEnsemble(this) ;

	connect(m_pEnsemble[0], SIGNAL(Done(cv::Mat)), this, SLOT(updatePicture_Top(cv::Mat))) ;
	connect(m_pEnsemble[1], SIGNAL(Done(cv::Mat)), this, SLOT(updatePicture_Bottom(cv::Mat))) ;

	connect(m_pEnsemble[0], SIGNAL(UpdateObjectImae(cv::Mat)), this, SLOT(updateObjectPicture_Top(cv::Mat))) ;
	connect(m_pEnsemble[1], SIGNAL(UpdateObjectImae(cv::Mat)), this, SLOT(updateObjectPicture_Bottom(cv::Mat))) ;
	
	connect(m_pEnsemble[0], SIGNAL(NetStatus(bool)), this, SLOT(updateNetwork_Top(bool))) ;
	connect(m_pEnsemble[1], SIGNAL(NetStatus(bool)), this, SLOT(updateNetwork_Bottom(bool))) ;

	//job info
	connect(m_pEnsemble[0], SIGNAL(JobInfo(QString)), this, SLOT(updateJobInfo_Top(QString))) ;
	connect(m_pEnsemble[1], SIGNAL(JobInfo(QString)), this, SLOT(updateJobInfo_Bottom(QString))) ;
	
	//button
	connect(ui->pushButton_config, SIGNAL(clicked()), this,  SLOT(OnButton_Config())) ;
	connect(ui->pushButton_config_new, SIGNAL(clicked()), this,  SLOT(OnButton_Config_New())) ;
	connect(ui->pushButton_config_save, SIGNAL(clicked()), this,  SLOT(OnButton_Config_Save())) ;

	ui->pushButton_config_new->hide() ;
	ui->pushButton_config_save->hide() ;
		
	
	//m_pEnsemble[0]->SetIP("192.168.56.102") ;
	//m_pEnsemble[1]->SetIP("192.168.56.102") ;

	//m_pEnsemble[0]->SetPort(4000) ;
	//m_pEnsemble[1]->SetPort(4001) ;

	std::string top_ip = m_cls_info.Get_Top_Ip_Addr() ;
	int top_port = m_cls_info.Get_Top_Port_Num() ;

	std::string bottom_ip = m_cls_info.Get_Bottom_Ip_Addr() ;
	int bottom_port = m_cls_info.Get_Bottom_Port_Num() ;
	
	m_cls_info.Save(top_ip, top_port, bottom_ip, bottom_port) ;

	m_pEnsemble[0]->SetIP(top_ip) ;
	m_pEnsemble[1]->SetIP(bottom_ip) ;

	m_pEnsemble[0]->SetPort(top_port) ;
	m_pEnsemble[1]->SetPort(bottom_port) ;
	
	m_pEnsemble[0]->m_thread_run = true ;
	m_pEnsemble[1]->m_thread_run = true ;
	
	m_pEnsemble[0]->start() ;
	m_pEnsemble[1]->start() ;

	//display
	QString qstr_top = QString::fromStdString(top_ip) + ":" + QString::number(top_port);
	QString qstr_bottom = QString::fromStdString(bottom_ip) + ":" + QString::number(bottom_port);
	
	ui->label_info_top->setText(qstr_top) ;
	ui->label_info_bottom->setText(qstr_bottom) ;
	
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
	if( image.empty() ) return ;
	
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
	if( image.empty() ) return ;
	
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

void MainWindow::updateObjectPicture_Top(cv::Mat image)
{
	if( image.empty() ) return ;
	
	QLabel *p_image_label_bg = ui->label_image_top_object_bg ;
    QLabel *p_image_label = ui->label_image_top_object ;

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

void MainWindow::updateObjectPicture_Bottom(cv::Mat image)
{
	if( image.empty() ) return ;

	QLabel *p_image_label_bg = ui->label_image_bottom_object_bg ;
    QLabel *p_image_label = ui->label_image_bottom_object ;

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

void MainWindow::updateNetwork_Top(bool b_con)
{
	if( b_con )
	{
		ui->label_info_top->setStyleSheet("QLabel { color : green; }");
	}
	else
	{
		ui->label_info_top->setStyleSheet("QLabel { color : red; }");
	}
}

void MainWindow::updateNetwork_Bottom(bool b_con)
{
	if( b_con )
	{
		ui->label_info_bottom->setStyleSheet("QLabel { color : green; }");
	}
	else
	{
		ui->label_info_bottom->setStyleSheet("QLabel { color : red; }");
	}
}

void MainWindow::updateJobInfo_Top(QString qstr_info)
{
	ui->label_job_info_top->setText(qstr_info) ;
}

void MainWindow::updateJobInfo_Bottom(QString qstr_info)
{
	ui->label_job_info_bottom->setText(qstr_info) ;
}

void MainWindow::OnButton_Config(void)
{
	int status = m_pEnsemble[0]->Get_Status() ;

	if( status == STATUS_CONFIG )	status = STATUS_NORMAL ;
	else							status = STATUS_CONFIG ;
	
	m_pEnsemble[0]->Set_Status(status) ;
	m_pEnsemble[1]->Set_Status(status) ;

    //button text
    if( status == STATUS_CONFIG )
	{
		ui->pushButton_config->setText("Done");

		ui->pushButton_config_new->show() ;
		ui->pushButton_config_save->show() ;
	}
	else
	{
		ui->pushButton_config->setText("Config");

		ui->pushButton_config_new->hide() ;
		ui->pushButton_config_save->hide() ;
	}

}

void MainWindow::OnButton_Config_New(void)
{
	m_pEnsemble[0]->Config_New() ;
	m_pEnsemble[1]->Config_New() ;
}

void MainWindow::OnButton_Config_Save(void)
{
	m_pEnsemble[0]->Config_Save() ;
	m_pEnsemble[1]->Config_Save() ;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
}


