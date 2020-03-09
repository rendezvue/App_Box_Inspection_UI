#ifndef CENSEMBLE_H
#define CENSEMBLE_H

#include "opencv2/opencv.hpp"
#include <QThread>

//API
#include "EnsembleAPI.h"
#include "ImgDec.h"
#include "pugixml.hpp"

#define DISPLAY_IMAGE_WIDTH		640
#define DISPLAY_IMAGE_HEIGHT	480

#define STATUS_NORMAL		0
#define STATUS_CONFIG		1
#define STATUS_TEST_RUN		2

#define TOP		0
#define BOTTOM	1
#define FACE_MAX_COUNT 2


class CEnsemble : public QThread
{
    Q_OBJECT

public:
    explicit CEnsemble(QObject *parent = 0):
            QThread(parent)
          , m_thread_run(true)
          , m_status(STATUS_NORMAL)
    {    	
    	for( int i=0 ; i<FACE_MAX_COUNT ; i++ )
    	{
			m_count_run[i] = 0 ;
			m_count_pass[i] = 0 ;
			m_count_ng[i] = 0 ;
			m_count_ng_crack[i] = 0 ;
			m_count_ng_color[i] = 0 ;
    	}
    }
    ~CEnsemble() {}
public:
	void SetNetwork(const std::string str_ip_top, const unsigned int port_top, const std::string str_ip_bottom, const unsigned int port_bottom) ;

	std::string Get_Job_Info(const int surface, std::string* p_out_str_job_id=NULL, std::string* p_out_str_option_crack_id=NULL, std::string* p_out_str_option_color_id=NULL) ;
	cv::Mat Get_Job_Image(const int surface, const std::string str_job_id="") ; 
	std::string Get_Job_ID(const int surface) ;

	std::vector<std::string> Get_Source_List(const int surface) ;

	void Get_Result_Crack_Quality(const std::string str_result_xml, const std::string job_id, int* out_pass, float* out_quality) ;
		
	void Set_Status(const int status) ;
	int Get_Status(void) ;
	
	void Config_New(void) ;
	void Config_Save(void) ;
	void Config_Load(const int surface=-1) ;

	void Config_Set_Level(const int surface, const int level) ;
	void Config_Set_Level_ColorCompare(const int surface, const int level) ;
	void Config_Set_ColorCompare_Sensitivity(const int surface, const int level) ;
	
	void Config_Set_Region(const int surface, const float f_x, const float f_y, const float f_w, const float f_h) ;
	
	bool m_thread_run ;

private:
	std::string m_str_ip[2] ;
	unsigned int m_port[2] ;

	CEnsembleAPI m_cls_api[2] ;
	cv::Mat m_mat_input_image[2] ;

	int m_status ;

	//top
	std::string m_str_job_id[2] ; 
	std::string m_str_option_inspect_crack_id[2] ;
	std::string m_str_option_inspect_color_id[2] ;
	
	unsigned int m_count_run[2] ;
	unsigned int m_count_pass[2] ;
	unsigned int m_count_ng[2] ;
	unsigned int m_count_ng_crack[2] ;
	unsigned int m_count_ng_color[2] ;
	
protected:
    void run(void) ;

signals:
	void Done(cv::Mat image_top, cv::Mat image_bottom);
	void Done_Top(cv::Mat image);
	void Done_Bottom(cv::Mat image);
	void UpdateObjectImae(cv::Mat image_top, cv::Mat image_bottom);
	void UpdateObjectImae_Top(cv::Mat image);
	void UpdateObjectImae_Bottom(cv::Mat image);
	void NetStatus(bool b_con_top,bool b_con_bottom);
	void NetStatus_Top(bool b_con);
	void NetStatus_Bottom(bool b_con);
	void JobInfo(QString qstr_info_top, QString qstr_info_bottom);
	void JobInfo_Top(QString qstr_info);
	void JobInfo_Bottom(QString qstr_info);
	void RunCheck_Crack(bool b_run_top, bool b_run_bottom);
	void RunCheck_Crack_Top(bool b_run);
	void RunCheck_Crack_Bottom(bool b_run);
	void RunCheck_Color(bool b_run_top, bool b_run_bottom);
	void RunCheck_Color_Top(bool b_run);
	void RunCheck_Color_Bottom(bool b_run);
	void Level_Crack(int level_top, int level_bottom);
	void Level_Crack_Top(int level);
	void Level_Crack_Bottom(int level);
	void Level_Color(int level_top, int level_bottom);
	void Level_Color_Top(int level);
	void Level_Color_Bottom(int level);
	void Sensitivity_Color(int level_top, int level_bottom);
	void Sensitivity_Color_Top(int level);
	void Sensitivity_Color_Bottom(int level);
	void signal_Quality_Crack(float quality_top, float quality_bottom);
	void signal_Quality_Crack_Top(float quality);
	void signal_Quality_Crack_Bottom(float quality);
	void signal_Quality_Color(float quality_top, float quality_bottom);
	void signal_Quality_Color_Top(float quality);
	void signal_Quality_Color_Bottom(float quality);

	void signal_Count_Run(int count_top, int count_bottom);
	void signal_Count_Run_Top(int count);
	void signal_Count_Run_Bottom(int count);
	void signal_Count_Pass(int count_top, int count_bottom);
	void signal_Count_Pass_Top(int count);
	void signal_Count_Pass_Bottom(int count);
	void signal_Count_Ng(int count_top, int count_bottom);
	void signal_Count_Ng_Top(int count);
	void signal_Count_Ng_Bottom(int count);
	void signal_Count_Ng_Crack(int count_top, int count_bottom);
	void signal_Count_Ng_Crack_Top(int count);
	void signal_Count_Ng_Crack_Bottom(int count);
	void signal_Count_Ng_Color(int count_top, int count_bottom);
	void signal_Count_Ng_Color_Top(int count);
	void signal_Count_Ng_Color_Bottom(int count);

};

#endif // CGETIMAGETHREAD_H
