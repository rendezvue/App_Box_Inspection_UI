#include "censemble.h"

void CEnsemble::run(void)
{
    qRegisterMetaType<cv::Mat>("cv::Mat");

	//Connect
	m_cls_api.Ensemble_Network_Connect(m_str_ip.c_str(), m_port) ;
	
    while(m_thread_run)
    {        
    	//Check Network

		if( !m_str_ip.empty() )
        {
            if( !m_cls_api.Ensemble_Network_IsOnline() )
            {
                qDebug("Try Re-Connect = %s", m_str_ip.c_str()) ;
                //try re-connect
                m_cls_api.Ensemble_Network_Disconnect() ;
                m_cls_api.Ensemble_Network_Connect(m_str_ip.c_str(), m_port) ;

				emit NetStatus(false);
            }
            else
            {
#if 1    
				std::string str_job_info = Get_Job_Info(&m_str_job_id) ;
				//qDebug("str_job_info = %s", str_job_info.c_str()) ;

				if( str_job_info.empty() )
				{
					//load DB
	            	m_cls_api.Ensemble_Task_File_Load() ;
				}
				else
				{
                    QString qstr_info = QString::fromStdString(m_str_job_id);
        			emit JobInfo(qstr_info);
				}
#endif				

#if 1
				//Get Object Image
                cv::Mat job_image = Get_Job_Image(m_str_job_id) ;
				emit UpdateObjectImae(job_image) ;
#endif				
            	//Get Image
            	if( m_mat_input_image.empty() )
		        {
		            m_mat_input_image = cv::Mat::zeros(DISPLAY_IMAGE_HEIGHT, DISPLAY_IMAGE_WIDTH, CV_8UC3) ;
		        }

				unsigned char* get_data = NULL ;
	            int width = DISPLAY_IMAGE_WIDTH ;
	            int height = DISPLAY_IMAGE_HEIGHT ;

	            const int image_type = IMAGE_RGB888 ;
				int get_result_image_type = 0 ;
				int get_source_image_type = 0 ;
				
	            int ret = 0 ;

				//qDebug("test 0") ;
				const int status = Get_Status() ;
				if( status == STATUS_CONFIG)
				{
					//Set Base Image : not reset 
					//m_cls_api.Ensemble_Job_Set_Image(str_job_id, false)  ;
					
					//qDebug("test 1") ;
                    ret = m_cls_api.Ensemble_Job_Get_Image(m_str_job_id, image_type, &get_data, &width, &height, &get_source_image_type)  ;
				}
				else
				{
					//qDebug("test 2") ;
                	ret = m_cls_api.Ensemble_Source_Get_Image(GET_IMAGE_INPUT, std::string(), image_type+IMAGE_ADD_TIME+IMAGE_ADD_SOURCE_INFO, &get_data, &width, &height, &get_source_image_type) ;
				}
				//qDebug("test 3") ;
				
				if( get_data != NULL )
	            {
	                if( width>0 && height >0 )
	                {
	                    if( get_source_image_type == IMAGE_YUV420 )
	                    {
	                        cv::Mat get_image(height + height / 2, width, CV_8UC1, get_data) ;

	                        CImgDec cls_image_decoder ;
	                        m_mat_input_image = cls_image_decoder.Decoding(get_image) ;
	                    }
	                    else if( get_source_image_type == IMAGE_RGB888 )
	                    {
	                        cv::Mat get_image(height, width, CV_8UC3, get_data) ;
	                        cv::cvtColor(get_image, m_mat_input_image, cv::COLOR_BGR2RGB) ;
	                    }
						 else if( get_source_image_type == IMAGE_JPG )
	                    {
	                        cv::Mat get_image = cv::imdecode(cv::Mat(1,  width*height, CV_8UC1, get_data), cv::IMREAD_UNCHANGED);
	                        cv::cvtColor(get_image, m_mat_input_image, cv::COLOR_BGR2RGB) ;
	                    }
	                }

	                delete [] get_data ;
	                get_data = NULL ;
	            }
	            else
	            {
	                if( m_mat_input_image.empty() )
	                {
	                    m_mat_input_image = cv::Mat::zeros(DISPLAY_IMAGE_HEIGHT, DISPLAY_IMAGE_WIDTH, CV_8UC3) ;
	                }
	                else
	                {
	                    m_mat_input_image = cv::Scalar(0) ;
	                }
	            }

				emit Done(m_mat_input_image);
				emit NetStatus(true);
            }
        }
		
        QThread::yieldCurrentThread() ;
        QThread::usleep(33000) ;
    }
}

