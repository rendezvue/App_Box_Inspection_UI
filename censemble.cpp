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
                ret = m_cls_api.Ensemble_Source_Get_Image(GET_IMAGE_INPUT, std::string(), image_type+IMAGE_ADD_TIME+IMAGE_ADD_SOURCE_INFO, &get_data, &width, &height, &get_source_image_type) ;

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

