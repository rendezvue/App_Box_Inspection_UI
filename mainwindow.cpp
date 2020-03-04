#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //installEventFilter(this); // install filter BEFORE setupUI.
	//qApp->installEventFilter(this);
	//setMouseTracking(true);
	ui->setupUi(this);

	centralWidget()->installEventFilter(this);
	centralWidget()->setMouseTracking(true);

	m_top_point_x = -1 ;
	m_top_point_y = -1 ;

	m_bottom_point_x = -1 ;
	m_bottom_point_y = -1 ;

	m_set_user_level_top = false ;
	m_set_user_level_bottom = false ;
	
    //showMaximized();
    setWindowState(Qt::WindowFullScreen);

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

	//run checked
	connect(m_pEnsemble[0], SIGNAL(RunCheck_Crack(bool)), this, SLOT(updateRunCrack_Top(bool))) ;
	connect(m_pEnsemble[1], SIGNAL(RunCheck_Crack(bool)), this, SLOT(updateRunCrack_Bottom(bool))) ;

	//level
	connect(m_pEnsemble[0], SIGNAL(Level_Crack(int)), this, SLOT(updateLevelCrack_Top(int))) ;
	connect(m_pEnsemble[1], SIGNAL(Level_Crack(int)), this, SLOT(updateLevelCrack_Bottom(int))) ;

	//quality
	connect(m_pEnsemble[0], SIGNAL(signal_Quality_Crack(float)), this, SLOT(updateQualityCrack_Top(float))) ;
	connect(m_pEnsemble[1], SIGNAL(signal_Quality_Crack(float)), this, SLOT(updateQualityCrack_Bottom(float))) ;
	
	//slider
	connect(ui->horizontalSlider_level_top, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetTopLevel()));
	connect(ui->horizontalSlider_level_top, SIGNAL(sliderMoved(int)), this, SLOT(OnSliderTopMove(int)));
	connect(ui->horizontalSlider_level_bottom, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetBottomLevel()));
	connect(ui->horizontalSlider_level_bottom, SIGNAL(sliderMoved(int)), this, SLOT(OnSliderBottomMove(int)));
	
	//button
	connect(ui->pushButton_config, SIGNAL(clicked()), this,  SLOT(OnButton_Config())) ;
	connect(ui->pushButton_config_new, SIGNAL(clicked()), this,  SLOT(OnButton_Config_New())) ;
	connect(ui->pushButton_config_save, SIGNAL(clicked()), this,  SLOT(OnButton_Config_Save())) ;

	connect(ui->pushButton_test_run, SIGNAL(clicked()), this,  SLOT(OnButton_Test_Run())) ;
	

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
        //p_image_label->setPixmap(QPixmap::fromImage(qt_display_image));
			
		//draw guide line(mouse)
		if( m_top_point_x >= 0 && m_top_point_y >= 0 && m_pEnsemble[0]->Get_Status() == STATUS_CONFIG )
	    {
	        QPainter qPainter(&qt_display_image);
	        qPainter.setBrush(Qt::NoBrush);
	        qPainter.setPen(Qt::red);

	        qPainter.drawLine(m_top_point_x,0,m_top_point_x,image.rows);
	        qPainter.drawLine(0,m_top_point_y,image.cols,m_top_point_y);

	        bool bEnd = qPainter.end();
	    }

		//draw select region
		if( m_select_region.type == 0 )	//top
		{
			if( m_select_region.w > 0 && m_select_region.h > 0 )
			{
				QPainter qPainter(&qt_display_image);
				qPainter.setBrush(Qt::NoBrush);
				qPainter.setPen(Qt::blue);
				
				qPainter.drawRect(m_select_region.x,m_select_region.y,m_select_region.w,m_select_region.h);

				bool bEnd = qPainter.end();
			}
		}
		
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

		//draw guide line(mouse)
		if( m_bottom_point_x >= 0 && m_bottom_point_y >= 0 && m_pEnsemble[1]->Get_Status() == STATUS_CONFIG )
	    {
	        QPainter qPainter(&qt_display_image);
	        qPainter.setBrush(Qt::NoBrush);
	        qPainter.setPen(Qt::red);

	        qPainter.drawLine(m_bottom_point_x,0,m_bottom_point_x,image.rows);
	        qPainter.drawLine(0,m_bottom_point_y,image.cols,m_bottom_point_y);

	        bool bEnd = qPainter.end();
	    }

		//draw select region
		if( m_select_region.type == 1 )	//bottom
		{
			if( m_select_region.w > 0 && m_select_region.h > 0 )
			{
				QPainter qPainter(&qt_display_image);
				qPainter.setBrush(Qt::NoBrush);
				qPainter.setPen(Qt::blue);
				
				qPainter.drawRect(m_select_region.x,m_select_region.y,m_select_region.w,m_select_region.h);

				bool bEnd = qPainter.end();
			}
		}
		
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

	m_pEnsemble[0]->Config_Load() ;
	m_pEnsemble[1]->Config_Load() ;
}

