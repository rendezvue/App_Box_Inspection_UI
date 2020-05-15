#include "censemble.h"

void CEnsemble::run(void)
{	
#if 1
    qRegisterMetaType<cv::Mat>("cv::Mat");

	//Connect
	m_cls_api[TOP].Ensemble_Network_Connect(m_str_ip[TOP].c_str(), m_port[TOP]) ;
	m_cls_api[BOTTOM].Ensemble_Network_Connect(m_str_ip[BOTTOM].c_str(), m_port[BOTTOM]) ;

	//get source list
	m_vec_test_source_list[TOP] = Get_Source_List(TOP) ;
	m_vec_test_source_list[BOTTOM] = Get_Source_List(BOTTOM) ;

	cv::Mat job_image[FACE_MAX_COUNT] ;
	bool network_status[FACE_MAX_COUNT] = {false,} ;
	std::string str_job_info[FACE_MAX_COUNT] ;
	int run_option_crack[FACE_MAX_COUNT] = {0,} ;
	int run_option_color[FACE_MAX_COUNT] = {0,} ;
	int inspect_level_crack[FACE_MAX_COUNT] = {0,} ;
	int inspect_level_color[FACE_MAX_COUNT] = {0,} ;
	int sensitivity_level_color[FACE_MAX_COUNT] = {0,} ;
	float result_crack_quality[FACE_MAX_COUNT] = {0.0, } ;
	float result_color_quality[FACE_MAX_COUNT] = {0.0, } ;
	std::string str_result_xml[FACE_MAX_COUNT] ;
	int result_crack_pass[FACE_MAX_COUNT] = {-1,} ;
	int result_color_pass[FACE_MAX_COUNT] = {-1,} ;

    Start_Capture_Top = false;
    boost::thread* thr = new boost::thread(boost::bind(&CEnsemble::Thread_Capture_SW_Trigger, this));

    while(m_thread_run)
    {       
    	const int status = Get_Status() ;

    	///////////////////////////////////////////////////////////////////////////////////////////////////
    	//Check Network
    	//
    	//qDebug("RUN : 1 : Check Network") ;
		for( int nFace=0 ; nFace<FACE_MAX_COUNT ; nFace++ )
		{
			if( !m_str_ip[nFace].empty() )
	        {
            	if( !m_cls_api[nFace].Ensemble_Network_IsOnline() )
            	{
	                //qDebug("Try Re-Connect = %s:%d (%d)", m_str_ip[nFace].c_str(), m_port[nFace], nFace) ;
	                //try re-connect
	                m_cls_api[nFace].Ensemble_Network_Disconnect() ;
	                m_cls_api[nFace].Ensemble_Network_Connect(m_str_ip[nFace].c_str(), m_port[nFace]) ;

					//get source list
					m_vec_test_source_list[nFace].clear() ;
					m_vec_test_source_list[nFace] = Get_Source_List(nFace) ;

					network_status[nFace] = false ;
            	}
				else
				{
					network_status[nFace] = true ;
				}
			}
		}
		//network
        emit NetStatus(network_status[TOP], network_status[BOTTOM]) ;
		//
    	//Check Network
    	///////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////
		//Get Information
		//
		//qDebug("RUN : 2 : Get Base Information") ;
		for( int nFace=0 ; nFace<FACE_MAX_COUNT ; nFace++ )
		{
			str_job_info[nFace] = Get_Job_Info(nFace, &m_str_job_id[nFace], &m_str_option_inspect_crack_id[nFace], &m_str_option_inspect_color_id[nFace]) ;

			if( str_job_info[nFace].empty() )
			{
				//load DB
            	Config_Load(nFace) ;
			}

			//Get Object Image
            job_image[nFace] = Get_Job_Image(nFace, m_str_job_id[nFace]) ;
			//if( nFace == TOP )			emit UpdateObjectImae_Top(job_image) ;
			//else if( nFace == BOTTOM )	emit UpdateObjectImae_Bottom(job_image) ;
			
			//check run for crack
			//run_option_crack[nFace] = m_cls_api[nFace].Ensemble_Task_Get_Run_Option(m_str_option_inspect_crack_id[nFace]) ;

			//check run for color
			run_option_color[nFace] = m_cls_api[nFace].Ensemble_Task_Get_Run_Option(m_str_option_inspect_color_id[nFace]) ;

			//get inspect level
			//Get Level 
			//inspect_level_crack[nFace] = m_cls_api[nFace].Ensemble_Tool_Option_Crack_Get_InspectLevel(m_str_option_inspect_crack_id[nFace]);

			////qDebug("m_str_option_inspect_color_id = %s", m_str_option_inspect_color_id.c_str()) ;
			inspect_level_color[nFace] = m_cls_api[nFace].Ensemble_Tool_Option_ColorCompare_Get_InspectLevel(m_str_option_inspect_color_id[nFace]);

			//Sensitivity level
			sensitivity_level_color[nFace] = m_cls_api[nFace].Ensemble_Tool_Option_ColorCompare_Get_Sensitivity(m_str_option_inspect_color_id[nFace]);
		}	

		//object image
		emit UpdateObjectImae(job_image[TOP], job_image[BOTTOM]) ; 
		//Job Info
		QString qstr_info_top = QString::fromStdString(str_job_info[TOP]);
		QString qstr_info_bottom = QString::fromStdString(str_job_info[BOTTOM]);
		emit JobInfo(qstr_info_top, qstr_info_bottom) ;
		//run crack		
		//emit RunCheck_Crack((bool)run_option_crack[TOP], (bool)run_option_crack[BOTTOM]);
		//run color
		emit RunCheck_Color((bool)run_option_color[TOP], (bool)run_option_color[BOTTOM]);
		//crack level
		//emit Level_Crack(inspect_level_crack[TOP], inspect_level_crack[BOTTOM]) ;
		//color level
		emit Level_Color(inspect_level_color[TOP], inspect_level_color[BOTTOM]) ;
		//color sensitivity level
		emit Sensitivity_Color(sensitivity_level_color[TOP], sensitivity_level_color[BOTTOM]) ;
		//
		//Get Information
		///////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////
		//Run 
		//
		//qDebug("RUN : 3 : Run") ;
		if( status == STATUS_TEST_RUN )
		{
			//STEP 1~6 : Capture Image
//			Capture_Camera_Image(Get_Status());
			Capture_Camera_Center_Image(Get_Status());
            string RunStart_str_A = "RunStart-CAM-A";
            string RunStart_str_B = "RunStart-CAM-B";
            m_cls_api[TOP].Ensemble_Camera_Save_Image_To_Device_Local(RunStart_str_A);
            m_cls_api[BOTTOM].Ensemble_Camera_Save_Image_To_Device_Local(RunStart_str_B);

			//------------------------------------------------------------
			//STEP 7 : Do Vision
			//qDebug("RUN : 3 : Run : Step5 : Do Vision") ;
			for( int nFace=0 ; nFace<FACE_MAX_COUNT ; nFace++ ) 
			{
				str_result_xml[nFace] = m_cls_api[nFace].Ensemble_Job_Run(m_str_job_id[nFace]) ;
				m_count_run[nFace]++ ;
			}
			//------------------------------------------------------------
			//STEP 8 : Check Result & Set LED Status
			//
			//qDebug("RUN : 3 : Run : Step7 : Check Result & Set LED Status") ;
			for( int nFace=0 ; nFace<FACE_MAX_COUNT ; nFace++ ) 
			{
				//----------------------------------------
				//Result Inspect
				//Get_Result_Crack_Quality(str_result_xml[nFace], m_str_option_inspect_crack_id[nFace], &result_crack_pass[nFace], &result_crack_quality[nFace]) ;
				Get_Result_Crack_Quality(str_result_xml[nFace], m_str_option_inspect_color_id[nFace], &result_color_pass[nFace], &result_color_quality[nFace]) ;							
											
				//if( result_crack_pass[nFace] && result_color_pass[nFace] )
				if( result_color_pass[nFace] )
				{
					m_count_pass[nFace]++ ;
				}
				else
				{	
					m_count_ng[nFace]++ ;

#if 0
					if( result_crack_pass[nFace] == false )
					{
						m_count_ng_crack[nFace]++ ;
					}
					if( result_color_pass[nFace] == false )
					{	
						m_count_ng_color[nFace]++ ;
					}
#endif					
				}
				//Result Inspect
				//----------------------------------------
			}

			//if( result_crack_pass[TOP] && result_crack_pass[BOTTOM] &&
			//	result_color_pass[TOP] && result_color_pass[BOTTOM] )
			if( result_color_pass[TOP] && result_color_pass[BOTTOM] )
			{
				//PASS sign
				m_cls_api[TOP].Ensemble_Digital_IO_SetOut( IO_DEVICE_SIGN_LED_GREEN, IO_DEVICE_ON ) ;
				m_cls_api[TOP].Ensemble_Digital_IO_SetOut( IO_DEVICE_SIGN_LED_RED, IO_DEVICE_OFF ) ;
			}
			else
			{
				//NG sign
				m_cls_api[TOP].Ensemble_Digital_IO_SetOut( IO_DEVICE_SIGN_LED_GREEN, IO_DEVICE_OFF ) ;
				m_cls_api[TOP].Ensemble_Digital_IO_SetOut( IO_DEVICE_SIGN_LED_RED, IO_DEVICE_ON ) ;
			}
	
		}
		//
		//Run 
		///////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////
		//Get Image
		//
		//qDebug("RUN : 4 : Get Image") ;
		for( int nFace=0 ; nFace<FACE_MAX_COUNT ; nFace++ ) 
		{
			if( m_mat_input_image[nFace].empty() )	m_mat_input_image[nFace] = cv::Mat::zeros(DISPLAY_IMAGE_HEIGHT, DISPLAY_IMAGE_WIDTH, CV_8UC3) ;

			
			//unsigned char* get_data = NULL ;
			//int width = DISPLAY_IMAGE_WIDTH ;
			//int height = DISPLAY_IMAGE_HEIGHT ;

			const int image_type = IMAGE_RGB888 ;

			int ret_data_size = 0 ;
			if( status == STATUS_TEST_RUN )
			{
				//Get Result Imag
                ret_data_size = m_cls_api[nFace].Ensemble_Task_Get_Result_Image(m_str_job_id[nFace], image_type,  &m_image[nFace]) ;
			}
			else if( status == STATUS_CONFIG)
			{
                ret_data_size = m_cls_api[nFace].Ensemble_Task_Get_Image(m_str_job_id[nFace], image_type, &m_image[nFace])  ;
			}
			else
			{
				m_cls_api[nFace].Ensemble_Camera_Capture_SW_Trigger() ;		//New Image
                ret_data_size = m_cls_api[nFace].Ensemble_Source_Get_Image(image_type+IMAGE_ADD_TIME+IMAGE_ADD_SOURCE_INFO, &m_image[nFace]) ;
			}

			if( m_image[nFace].p_buf != NULL )
            {
				m_mat_input_image[nFace] = m_cls_imagebuf2mat.Cvt(m_image[nFace]) ;
            }
            else
            {
                if( m_mat_input_image[nFace].empty() )
                {
                    m_mat_input_image[nFace] = cv::Mat::zeros(DISPLAY_IMAGE_HEIGHT, DISPLAY_IMAGE_WIDTH, CV_8UC3) ;
                }
                else
                {
                    m_mat_input_image[nFace] = cv::Scalar(0) ;
                }
            }

			//result image
			if( status == STATUS_TEST_RUN )
			{
				//if( result_crack_pass[nFace] >= 0 || result_color_pass[nFace] >= 0 )
				{
					cv::Scalar color_inspect  = cv::Scalar(255,0,0) ;  //NG
					//if( result_crack_pass[nFace] == 1 && result_color_pass[nFace] == 1 )
					if( result_color_pass[nFace] == 1 )
					{
						color_inspect  = cv::Scalar(0,255,0) ;  //PASS
					}

					//draw
					cv::Rect rect_inspect_display = cv::Rect(0,0,m_mat_input_image[nFace].cols, m_mat_input_image[nFace].rows) ;
					cv::rectangle(m_mat_input_image[nFace], rect_inspect_display, color_inspect, 3) ;
				}
			}

			//---------------------------------------------------------------------------
			//Save Log : Result Image
			if( status == STATUS_TEST_RUN )
			{
				//if( result_crack_pass[nFace] ==false || result_color_pass[nFace] == false )
				//if( result_color_pass[nFace] == false )
				{
	                //SaveLogFile(nFace, b_ng_crack[nFace], b_ng_color[nFace]) ;
	                m_cls_log_file.SaveLogFile(m_count_run[nFace], nFace, inspect_level_crack[nFace], result_crack_quality[nFace], result_crack_pass[nFace], sensitivity_level_color[nFace], inspect_level_color[nFace], result_color_quality[nFace], result_color_pass[nFace], m_mat_input_image[nFace]) ;
				}
			}
			//Save Log : Result Image
			//---------------------------------------------------------------------------
		}
		//
		//Get Image
		///////////////////////////////////////////////////////////////////////////////////////////////////

		//image
		emit Done(m_mat_input_image[TOP], m_mat_input_image[BOTTOM]) ;
		//run count
		emit signal_Count_Run(m_count_run[TOP], m_count_run[BOTTOM]) ;
		emit signal_Count_Pass(m_count_pass[TOP], m_count_pass[BOTTOM]) ;
		emit signal_Count_Ng(m_count_ng[TOP], m_count_ng[BOTTOM]) ;
		//emit signal_Count_Ng_Crack(m_count_ng_crack[TOP], m_count_ng_crack[BOTTOM]) ;
		//emit signal_Count_Ng_Color(m_count_ng_color[TOP], m_count_ng_color[BOTTOM]) ;
		//result
		//emit signal_Quality_Crack(result_crack_quality[TOP], result_crack_quality[BOTTOM]) ;
		emit signal_Quality_Color(result_color_quality[TOP], result_color_quality[BOTTOM]) ;

		//Run delay
		if( status == STATUS_TEST_RUN )
		{			
			//QThread::msleep(1000) ;	//delay 1sec
			SetNextImage() ;
		}
		
        QThread::yieldCurrentThread() ;
        QThread::usleep(33000) ;
    }
#endif	
}
void CEnsemble::Capture_Camera_Image(int CurrentStatus)
{
	//qDebug("RUN : 3 : Run : STart") ;
	//------------------------------------------------------------
	//STEP 1 : Check Get IO ( Front Sensor )
	//
	do
	{
        if( Get_Status() != CurrentStatus ) break;
        if( (m_cls_api[TOP].Ensemble_Digital_IO_GetIn() & SENSOR_FRONT) ) break ;		//Check Photo Sensor 1
	}while(1) ;		
	//qDebug("RUN : 3 : Run : Step1 : Check Get IO : Sensor1") ;

	//------------------------------------------------------------
	//STEP 2 : Check Get IO ( Back Sensor )
	//			
	do
	{
        if( Get_Status() != CurrentStatus ) break;			
        if( m_cls_api[TOP].Ensemble_Digital_IO_GetIn() & SENSOR_BACK ) break ;		//Check Photo Sensor 2
	}while(1) ;
	//qDebug("RUN : 3 : Run : Step1 : Check Get IO : Sensor2") ;
		
	//------------------------------------------------------------
	//STEP 3 : Sign LED All Off
	//
	//qDebug("RUN : 3 : Run : Step2 : Sign LED All Off") ;
	m_cls_api[TOP].Ensemble_Digital_IO_SetOut( IO_DEVICE_SIGN_LED_GREEN, IO_DEVICE_OFF ) ;
	m_cls_api[TOP].Ensemble_Digital_IO_SetOut( IO_DEVICE_SIGN_LED_RED, IO_DEVICE_OFF ) ;

	//------------------------------------------------------------
	//STEP 4 : Light On
	//
	//qDebug("RUN : 3 : Run : Step3 : Light On") ;
	m_cls_api[TOP].Ensemble_Digital_IO_SetOut( IO_DEVICE_LIGHT, IO_DEVICE_ON ) ;
    QThread::msleep(0) ;    //TODO : timing sw delay
	//------------------------------------------------------------

	//STEP 5 : Image Capture
	//
	//qDebug("RUN : 3 : Run : Step4 : Image Capture") ;			
	do
	{
		int top_frame_cnt = m_cls_api[TOP].Ensemble_Camera_Get_Frame_Count();
		int bottom_frame_cnt = m_cls_api[BOTTOM].Ensemble_Camera_Get_Frame_Count();
		qDebug("Frame cnt(top:%d, bottom:%d)\n",top_frame_cnt, bottom_frame_cnt);
        if( Get_Status() != CurrentStatus ) break;			
        if( m_cls_api[TOP].Ensemble_Digital_IO_GetIn() & SENSOR_FRONT ) break ;		//Check Photo Sensor 1
	}while(1) ;			
    Start_Capture_Top = true;
    m_cls_api[BOTTOM].Ensemble_Camera_Capture_SW_Trigger() ;
	//STEP 6 : Light Off
	m_cls_api[TOP].Ensemble_Digital_IO_SetOut( IO_DEVICE_LIGHT, IO_DEVICE_OFF ) ;
}
void CEnsemble::Capture_Camera_Center_Image(int CurrentStatus)
{
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    std::chrono::duration<double> latency;
    double time_over_sec = 5;

	// Step 1 : Check Front Sensor High ( The Box is entering )
    qDebug("box front check\n");
	do
	{
		if( Get_Status() != CurrentStatus ) break;
        if( (m_cls_api[TOP].Ensemble_Digital_IO_GetIn() & SENSOR_FRONT) )
        {
            qDebug("Fonrt sensor start check!\n");
            break ;		//Front sensor check( Object is entering )
        }
        latency = std::chrono::system_clock::now() - start;
    }while(latency.count() < time_over_sec) ;
	// Step 1 End

	// Step 2 : Check Front Sensor Low ( The box passes through the sensor perfectly. )
	/* need to test with ENSEMBLE_base */
	do
	{
		if( Get_Status() != CurrentStatus ) break;
        if( (m_cls_api[TOP].Ensemble_Digital_IO_GetIn() & SENSOR_FRONT) == 0)
        {
            qDebug("Front sensor end check!\n");
            break ;		//Front sensor check( Waiting object out for start capture )
        }
        latency = std::chrono::system_clock::now() - start;
    }while(latency.count() < time_over_sec) ;
	/************************************/
	// Step 2 End

	// Step 3 : Get Start Frame Number
	int TOP_FrameNum_start = m_cls_api[TOP].Ensemble_Camera_Get_Frame_Count();
	int BOTTOM_FrameNum_start = m_cls_api[BOTTOM].Ensemble_Camera_Get_Frame_Count();
	// Step 3 End

	// Step 4 : Camera Capture Start
	do
	{
		Start_Capture_Top = true;
		m_cls_api[BOTTOM].Ensemble_Camera_Capture_SW_Trigger() ;
		
        if( Get_Status() != CurrentStatus )
        {
            qDebug("Back sensor OK!!\n");
            break;
        }
        if( m_cls_api[TOP].Ensemble_Digital_IO_GetIn() & SENSOR_BACK ) break ; 	//End sensor check

        latency = std::chrono::system_clock::now() - start;
    }while(latency.count() < time_over_sec) ;
	// Step 4 End
	
	// Step 5 : Get End Frame Number
	int TOP_FrameNum_end = m_cls_api[TOP].Ensemble_Camera_Get_Frame_Count();
	int BOTTOM_FrameNum_end = m_cls_api[BOTTOM].Ensemble_Camera_Get_Frame_Count();
	// Step 5 End

	int TOP_Center_FrameNum = TOP_FrameNum_start + ((TOP_FrameNum_end - TOP_FrameNum_start)/2);
	int BOTTOM_Center_FrameNum = BOTTOM_FrameNum_start + ((BOTTOM_FrameNum_end - BOTTOM_FrameNum_start)/2);	
	
	qDebug("TOP Camera Frame Change ( %d -> %d ), BOTTOM Camera Frame Change ( %d -> %d )\n", 
			TOP_FrameNum_start,TOP_FrameNum_end ,BOTTOM_FrameNum_start,BOTTOM_FrameNum_end );
	
    qDebug("TOP_Center_FrameNum[%d] / BOTTOM_Center_FrameNum[%d]\n",TOP_Center_FrameNum ,BOTTOM_Center_FrameNum);

	m_cls_api[TOP].Ensemble_Camera_Set_Camera_Image_To_Past_Frame(TOP_Center_FrameNum);
	m_cls_api[BOTTOM].Ensemble_Camera_Set_Camera_Image_To_Past_Frame(BOTTOM_Center_FrameNum);	

}