void CEnsemble::SetIP(const std::string str_ip)
{
	m_str_ip = str_ip ;
}

void CEnsemble::SetPort(const unsigned int port)
{
	m_port = port ;
}

std::string CEnsemble::Get_Job_Info(std::string* p_out_str_job_id)
{
	std::string str_ret ;
	
	//Get Job Info
	std::string str_prj_list_xml = m_cls_api.Ensemble_Project_Get_List() ;

	//Job info parsing
	//XML Parsing
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string((char *)(str_prj_list_xml.c_str()));

	if (!result)
    {
        qDebug("xml parsing error") ;
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

				//qDebug("Project[%d]: id=%s, name=%s", project_count, str_prj_id.c_str(), str_prj_name.c_str()) ;

				//add info
				str_ret += str_prj_name + "::" ;
				
				//Jobs			
				for (pugi::xml_node job: project.child("Jobs").children("Job"))
				{
					std::string str_job_id = job.attribute("ID").value();
					int type = job.attribute("Type").as_int();
					std::string str_name = job.attribute("Name").value();
					
					std::string str_tool_type_name = m_cls_api.Ensemble_Info_Get_ToolTypeName(type) ;

					if( p_out_str_job_id )
					{
						(*p_out_str_job_id) = str_job_id ;
					}
					
					//qDebug("Job : Type=%d, TypeName=%s, Name=%s", type, str_tool_type_name.c_str(), str_name.c_str()) ;

					//add info
					str_ret += str_name + "::" ;
					
					//---------------------------
	                //Tool list
	                for (pugi::xml_node tool: job.child("Tools").children("Tool"))
	                {
	                    std::string str_id = tool.attribute("ID").value();
	                    int type = tool.attribute("Type").as_int();
	                    std::string str_name = tool.attribute("Name").value();
						
						std::string str_tool_type_name = m_cls_api.Ensemble_Info_Get_ToolTypeName(type) ;
						
	                    //qDebug("Tool Type=%d, TypeName=%s, Name=%s", type, str_tool_type_name.c_str(), str_name.c_str()) ;

						//add info
						str_ret += str_name ;
	                }
				}
			}
        }
    }

	return str_ret ;
}

cv::Mat CEnsemble::Get_Job_Image(const std::string str_job_id) 
{
	cv::Mat object_image ;
	
	//Get Base Object Image
	unsigned char* get_object_image_data = NULL ;
    int object_image_width = 320 ;
    int object_image_height = 240 ;
	
    const int image_type = IMAGE_RGB888 ;
    int get_image_type = 0 ;
    //int object_image_size = m_cls_api.Ensemble_Job_Get_ObjectImage(str_job_id, image_type+IMAGE_THUMBNAIL, &get_object_image_data, &object_image_width, &object_image_height, &get_image_type)  ;
    int object_image_size = m_cls_api.Ensemble_Job_Get_ObjectImage(str_job_id, image_type, &get_object_image_data, &object_image_width, &object_image_height, &get_image_type)  ;

	if( get_object_image_data != NULL )
    {
		if( object_image_width > 0 && object_image_height > 0 )
		{
            if( get_image_type == IMAGE_YUV420 )
			{
				//YUV420 
		        cv::Mat get_image(object_image_height + object_image_height / 2, object_image_width, CV_8UC1, get_object_image_data) ;

		        CImgDec cls_image_decoder ;
		        object_image = cls_image_decoder.Decoding(get_image) ;
			}
            else if( get_image_type == IMAGE_RGB888 )
			{
                cv::Mat get_image(object_image_height, object_image_width, CV_8UC3, get_object_image_data) ;
				cv::cvtColor(get_image, object_image, cv::COLOR_BGR2RGB) ;
			}
            else if( get_image_type == ImageTypeOption::IMAGE_JPG)
            {
                cv::Mat get_image = cv::imdecode(cv::Mat(1, object_image_width*object_image_height, CV_8UC1, get_object_image_data), cv::IMREAD_UNCHANGED) ;
                cv::cvtColor(get_image, object_image, cv::COLOR_BGR2RGB) ;
            }
		}
    
        delete [] get_object_image_data ;
        get_object_image_data = NULL ;
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
		//Set Base Image : not reset 
		m_cls_api.Ensemble_Job_Set_Image(m_str_job_id)  ;
	}
}

void CEnsemble::Config_Save(void) 
{
	if( Get_Status() == STATUS_CONFIG )
	{
		m_cls_api.Ensemble_Task_File_Save() ;
	}
}