#if 0
bool MainWindow::eventFilter(QObject *obj, QEvent* event)
{
	if (event->type() == QEvent::MouseMove)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

		QPoint point = mouseEvent->pos() ;

		int pt_x = point.x() ;
		int pt_y = point.y() ;

		qDebug("event mouse move : %d, %d", pt_x, pt_y) ;

		//top
		int top_region_x = ui->label_image_top->x() ;
		int top_region_y = ui->label_image_top->y() ;

		int top_region_w = ui->label_image_top->width() ;
	    int top_region_h = ui->label_image_top->height() ;

		//bottom		
		int bottom_region_x = ui->label_image_bottom->x() ;
		int bottom_region_y = ui->label_image_bottom->y() ;

		int bottom_region_w = ui->label_image_bottom->width() ;
		int bottom_region_h = ui->label_image_bottom->height() ;

		//qDebug("mouse : %d, %d", pt_x, pt_y) ;
		//qDebug("left : %d, %d, %d, %d", top_region_x, top_region_y, top_region_w, top_region_h) ;
		//qDebug("right : %d, %d, %d, %d", bottom_region_x, bottom_region_y, bottom_region_w, bottom_region_h) ;

		if( pt_x >= top_region_x && pt_x <= top_region_x+top_region_w &&
			pt_y >= top_region_y && pt_y <= top_region_y+top_region_h )  
		{
			//qDebug("mouse top : %d, %d", pt_x, pt_y) ;
			
			m_top_point_x = top_region_x - pt_x ;
			m_top_point_y = top_region_y - pt_y ;

			m_bottom_point_x = -1 ;
			m_bottom_point_y = -1 ;
		}
		else if( pt_x >= bottom_region_x && pt_x <= bottom_region_x+bottom_region_w &&
				pt_y >= bottom_region_y && pt_y <= bottom_region_y+bottom_region_h )  
		{
			//qDebug("mouse bottom : %d, %d", pt_x, pt_y) ;
			
			m_top_point_x = -1 ;
			m_top_point_y = -1 ;

			m_bottom_point_x = bottom_region_x - pt_x ;
			m_bottom_point_y = bottom_region_y - pt_y ;
		}
	}
	
	return false;
}
#endif