void CEnsemble::Thread_Capture_SW_Trigger()
{
    while(1)
    {
        if( Start_Capture_Top )
        {
            m_cls_api[TOP].Ensemble_Camera_Capture_SW_Trigger();
            Start_Capture_Top = false;
        }
        else
        {
            usleep(1000);
        }
    }
}

void CEnsemble::SetNextImage(void)
{
	//Next Image
	//qDebug("RUN : Test Run : Next Image") ;
	for( int nFace=0 ; nFace<FACE_MAX_COUNT ; nFace++ ) 
	{
		if( m_vec_test_source_list[nFace].size() > 0 )
		{
			//next image
			if( m_test_index[nFace] >=0 )
			{
				if( m_test_index[nFace] >= m_vec_test_source_list[nFace].size() ) m_test_index[nFace] = 0 ;
				
				m_cls_api[nFace].Ensemble_Source_Set(m_vec_test_source_list[nFace][m_test_index[nFace]]) ;
			}

            m_test_index[nFace]++ ;
		}
		else
		{
			m_vec_test_source_list[nFace] = Get_Source_List(nFace) ;

			m_test_index[nFace] = 0 ;
		}
	}
}

void CEnsemble::SetNetwork(const std::string str_ip_top, const unsigned int port_top, const std::string str_ip_bottom, const unsigned int port_bottom)
{
	m_str_ip[TOP] = str_ip_top ;
	m_port[TOP] = port_top ;

	m_str_ip[BOTTOM] = str_ip_bottom ;
	m_port[BOTTOM] = port_bottom ;
}

