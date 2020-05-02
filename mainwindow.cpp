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

	m_pCheckLogSize = new CCheckLogSize(this) ;
	m_pCheckLogSize->m_thread_run = true ;
	m_pCheckLogSize->start() ;
	
	//
	ui->lineEdit_count_run_top->setStyleSheet("color: black") ;
	ui->lineEdit_count_run_bottom->setStyleSheet("color: black") ;

	ui->lineEdit_count_pass_top->setStyleSheet("color: green") ;
	ui->lineEdit_count_pass_bottom->setStyleSheet("color: green") ;

	ui->lineEdit_count_ng_top->setStyleSheet("color: red") ;
	ui->lineEdit_count_ng_bottom->setStyleSheet("color: red") ;

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

	m_pEnsemble = new CEnsemble(this) ;

	connect(m_pEnsemble, SIGNAL(Done(cv::Mat, cv::Mat)), this, SLOT(updatePicture(cv::Mat, cv::Mat))) ;
	//connect(m_pEnsemble, SIGNAL(Done_Top(cv::Mat)), this, SLOT(updatePicture_Top(cv::Mat))) ;
	//connect(m_pEnsemble, SIGNAL(Done_Bottom(cv::Mat)), this, SLOT(updatePicture_Bottom(cv::Mat))) ;

	connect(m_pEnsemble, SIGNAL(UpdateObjectImae(cv::Mat, cv::Mat)), this, SLOT(updateObjectPicture(cv::Mat, cv::Mat))) ;
	//connect(m_pEnsemble, SIGNAL(UpdateObjectImae_Top(cv::Mat)), this, SLOT(updateObjectPicture_Top(cv::Mat))) ;
	//connect(m_pEnsemble, SIGNAL(UpdateObjectImae_Bottom(cv::Mat)), this, SLOT(updateObjectPicture_Bottom(cv::Mat))) ;

	connect(m_pEnsemble, SIGNAL(NetStatus(bool, bool)), this, SLOT(updateNetwork(bool, bool))) ;
	//connect(m_pEnsemble, SIGNAL(NetStatus_Top(bool)), this, SLOT(updateNetwork_Top(bool))) ;
	//connect(m_pEnsemble, SIGNAL(NetStatus_Bottom(bool)), this, SLOT(updateNetwork_Bottom(bool))) ;

	//job info
	connect(m_pEnsemble, SIGNAL(JobInfo(QString, QString)), this, SLOT(updateJobInfo(QString, QString))) ;
	//connect(m_pEnsemble, SIGNAL(JobInfo_Top(QString)), this, SLOT(updateJobInfo_Top(QString))) ;
	//connect(m_pEnsemble, SIGNAL(JobInfo_Bottom(QString)), this, SLOT(updateJobInfo_Bottom(QString))) ;

	//run checked
	connect(m_pEnsemble, SIGNAL(RunCheck_Color(bool, bool)), this, SLOT(updateRunColor(bool, bool))) ;
	//connect(m_pEnsemble, SIGNAL(RunCheck_Color_Top(bool)), this, SLOT(updateRunColor_Top(bool))) ;
	//connect(m_pEnsemble, SIGNAL(RunCheck_Color_Bottom(bool)), this, SLOT(updateRunColor_Bottom(bool))) ;

	//level
	connect(m_pEnsemble, SIGNAL(Level_Color(int, int)), this, SLOT(updateLevelColor(int, int))) ;
	//connect(m_pEnsemble, SIGNAL(Level_Color_Top(int)), this, SLOT(updateLevelColor_Top(int))) ;
	//connect(m_pEnsemble, SIGNAL(Level_Color_Bottom(int)), this, SLOT(updateLevelColor_Bottom(int))) ;

	connect(m_pEnsemble, SIGNAL(Sensitivity_Color(int, int)), this, SLOT(updateSensitivityColor(int, int))) ;
	//connect(m_pEnsemble, SIGNAL(Sensitivity_Color_Top(int)), this, SLOT(updateSensitivityColor_Top(int))) ;
	//connect(m_pEnsemble, SIGNAL(Sensitivity_Color_Bottom(int)), this, SLOT(updateSensitivityColor_Bottom(int))) ;

	//quality
	//color
	connect(m_pEnsemble, SIGNAL(signal_Quality_Color(float, float)), this, SLOT(updateQualityColor(float, float))) ;
	//connect(m_pEnsemble, SIGNAL(signal_Quality_Color_Top(float)), this, SLOT(updateQualityColor_Top(float))) ;
	//connect(m_pEnsemble, SIGNAL(signal_Quality_Color_Bottom(float)), this, SLOT(updateQualityColor_Bottom(float))) ;

	//ccount

	connect(m_pEnsemble, SIGNAL(signal_Count_Run(int, int)), this, SLOT(updateCountRun(int, int))) ;
	//connect(m_pEnsemble, SIGNAL(signal_Count_Run_Top(int)), this, SLOT(updateCountRun_Top(int))) ;
	//connect(m_pEnsemble, SIGNAL(signal_Count_Run_Bottom(int)), this, SLOT(updateCountRun_Bottom(int))) ;
	connect(m_pEnsemble, SIGNAL(signal_Count_Pass(int, int)), this, SLOT(updateCountPass(int, int))) ;
	//connect(m_pEnsemble, SIGNAL(signal_Count_Pass_Top(int)), this, SLOT(updateCountPass_Top(int))) ;
	//connect(m_pEnsemble, SIGNAL(signal_Count_Pass_Bottom(int)), this, SLOT(updateCountPass_Bottom(int))) ;
	connect(m_pEnsemble, SIGNAL(signal_Count_Ng(int, int)), this, SLOT(updateCountNg(int, int))) ;
	//connect(m_pEnsemble, SIGNAL(signal_Count_Ng_Top(int)), this, SLOT(updateCountNg_Top(int))) ;
	//connect(m_pEnsemble, SIGNAL(signal_Count_Ng_Bottom(int)), this, SLOT(updateCountNg_Bottom(int))) ;
	//connect(m_pEnsemble, SIGNAL(signal_Count_Ng_Crack(int, int)), this, SLOT(updateCountNgCrack(int, int))) ;
	//connect(m_pEnsemble, SIGNAL(signal_Count_Ng_Crack_Top(int)), this, SLOT(updateCountNgCrack_Top(int))) ;
	//connect(m_pEnsemble, SIGNAL(signal_Count_Ng_Crack_Bottom(int)), this, SLOT(updateCountNgCrack_Bottom(int))) ;
	//connect(m_pEnsemble, SIGNAL(signal_Count_Ng_Color(int, int)), this, SLOT(updateCountNgColor(int, int))) ;
	//connect(m_pEnsemble, SIGNAL(signal_Count_Ng_Color_Top(int)), this, SLOT(updateCountNgColor_Top(int))) ;
	//connect(m_pEnsemble, SIGNAL(signal_Count_Ng_Color_Bottom(int)), this, SLOT(updateCountNgColor_Bottom(int))) ;
	
	
	//slider
	//color compare : sensitivity
	connect(ui->horizontalSlider_color_sensitivity_top, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetTopSensitivity_Color()));
	connect(ui->horizontalSlider_color_sensitivity_top, SIGNAL(sliderMoved(int)), this, SLOT(OnSliderTopSensitivityMove_Color(int)));
	connect(ui->horizontalSlider_color_sensitivity_bottom, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetBottomSensitivity_Color()));
	connect(ui->horizontalSlider_color_sensitivity_bottom, SIGNAL(sliderMoved(int)), this, SLOT(OnSliderBottomSensitivityMove_Color(int)));
	//color compare : check level	
	connect(ui->horizontalSlider_color_level_top, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetTopLevel_Color()));
	connect(ui->horizontalSlider_color_level_top, SIGNAL(sliderMoved(int)), this, SLOT(OnSliderTopLevelMove_Color(int)));
	connect(ui->horizontalSlider_color_level_bottom, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetBottomLevel_Color()));
	connect(ui->horizontalSlider_color_level_bottom, SIGNAL(sliderMoved(int)), this, SLOT(OnSliderBottomLevelMove_Color(int)));
	
	//button
	connect(ui->pushButton_config, SIGNAL(clicked()), this,  SLOT(OnButton_Config())) ;
	connect(ui->pushButton_config_new, SIGNAL(clicked()), this,  SLOT(OnButton_Config_New())) ;
	connect(ui->pushButton_config_save, SIGNAL(clicked()), this,  SLOT(OnButton_Config_Save())) ;
	connect(ui->pushButton_config_load, SIGNAL(clicked()), this,  SLOT(OnButton_Config_Load())) ;

	connect(ui->pushButton_test_run, SIGNAL(clicked()), this,  SLOT(OnButton_Test_Run())) ;
	connect(ui->pushButton_test_open_log, SIGNAL(clicked()), this,  SLOT(OnButton_Open_Log())) ;
	

	//check box
    connect(ui->checkBox_check_color_top, SIGNAL(clicked(bool)), this, SLOT(OnCheckbox_Toggle_Color_Top(bool)));
    connect(ui->checkBox_check_color_bottom, SIGNAL(clicked(bool)), this, SLOT(OnCheckbox_Toggle_Color_Bottom(bool)));
	
	ui->pushButton_config_new->hide() ;
	ui->pushButton_config_save->hide() ;
	ui->pushButton_config_load->hide() ;
		
	
	//m_pEnsemble[0]->SetIP("192.168.56.102") ;
	//m_pEnsemble->SetIP("192.168.56.102") ;

	//m_pEnsemble[0]->SetPort(4000) ;
	//m_pEnsemble->SetPort(4001) ;

	std::string top_ip = m_cls_info.Get_Top_Ip_Addr() ;
	int top_port = m_cls_info.Get_Top_Port_Num() ;

	std::string bottom_ip = m_cls_info.Get_Bottom_Ip_Addr() ;
	int bottom_port = m_cls_info.Get_Bottom_Port_Num() ;
	
	m_cls_info.Save(top_ip, top_port, bottom_ip, bottom_port) ;

	m_pEnsemble->SetNetwork(top_ip, top_port, bottom_ip, bottom_port) ;
	
	m_pEnsemble->m_thread_run = true ;
	
	m_pEnsemble->start() ;

	//display
	QString qstr_top = QString::fromStdString(top_ip) + ":" + QString::number(top_port);
	QString qstr_bottom = QString::fromStdString(bottom_ip) + ":" + QString::number(bottom_port);
	
	ui->label_info_top->setText(qstr_top) ;
	ui->label_info_bottom->setText(qstr_bottom) ;

	//Thread Start
	
	
}