#if 1
void MainWindow::mousePressEvent(QMouseEvent *event)
{
	//check region
	QPoint point = event->pos() ;

	int pt_x = point.x() ;
	int pt_y = point.y() ;

	//top
	int top_region_x = ui->label_image_top->x() ;
	int top_region_y = ui->label_image_top->y() ;

	int top_region_w = ui->label_image_top->width() ;
    int top_region_h = ui->label_image_top->height() ;

	//bottom		
	int bottom_region_x = ui->label_image_bottom->x() ;
	int bottom_region_y = ui->label_image_bottom->y() ;

	int bottom_region_w = ui->label_image_bottom->width() ;
	int bottom_region_h = ui->label_image_bottom->height() ;

	if( pt_x >= top_region_x && pt_x <= top_region_x+top_region_w &&
		pt_y >= top_region_y && pt_y <= top_region_y+top_region_h )  
	{
		//top
		if( m_pEnsemble[0]->Get_Status() == STATUS_CONFIG )
		{
			m_select_region.type = 0 ;
			m_select_region.x = pt_x - top_region_x ;
			m_select_region.y = pt_y - top_region_y ;
			m_select_region.w = 1 ;
			m_select_region.h = 1 ;
		}
	}
	else if( pt_x >= bottom_region_x && pt_x <= bottom_region_x+bottom_region_w &&
			pt_y >= bottom_region_y && pt_y <= bottom_region_y+bottom_region_h )  
	{
		//bottom
		if( m_pEnsemble[1]->Get_Status() == STATUS_CONFIG )
		{
			m_select_region.type = 1 ;
			m_select_region.x = pt_x - bottom_region_x ;
			m_select_region.y = pt_y - bottom_region_y ;
			m_select_region.w = 1 ;
			m_select_region.h = 1 ;
		}
	}
	
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{	
	QPoint point = event->pos() ;

	int pt_x = point.x() ;
	int pt_y = point.y() ;

	//qDebug("event mouse move : %d, %d", pt_x, pt_y) ;

	//top
	int top_region_x = ui->label_image_top->x() ;
	int top_region_y = ui->label_image_top->y() ;

	int top_region_w = ui->label_image_top->width() ;
    int top_region_h = ui->label_image_top->height() ;

	//bottom		
	int bottom_region_x = ui->label_image_bottom->x() ;
	int bottom_region_y = ui->label_image_bottom->y() ;

	int bottom_region_w = ui->label_image_bottom->width() ;
	int bottom_region_h = ui->label_image_bottom->height() ;

	//qDebug("mouse : %d, %d", pt_x, pt_y) ;
	//qDebug("left : %d, %d, %d, %d", top_region_x, top_region_y, top_region_w, top_region_h) ;
	//qDebug("right : %d, %d, %d, %d", bottom_region_x, bottom_region_y, bottom_region_w, bottom_region_h) ;

	if( pt_x >= top_region_x && pt_x <= top_region_x+top_region_w &&
		pt_y >= top_region_y && pt_y <= top_region_y+top_region_h )  
	{
		//qDebug("mouse top : %d, %d", pt_x, pt_y) ;
		
		m_top_point_x = pt_x - top_region_x ;
		m_top_point_y = pt_y - top_region_y ;

		m_bottom_point_x = -1 ;
		m_bottom_point_y = -1 ;

		//top
		if( m_pEnsemble[0]->Get_Status() == STATUS_CONFIG && m_select_region.type == 0  )
		{
			m_select_region.w = (pt_x - top_region_x) - m_select_region.x ;
			m_select_region.h = (pt_y - top_region_y) - m_select_region.y ;
		}
	}
	else if( pt_x >= bottom_region_x && pt_x <= bottom_region_x+bottom_region_w &&
			pt_y >= bottom_region_y && pt_y <= bottom_region_y+bottom_region_h )  
	{
		//qDebug("mouse bottom : %d, %d", pt_x, pt_y) ;
		
		m_top_point_x = -1 ;
		m_top_point_y = -1 ;

		m_bottom_point_x = pt_x - bottom_region_x ;
		m_bottom_point_y = pt_y - bottom_region_y ;

		//bottom
		if( m_pEnsemble[1]->Get_Status() == STATUS_CONFIG && m_select_region.type == 1  )
		{
			m_select_region.w = (pt_x - bottom_region_x) - m_select_region.x ;
			m_select_region.h = (pt_y - bottom_region_y) - m_select_region.y ;
		}
	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
	m_top_point_x = -1 ;
	m_top_point_y = -1 ;

	m_bottom_point_x = -1 ;
	m_bottom_point_y = -1 ;

	if( m_select_region.type == 0 )	//top
	{
		if( m_pEnsemble[0]->Get_Status() == STATUS_CONFIG )
		{			
			float f_x = 0.0 ;
			float f_y = 0.0 ;
			float f_w = 0.0 ;
			float f_h = 0.0 ;

			int label_w = ui->label_image_top->width() ;
			int label_h = ui->label_image_top->height() ;

			f_x = (float)m_select_region.x / (float)label_w ;
			f_y = (float)m_select_region.y / (float)label_h ;
			f_w = (float)m_select_region.w / (float)label_w ;
			f_h = (float)m_select_region.h / (float)label_h ;
			
			m_pEnsemble[0]->Config_Set_Region(f_x, f_y, f_w, f_h) ;
		}
	}
	else if( m_select_region.type == 1 )	//bottom
	{
		if( m_pEnsemble[1]->Get_Status() == STATUS_CONFIG )
		{
			float f_x = 0.0 ;
			float f_y = 0.0 ;
			float f_w = 0.0 ;
			float f_h = 0.0 ;

			int label_w = ui->label_image_bottom->width() ;
			int label_h = ui->label_image_bottom->height() ;

			f_x = (float)m_select_region.x / (float)label_w ;
			f_y = (float)m_select_region.y / (float)label_h ;
			f_w = (float)m_select_region.w / (float)label_w ;
			f_h = (float)m_select_region.h / (float)label_h ;
			
			m_pEnsemble[1]->Config_Set_Region(f_x, f_y, f_w, f_h) ;
		}
	}

	//reset
	m_select_region.type = -1 ;
	m_select_region.x = -1 ;
	m_select_region.y = -1 ;
	m_select_region.w = -1 ;
	m_select_region.h = -1 ;
}
#endif

void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev) ;
}