std::string CEnsemble::Get_Job_Info(const int surface, std::string* p_out_str_job_id, std::string* p_out_str_option_crack_id, std::string* p_out_str_option_color_id)
{
	std::string str_ret ;

	if( surface < 0 || surface >=2 ) str_ret ;
	
	//Get Job Info
	std::string str_prj_list_xml = m_cls_api[surface].Ensemble_Project_Get_List() ;

	////qDebug("Get Job Info = %s", str_prj_list_xml.c_str()) ;
	
	//Job info parsing
	//XML Parsing
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string((char *)(str_prj_list_xml.c_str()));

	if (!result)
    {
        //qDebug("xml parsing error") ;
    }
    else
    {

        for (pugi::xml_node device : doc.children("Ensemble") )
        {
        	int project_count = 0 ;
			for (pugi::xml_node project: device.children("Project"))
            {
            	std::string str_prj_id = project.attribute("ID").value();
                std::string str_prj_name = project.attribute("Name").value();

				////qDebug("Project[%d]: id=%s, name=%s", project_count, str_prj_id.c_str(), str_prj_name.c_str()) ;

				//add info
				str_ret += str_prj_name + "::" ;
				
				//Jobs			
				for (pugi::xml_node job: project.child("Jobs").children("Job"))
				{
					std::string str_job_id = job.attribute("ID").value();
					int type = job.attribute("Type").as_int();
					std::string str_name = job.attribute("Name").value();
					
					//std::string str_tool_type_name = m_cls_api.Ensemble_Info_Get_ToolTypeName(type) ;

					if( p_out_str_job_id )
					{
						(*p_out_str_job_id) = str_job_id ;
					}
					
					////qDebug("Job : Type=%d, Name=%s", type, str_name.c_str()) ;

					//add info
					str_ret += str_name + "::" ;
					
					//---------------------------
	                //Tool list
	                for (pugi::xml_node tool: job.child("Tools").children("Tool"))
	                {
	                    std::string str_id = tool.attribute("ID").value();
	                    int type = tool.attribute("Type").as_int();
	                    std::string str_name = tool.attribute("Name").value();
						
						//std::string str_tool_type_name = m_cls_api.Ensemble_Info_Get_ToolTypeName(type) ;
						
	                    ////qDebug("Tool : Type=%d, Name=%s", type, str_name.c_str()) ;

						//add info
						str_ret += str_name ;

						//---------------------------
		                //Option list
		                int count = 0 ;
		                for (pugi::xml_node option: tool.child("Options").children("Option"))
		                {
		                	std::string str_option_id = option.attribute("ID").value();
		                    int option_type = option.attribute("Type").as_int();
		                    std::string str_option_name = option.attribute("Name").value();

							//edge crack
							if( option_type == TOOL_TYPE_OPTION_INSPECT_CRACK || option_type == TOOL_TYPE_OPTION_INSPECT_CRACK2 )
							{
								if( p_out_str_option_crack_id != NULL )
								{
									(*p_out_str_option_crack_id) = str_option_id ;
								}
							}

							//inspect color
							if( option_type == TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE || option_type == TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE2 )
							{
								if( p_out_str_option_color_id != NULL )
								{
									(*p_out_str_option_color_id) = str_option_id ;
								}
							}

							////qDebug("Option : ID=%s, Type=%d(%d), Name=%s", str_option_id.c_str(), option_type, TOOL_TYPE_OPTION_INSPECT_CRACK2, str_option_name.c_str()) ;
							
							//std::string str_tool_option_type_name = m_cls_api.Ensemble_Info_Get_ToolTypeName(option_type) ;

							if( count == 0 )
							{
								str_ret += "(" ;
								str_ret += str_option_name ;
							}
							else
							{
								str_ret += ", " ;
								str_ret += str_option_name ;
							}

							count++ ;
		                }

						if( count > 0 )
						{
							str_ret += ")" ;
						}
	                }
				}
			}
        }
    }

	return str_ret ;
}