MainWindow::~MainWindow()
{
	m_pEnsemble->m_thread_run = false ;

	delete m_pEnsemble ;
	m_pEnsemble = NULL ;

	if( m_pCheckLogSize ) 
	{
		delete m_pCheckLogSize ;
		m_pCheckLogSize = NULL ;
	}
	
    delete ui;
}

void MainWindow::updatePicture(cv::Mat image_top, cv::Mat image_bottom)
{
	cv::Mat top, bottom ;
	if( !image_top.empty() ) image_top.copyTo(top) ;
	if( !image_bottom.empty() ) image_bottom.copyTo(bottom) ;
		
	updatePicture_Top(top) ;
	updatePicture_Bottom(bottom) ;
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
		if( m_top_point_x >= 0 && m_top_point_y >= 0 && m_pEnsemble->Get_Status() == STATUS_CONFIG )
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
		if( m_bottom_point_x >= 0 && m_bottom_point_y >= 0 && m_pEnsemble->Get_Status() == STATUS_CONFIG )
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

void MainWindow::updateObjectPicture(cv::Mat image_top, cv::Mat image_bottom)
{
	updateObjectPicture_Top(image_top) ;
	updateObjectPicture_Bottom(image_bottom) ;
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

void MainWindow::updateNetwork(bool b_con_top, bool b_con_bottom)
{
	updateNetwork_Top(b_con_top) ;
	updateNetwork_Bottom(b_con_bottom) ;
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

void MainWindow::updateJobInfo(QString qstr_info_top, QString qstr_info_bottom)
{
	updateJobInfo_Top(qstr_info_top) ;
	updateJobInfo_Bottom(qstr_info_bottom) ;
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
	int status = m_pEnsemble->Get_Status() ;

	if( status == STATUS_CONFIG )	status = STATUS_NORMAL ;
	else							status = STATUS_CONFIG ;
	
	m_pEnsemble->Set_Status(status) ;

	status = m_pEnsemble->Get_Status() ;
	
	if( status == STATUS_TEST_RUN)
	{
		ui->pushButton_test_run->setText("Run Stop");		
	}
	else
	{
		ui->pushButton_test_run->setText("Run Start");
	}
	
    //button text
    if( status == STATUS_CONFIG )
	{
		ui->pushButton_config->setText("Done");

		ui->pushButton_config_new->show() ;
		ui->pushButton_config_save->show() ;
		ui->pushButton_config_load->show() ;
	}
	else
	{
		ui->pushButton_config->setText("Config");

		ui->pushButton_config_new->hide() ;
		ui->pushButton_config_save->hide() ;
		ui->pushButton_config_load->hide() ;
	}

}

void MainWindow::OnButton_Config_New(void)
{
	m_pEnsemble->Config_New() ;
}

void MainWindow::OnButton_Config_Save(void)
{
	m_pEnsemble->Config_Save() ;

	m_pEnsemble->Config_Load() ;
}

void MainWindow::OnButton_Config_Load(void)
{
	m_pEnsemble->Config_Load() ;
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
		if( m_pEnsemble->Get_Status() == STATUS_CONFIG )
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
		if( m_pEnsemble->Get_Status() == STATUS_CONFIG )
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
		if( m_pEnsemble->Get_Status() == STATUS_CONFIG && m_select_region.type == 0  )
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
		if( m_pEnsemble->Get_Status() == STATUS_CONFIG && m_select_region.type == 1  )
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
		if( m_pEnsemble->Get_Status() == STATUS_CONFIG )
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
			
			m_pEnsemble->Config_Set_Region(TOP, f_x, f_y, f_w, f_h) ;
		}
	}
	else if( m_select_region.type == 1 )	//bottom
	{
		if( m_pEnsemble->Get_Status() == STATUS_CONFIG )
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
			
			m_pEnsemble->Config_Set_Region(BOTTOM, f_x, f_y, f_w, f_h) ;
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
	if( m_pEnsemble->Get_Status() == STATUS_TEST_RUN)
	{
		m_pEnsemble->Set_Status(STATUS_NORMAL) ;
		
		ui->pushButton_test_run->setText("Run Start");
	}
	else
	{
		m_pEnsemble->Set_Status(STATUS_TEST_RUN) ;

		ui->pushButton_test_run->setText("Run Stop");
	}

	const int status = m_pEnsemble->Get_Status() ;

    //button text
    if( status == STATUS_CONFIG )
	{
		ui->pushButton_config->setText("Done");

		ui->pushButton_config_new->show() ;
		ui->pushButton_config_save->show() ;
		ui->pushButton_config_load->show() ;
	}
	else
	{
		ui->pushButton_config->setText("Config");

		ui->pushButton_config_new->hide() ;
		ui->pushButton_config_save->hide() ;
		ui->pushButton_config_load->hide() ;
	}
}

void MainWindow::OnButton_Open_Log(void)
{
	QDesktopServices::openUrl(QUrl(LOG_BASE_FOLDER, QUrl::TolerantMode));
}

void MainWindow::updateRunColor(bool b_run_top, bool b_run_bottom)
{
	updateRunColor_Top(b_run_top) ;
	updateRunColor_Bottom(b_run_bottom) ;
}

void MainWindow::updateRunColor_Top(bool b_run)
{
	ui->checkBox_check_color_top->setChecked(b_run);
}

void MainWindow::updateRunColor_Bottom(bool b_run)
{
	ui->checkBox_check_color_bottom->setChecked(b_run);
}

void MainWindow::updateLevelColor(int level_top, int level_bottom)
{
	updateLevelColor_Top(level_top) ;
	updateLevelColor_Bottom(level_bottom) ;
}

void MainWindow::updateLevelColor_Top(int level)
{
	//qDebug("crack level = %d", level) ;
	
	//Set Slider
	if( m_set_user_level_top == false ) ui->horizontalSlider_color_level_top->setValue(level) ;
}

void MainWindow::updateLevelColor_Bottom(int level)
{
	//Set Slider
	if( m_set_user_level_bottom == false ) ui->horizontalSlider_color_level_bottom->setValue(level) ;
}

void MainWindow::updateSensitivityColor(int level_top, int level_bottom)
{
	updateSensitivityColor_Top(level_top) ;
	updateSensitivityColor_Bottom(level_bottom) ;
}

void MainWindow::updateSensitivityColor_Top(int level)
{
	//Set Slider
	if( m_set_user_level_top == false ) ui->horizontalSlider_color_sensitivity_top->setValue(level) ;
}

void MainWindow::updateSensitivityColor_Bottom(int level)
{
	//Set Slider
	if( m_set_user_level_bottom == false ) ui->horizontalSlider_color_sensitivity_bottom->setValue(level) ;
}

void MainWindow::updateQualityColor(float quality_top, float quality_bottom) 
{
	updateQualityColor_Top(quality_top) ;
	updateQualityColor_Bottom(quality_bottom) ;
}

void MainWindow::updateQualityColor_Top(float quality)
{
	ui->progressBar_color_quality_top->setValue(quality) ;

	const int level = ui->horizontalSlider_color_level_top->value() ;
	UI_UpdateProgressbarColorStatus(ui->progressBar_color_quality_top, quality, level) ;
}

void MainWindow::updateQualityColor_Bottom(float quality)
{
	ui->progressBar_color_quality_bottom->setValue(quality) ;

	const int level = ui->horizontalSlider_color_level_bottom->value() ;
	UI_UpdateProgressbarColorStatus(ui->progressBar_color_quality_bottom, quality, level) ;
}

void MainWindow::UI_UpdateProgressbarColorStatus(QProgressBar *pbar, const float quality, const float level)
{
	QString danger = "QProgressBar{text-align: center; color: rgb(10,10,10);} QProgressBar::chunk {background-color: red;}";
	QString safe= "QProgressBar{text-align: center; color: rgb(10,10,10);} QProgressBar::chunk {background-color: green;}";

	if(quality < level)
	    pbar->setStyleSheet(danger);
	else
	    pbar->setStyleSheet(safe);
}

void MainWindow::updateCountRun(int count_top, int count_bottom)
{
	updateCountRun_Top(count_top) ;
	updateCountRun_Bottom(count_bottom) ;
}
	
void MainWindow::updateCountRun_Top(int count)
{
	ui->lineEdit_count_run_top->setText(QString::number(count)) ;
}

void MainWindow::updateCountRun_Bottom(int count)
{
	ui->lineEdit_count_run_bottom->setText(QString::number(count)) ;
}

void MainWindow::updateCountPass(int count_top, int count_bottom)
{
	updateCountPass_Top(count_top) ;
	updateCountPass_Bottom(count_bottom) ;
}

void MainWindow::updateCountPass_Top(int count)
{
	ui->lineEdit_count_pass_top->setText(QString::number(count)) ;
}

void MainWindow::updateCountPass_Bottom(int count)
{
	ui->lineEdit_count_pass_bottom->setText(QString::number(count)) ;
}

void MainWindow::updateCountNg(int count_top, int count_bottom)
{
	updateCountNg_Top(count_top) ;
	updateCountNg_Bottom(count_bottom) ;
}

void MainWindow::updateCountNg_Top(int count)
{
	ui->lineEdit_count_ng_top->setText(QString::number(count)) ;
}

void MainWindow::updateCountNg_Bottom(int count)
{
	ui->lineEdit_count_ng_bottom->setText(QString::number(count)) ;
}

void MainWindow::OnSliderSetTopSensitivity_Color(void)
{
	//get level
    int level = ui->horizontalSlider_color_sensitivity_top->value() ;
	m_pEnsemble->Config_Set_ColorCompare_Sensitivity(TOP, level) ;

	m_set_user_level_top = false ;
}

void MainWindow::OnSliderTopSensitivityMove_Color(int value)
{
	//set feature
	m_set_user_level_top = true ;
}

void MainWindow::OnSliderSetBottomSensitivity_Color(void)
{
	//get level
    int level = ui->horizontalSlider_color_sensitivity_bottom->value() ;
	m_pEnsemble->Config_Set_ColorCompare_Sensitivity(BOTTOM, level) ;

	m_set_user_level_bottom = false ;
}

void MainWindow::OnSliderBottomSensitivityMove_Color(int value)
{
	//set feature
	m_set_user_level_bottom = true ;
}

//slider Color : Check Level
void MainWindow::OnSliderSetTopLevel_Color(void)
{
	//get level
    int level = ui->horizontalSlider_color_level_top->value() ;
	m_pEnsemble->Config_Set_Level_ColorCompare(TOP, level) ;

	m_set_user_level_top = false ;
}

void MainWindow::OnSliderTopLevelMove_Color(int value)
{
	m_set_user_level_top = true ;
}

//slider Color : Check Level
void MainWindow::OnSliderSetBottomLevel_Color(void) 
{
	//get level
    int level = ui->horizontalSlider_color_level_bottom->value() ;
	m_pEnsemble->Config_Set_Level_ColorCompare(BOTTOM, level) ;

	m_set_user_level_bottom = false ;
}

void MainWindow::OnSliderBottomLevelMove_Color(int value)
{
	m_set_user_level_bottom = true ;
}

void MainWindow::OnCheckbox_Toggle_Color_Top(bool b_check)
{
	m_pEnsemble->Run_Enable_Color(TOP, b_check) ;
}

void MainWindow::OnCheckbox_Toggle_Color_Bottom(bool b_check)
{
	m_pEnsemble->Run_Enable_Color(BOTTOM, b_check) ;
}