void MainWindow::OnButton_Test_Run(void)
{
	if( m_pEnsemble[0]->Get_Status() == STATUS_TEST_RUN)
	{
		m_pEnsemble[0]->Set_Status(STATUS_NORMAL) ;
		m_pEnsemble[1]->Set_Status(STATUS_NORMAL) ;
		
		ui->pushButton_test_run->setText("Test Run Start");
	}
	else
	{
		m_pEnsemble[0]->Set_Status(STATUS_TEST_RUN) ;
		m_pEnsemble[1]->Set_Status(STATUS_TEST_RUN) ;

		ui->pushButton_test_run->setText("Test Run Stop");
	}
}

void MainWindow::updateRunCrack_Top(bool b_run)
{
	ui->checkBox_check_crack_top->setChecked(b_run);
}

void MainWindow::updateRunCrack_Bottom(bool b_run)
{
	ui->checkBox_check_crack_bottom->setChecked(b_run);
}

void MainWindow::updateLevelCrack_Top(int level)
{
	//qDebug("crack level = %d", level) ;
	
	//Set Slider
	if( m_set_user_level_top == false ) ui->horizontalSlider_level_top->setValue(level) ;
}

void MainWindow::updateLevelCrack_Bottom(int level)
{
	//Set Slider
	if( m_set_user_level_bottom == false ) ui->horizontalSlider_level_bottom->setValue(level) ;
}

void MainWindow::updateQualityCrack_Top(float quality)
{
	ui->progressBar_quality_top->setValue(quality) ;
}

void MainWindow::updateQualityCrack_Bottom(float quality)
{
	ui->progressBar_quality_bottom->setValue(quality) ;
}

void MainWindow::OnSliderSetTopLevel(void)
{
	//get level
    int level = ui->horizontalSlider_level_top->value() ;
	m_pEnsemble[0]->Config_Set_Level(level) ;

	m_set_user_level_top = false ;
}

void MainWindow::OnSliderTopMove(int value) 
{
	//set feature
	m_set_user_level_top = true ;
}

void MainWindow::OnSliderSetBottomLevel(void)
{
	//get level
    int level = ui->horizontalSlider_level_bottom->value() ;
	m_pEnsemble[1]->Config_Set_Level(level) ;

	m_set_user_level_bottom = false ;
}

void MainWindow::OnSliderBottomMove(int value) 
{
	//set feature
	m_set_user_level_bottom = true ;
}