std::string CEnsemble::Get_Job_ID(const int surface)
{
	if( surface < 0 || surface >=2 ) std::string() ;
	
	return m_str_job_id[surface] ;
}

cv::Mat CEnsemble::Get_Job_Image(const int surface, const std::string str_job_id) 
{
	cv::Mat object_image ;

	if( surface < 0 || surface >=2 ) object_image ;
	
	//Get Base Object Image
	//unsigned char* get_object_image_data = NULL ;
    //int object_image_width = 320 ;
    //int object_image_height = 240 ;
	
    const int image_type = IMAGE_RGB888 ;
    //int object_image_size = m_cls_api.Ensemble_Job_Get_ObjectImage(str_job_id, image_type+IMAGE_THUMBNAIL, &get_object_image_data, &object_image_width, &object_image_height, &get_image_type)  ;
    int object_image_size = m_cls_api[surface].Ensemble_Task_Get_ObjectImage(str_job_id, image_type, &m_object_image)  ;

	if( m_object_image.p_buf != NULL )
    {
		object_image = m_cls_imagebuf2mat.Cvt(m_object_image) ;
		
        //delete [] get_object_image_data ;
        //get_object_image_data = NULL ;
    }

	return object_image ;
}

void CEnsemble::Set_Status(const int status) 
{
	m_status = status ;
}

int CEnsemble::Get_Status(void) 
{
	return m_status ;
}

void CEnsemble::Config_New(void)
{
	if( Get_Status() == STATUS_CONFIG )
	{
		SetNextImage() ;
//		Capture_Camera_Image(Get_Status());
		Capture_Camera_Center_Image(Get_Status());

        string config_new_str_A = "ConfigNew-CAM-A";
        string config_new_str_B = "ConfigNew-CAM-B";
        m_cls_api[TOP].Ensemble_Camera_Save_Image_To_Device_Local(config_new_str_A);
        m_cls_api[BOTTOM].Ensemble_Camera_Save_Image_To_Device_Local(config_new_str_B);

        m_cls_api[TOP].Ensemble_Job_Set_Image(m_str_job_id[TOP])  ;
        m_cls_api[BOTTOM].Ensemble_Job_Set_Image(m_str_job_id[BOTTOM])  ;


#if 0		
		//Light On
		m_cls_api[TOP].Ensemble_Digital_IO_SetOut( IO_DEVICE_LIGHT, IO_DEVICE_ON ) ;
        QThread::msleep(300) ;
		
		//Get New Image
//		m_cls_api[TOP].Ensemble_Camera_Capture_SW_Trigger() ;
		Start_Capture_Top = true;
		m_cls_api[BOTTOM].Ensemble_Camera_Capture_SW_Trigger() ;

		//Light Off
		m_cls_api[TOP].Ensemble_Digital_IO_SetOut( IO_DEVICE_LIGHT, IO_DEVICE_OFF ) ;
			
		//Set Base Image : not reset 
        m_cls_api[TOP].Ensemble_Job_Set_Image(m_str_job_id[TOP])  ;
        m_cls_api[BOTTOM].Ensemble_Job_Set_Image(m_str_job_id[BOTTOM])  ;
#endif        
	}
}

void CEnsemble::Config_Save(void) 
{
	if( Get_Status() == STATUS_CONFIG )
	{
		m_cls_api[TOP].Ensemble_Task_File_Save(true) ;
		m_cls_api[BOTTOM].Ensemble_Task_File_Save(true) ;
	}
}

void CEnsemble::Config_Load(const int surface)
{
	if( surface < 0 )
	{
		//load DB
		m_cls_api[TOP].Ensemble_Task_File_Load() ;
		m_cls_api[BOTTOM].Ensemble_Task_File_Load() ;
	}
	else if( surface>= 0 && surface < 2 )
	{
		m_cls_api[surface].Ensemble_Task_File_Load() ;
	}
}

void CEnsemble::Config_Set_Level(const int surface, const int level)
{
    if( surface == TOP )			m_cls_api[TOP].Ensemble_Tool_Option_Crack_Set_InspectLevel(m_str_option_inspect_crack_id[TOP], level);
    else if( surface == BOTTOM )	m_cls_api[BOTTOM].Ensemble_Tool_Option_Crack_Set_InspectLevel(m_str_option_inspect_crack_id[BOTTOM], level);
}

void CEnsemble::Config_Set_Level_ColorCompare(const int surface, const int level)
{
    if( surface == TOP )			m_cls_api[TOP].Ensemble_Tool_Option_ColorCompare_Set_InspectLevel(m_str_option_inspect_color_id[TOP], level);
    else if( surface == BOTTOM )	m_cls_api[BOTTOM].Ensemble_Tool_Option_ColorCompare_Set_InspectLevel(m_str_option_inspect_color_id[BOTTOM], level);
}

void CEnsemble::Config_Set_ColorCompare_Sensitivity(const int surface, const int level)
{
    if( surface == TOP )			m_cls_api[TOP].Ensemble_Tool_Option_ColorCompare_Set_Sensitivity(m_str_option_inspect_color_id[TOP], level);
    else if( surface == BOTTOM )	m_cls_api[BOTTOM].Ensemble_Tool_Option_ColorCompare_Set_Sensitivity(m_str_option_inspect_color_id[BOTTOM], level);
}

void CEnsemble::Config_Set_Mask(const int surface)
{
	boost::property_tree::ptree pt_object;
	boost::property_tree::ptree pt_local;

	float mask_object_fx=0 ;
	float mask_object_fy=0 ;
	float mask_object_fw=0 ;
	float mask_object_fh=0 ;

	float mask_local_fx=0 ;
	float mask_local_fy=0 ;
	float mask_local_fw=0 ;
	float mask_local_fh=0 ;
		
	if( surface == TOP )
	{
		try
		{
			boost::property_tree::ini_parser::read_ini("./mask_top_object.ini", pt_object);
			
			mask_object_fx = pt_object.get<float>("top.mask_fx") ;
			mask_object_fy = pt_object.get<float>("top.mask_fy") ;
			mask_object_fw = pt_object.get<float>("top.mask_fw") ;
			mask_object_fh = pt_object.get<float>("top.mask_fh") ;
		}
		catch(std::exception e)
		{
			mask_object_fx = 0 ;
			mask_object_fy = 0 ;
			mask_object_fw = 0 ;
			mask_object_fh = 0 ;
		}

		
		try
		{
			boost::property_tree::ini_parser::read_ini("./mask_top_local.ini", pt_local);
			
			mask_local_fx = pt_local.get<float>("top.mask_fx") ;
			mask_local_fy = pt_local.get<float>("top.mask_fy") ;
			mask_local_fw = pt_local.get<float>("top.mask_fw") ;
			mask_local_fh = pt_local.get<float>("top.mask_fh") ;
		}
		catch(std::exception e)
		{
			mask_local_fx = 0 ;
			mask_local_fy = 0 ;
			mask_local_fw = 0 ;
			mask_local_fh = 0 ;
		}
	}
	else if( surface == BOTTOM )
	{
		try
		{
			boost::property_tree::ini_parser::read_ini("./mask_bottom_object.ini", pt_object);
			
			mask_object_fx = pt_object.get<float>("bottom.mask_fx") ;
			mask_object_fy = pt_object.get<float>("bottom.mask_fy") ;
			mask_object_fw = pt_object.get<float>("bottom.mask_fw") ;
			mask_object_fh = pt_object.get<float>("bottom.mask_fh") ;
		}
		catch(std::exception e)
		{
			mask_object_fx = 0 ;
			mask_object_fy = 0 ;
			mask_object_fw = 0 ;
			mask_object_fh = 0 ;
		}

		
		try
		{
			boost::property_tree::ini_parser::read_ini("./mask_bottom_local.ini", pt_local);
			
			mask_local_fx = pt_local.get<float>("bottom.mask_fx") ;
			mask_local_fy = pt_local.get<float>("bottom.mask_fy") ;
			mask_local_fw = pt_local.get<float>("bottom.mask_fw") ;
			mask_local_fh = pt_local.get<float>("bottom.mask_fh") ;
		}
		catch(std::exception e)
		{
			mask_local_fx = 0 ;
			mask_local_fy = 0 ;
			mask_local_fw = 0 ;
			mask_local_fh = 0 ;
		}
	}

	//Mask region Clear.
	m_cls_api[surface].Ensemble_Task_Del_MaskArea(m_str_job_id[surface]);

	//Mask region Add. (Object)
	m_cls_api[surface].Ensemble_Task_Set_MaskArea(m_str_job_id[surface], mask_object_fx, mask_object_fy, mask_object_fw, mask_object_fh, false);

	//Mask region Add. (Local)
	m_cls_api[surface].Ensemble_Task_Set_MaskArea(m_str_job_id[surface], mask_local_fx, mask_local_fy, mask_local_fw, mask_local_fh, true);
}

void CEnsemble::Config_Set_Local_Mask(const int surface, const float f_x, const float f_y, const float f_w, const float f_h)
{
	
	if( surface == TOP )
	{
		//Set Object masking area
		boost::property_tree::ptree pt;

	    pt.put("top.mask_fx", f_x) ;
		pt.put("top.mask_fy", f_y) ;
		pt.put("top.mask_fw", f_w) ;
		pt.put("top.mask_fh", f_h) ;

		boost::property_tree::ini_parser::write_ini( "./mask_top_local.ini" , pt );
	}
	else if( surface == BOTTOM )
	{
		//Set Object masking area
		boost::property_tree::ptree pt;

	    pt.put("bottom.mask_fx", f_x) ;
		pt.put("bottom.mask_fy", f_y) ;
		pt.put("bottom.mask_fw", f_w) ;
		pt.put("bottom.mask_fh", f_h) ;

		boost::property_tree::ini_parser::write_ini( "./mask_bottom_local.ini" , pt );
	}

	Config_Set_Mask(surface) ;
}


void CEnsemble::Config_Set_Region(const int surface, const float f_x, const float f_y, const float f_w, const float f_h)
{
	if( surface == TOP || surface == BOTTOM )
	{
		//Add 
		const float margin_top = 0.02 ;
		float mask_fx = 0.01 ;
		float mask_fy = f_y - margin_top ;
		float mask_fw = 1.0 - (0.01)*2 ;
		float mask_fh = f_h + (margin_top*2) ;
		if( mask_fx < 0 ) mask_fx = 0 ;
		if( mask_fx > 1.0 ) mask_fx = 1.0 ;
		if( mask_fy < 0 ) mask_fy = 0 ;
		if( mask_fy > 1.0 ) mask_fy = 1.0 ;
		if( mask_fw < 0 ) mask_fw = 0 ;
		if( mask_fw > 1.0 ) mask_fw = 1.0 ;
		if( mask_fh < 0 ) mask_fh = 0 ;
		if( mask_fh > 1.0 ) mask_fh = 1.0 ;
		
        cv::Point2f pt_rotated_roi_1 = cv::Point2f(f_x, f_y) ;
        cv::Point2f pt_rotated_roi_2 = cv::Point2f(f_x + f_w, f_y) ;
        cv::Point2f pt_rotated_roi_3 = cv::Point2f(f_x + f_w, f_y + f_h) ;
        cv::Point2f pt_rotated_roi_4 = cv::Point2f(f_x, f_y + f_h) ;

		if( surface == TOP )
		{
			//Set Object masking area
			boost::property_tree::ptree pt;

		    pt.put("top.mask_fx", mask_fx) ;
			pt.put("top.mask_fy", mask_fy) ;
			pt.put("top.mask_fw", mask_fw) ;
			pt.put("top.mask_fh", mask_fh) ;
	
			boost::property_tree::ini_parser::write_ini( "./mask_top_object.ini" , pt );
		}
		else if( surface == BOTTOM )
		{
			//Set Object masking area
			boost::property_tree::ptree pt;

		    pt.put("bottom.mask_fx", mask_fx) ;
			pt.put("bottom.mask_fy", mask_fy) ;
			pt.put("bottom.mask_fw", mask_fw) ;
			pt.put("bottom.mask_fh", mask_fh) ;

			boost::property_tree::ini_parser::write_ini( "./mask_bottom_object.ini" , pt );
		}
		
		Config_Set_Mask(surface) ;		
		
        m_cls_api[surface].Ensemble_Find_Object_Set_SelectObject(m_str_job_id[surface], pt_rotated_roi_1.x, pt_rotated_roi_1.y, pt_rotated_roi_2.x, pt_rotated_roi_2.y, pt_rotated_roi_3.x, pt_rotated_roi_3.y, pt_rotated_roi_4.x, pt_rotated_roi_4.y ) ;
	}
}

std::vector<std::string> CEnsemble::Get_Source_List(const int surface)
{
	std::vector<std::string> ret_vec_str_path ;

	if( surface < 0 || surface >=2 ) ret_vec_str_path ;
	
	std::string str_source_list_xml = m_cls_api[surface].Ensemble_Source_Get_List() ;

	//xml parsing
	//XML Parsing
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string((char *)(str_source_list_xml.c_str()));

    if (!result)
    {
        //qDebug("xml parsing error") ;
    }
    else
    {
        for (pugi::xml_node job: doc.child("Ensemble").children("Image"))
        {
            std::string str_path = job.child("PATH").text().get();
            std::string str_path2 = str_path ;

			ret_vec_str_path.push_back(str_path2) ;
        }
    }

	return ret_vec_str_path ;
}

/*
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>	
<Result ID='20200303152237_eWTM6398'>
	<Jobs ID='20200303152237_eWTM6398' TYPE='30001' FindCount='1'>    
		<Job>        
			<Src Width='640' Height='480'/>        
			<Pose CenterX='330.000000' CenterY='337.000000' Angle='357' Roi_TL_X='71.000000' Roi_TL_Y='243.000000' Roi_TR_X='598.276367' Roi_TR_Y='270.633392' Roi_BR_X='589.745605' Roi_BR_Y='433.410004' Roi_BL_X='62.469238' Roi_BL_Y='405.776611' />        
			<Matching Score='0.747813'/>        
			<Specific>        </Specific>        
			<Tools>    
				<Tool ID='20200303152301_wuzI6025' TYPE='40500'>        
					<Pose CenterX='330.000000' CenterY='337.000000' Angle='357' Roi_TL_X='71.000000' Roi_TL_Y='243.000000' Roi_TR_X='598.276367' Roi_TR_Y='270.633392' Roi_BR_X='589.745605' Roi_BR_Y='433.410004' Roi_BL_X='62.469238' Roi_BL_Y='405.776611' />        
					<Matching Score='0.747813'/>        
					<Specific>        </Specific>        
					<Options>    
						<Option ID='20200303152310_fmMJ6097' TYPE='40101'>        
							<Specific>            
								<Pass>1</Pass>            
								<Quality>100.000000</Quality>        
							</Specific>    
						</Option>        
					</Options>    
				</Tool>        
			</Tools>    
		</Job>
	</Jobs>	
</Result>
*/
void CEnsemble::Get_Result_Crack_Quality(const std::string str_result_xml, const std::string job_id, int* out_pass, float* out_quality)
{
	//Job info parsing
	//XML Parsing
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string((char *)(str_result_xml.c_str()));

	if (!result)
    {
        //qDebug("xml parsing error") ;
    }
    else
    {

        for (pugi::xml_node result : doc.children("Result") )
        {
			for (pugi::xml_node jobs: result.children("Jobs"))
            {
            	std::string jobs_str_id = jobs.attribute("ID").value();
                int jobs_type = jobs.attribute("TYPE").as_int();
				int jobs_find_count = jobs.attribute("FindCount").as_int();

				for (pugi::xml_node job: jobs.children("Job"))
				{
					int job_source_width = job.child("Src").attribute("Width").as_int();
					int job_source_height = job.child("Src").attribute("Height").as_int();
						
					//---------------------------
	                //Tool list
	                for (pugi::xml_node tool: job.child("Tools").children("Tool"))
	                {
	                    std::string str_id = tool.attribute("ID").value();
	                    int type = tool.attribute("Type").as_int();

						//---------------------------
		                //Option list
		                for (pugi::xml_node option: tool.child("Options").children("Option"))
		                {
		                	std::string str_option_id = option.attribute("ID").value();
		                    int option_type = option.attribute("Type").as_int();

                            int pass = option.child("Specific").child("Pass").text().as_int();
                            float quality = option.child("Specific").child("Quality").text().as_float();

                            ////qDebug("str_option_id = %s, quality = %f", str_option_id.c_str(), quality) ;

							if( job_id == str_option_id )
							{
								if( out_pass ) (*out_pass) = pass ;
								if( out_quality ) (*out_quality) = quality ;

								break ;
							}
		                }
	                }
				}
			}
        }
    }
}

void CEnsemble::Set_Ensemble_Light(bool on)
{
	if( on )
	{		
		m_cls_api[TOP].Ensemble_Digital_IO_SetOut( IO_DEVICE_LIGHT, IO_DEVICE_ON ) ;
	}
	else
	{
		m_cls_api[TOP].Ensemble_Digital_IO_SetOut( IO_DEVICE_LIGHT, IO_DEVICE_OFF ) ;
	}
}

void CEnsemble::Run_Enable_Crack(const int surface, const bool run)
{
	if( surface == TOP )			m_cls_api[TOP].Ensemble_Task_Set_Run_Option(m_str_option_inspect_crack_id[TOP], run);
    else if( surface == BOTTOM )	m_cls_api[BOTTOM].Ensemble_Task_Set_Run_Option(m_str_option_inspect_crack_id[BOTTOM], run);
}

void CEnsemble::Run_Enable_Color(const int surface, const bool run)
{
	if( surface == TOP )			m_cls_api[TOP].Ensemble_Task_Set_Run_Option(m_str_option_inspect_color_id[TOP], run);
    else if( surface == BOTTOM )	m_cls_api[BOTTOM].Ensemble_Task_Set_Run_Option(m_str_option_inspect_color_id[BOTTOM], run);
}

