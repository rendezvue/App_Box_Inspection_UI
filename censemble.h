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

class CEnsemble : public QThread
{
    Q_OBJECT

public:
    explicit CEnsemble(QObject *parent = 0):
            QThread(parent)
          , m_thread_run(true)
          , m_status(STATUS_NORMAL)
          , m_count_run(0)
          , m_count_pass(0)
          , m_count_ng(0)
    {
    }
    ~CEnsemble() {}
public:
	void SetIP(const std::string str_ip) ;
	void SetPort(const unsigned int port) ;

	std::string Get_Job_Info(std::string* p_out_str_job_id=NULL, std::string* p_out_str_option_crack_id=NULL, std::string* p_out_str_option_color_id=NULL) ;
	cv::Mat Get_Job_Image(const std::string str_job_id="") ; 
	std::string Get_Job_ID(void) ;

	std::vector<std::string> Get_Source_List(void) ;

	void Get_Result_Crack_Quality(const std::string str_result_xml, const std::string job_id, int* out_pass, float* out_quality) ;
		
	void Set_Status(const int status) ;
	int Get_Status(void) ;
	
	void Config_New(void) ;
	void Config_Save(void) ;
	void Config_Load(void) ;

	void Config_Set_Level(const int level) ;
	void Config_Set_Level_ColorCompare(const int level) ;
	
	void Config_Set_Region(const float f_x, const float f_y, const float f_w, const float f_h) ;
	
	bool m_thread_run ;

private:
	std::string m_str_ip ;
	unsigned int m_port ;

	CEnsembleAPI m_cls_api ;
	cv::Mat m_mat_input_image ;

	int m_status ;

	std::string m_str_job_id ; 
	std::string m_str_option_inspect_crack_id ;
	std::string m_str_option_inspect_color_id ;

	unsigned int m_count_run ;
	unsigned int m_count_pass ;
	unsigned int m_count_ng ;
	
protected:
    void run(void) ;

signals:
	void Done(cv::Mat image);
	void UpdateObjectImae(cv::Mat image);
	void NetStatus(bool b_con);
	void JobInfo(QString qstr_info);
	void RunCheck_Crack(bool b_run);
	void RunCheck_Color(bool b_run);
	void Level_Crack(int level);
	void Level_Color(int level);
	void signal_Quality_Crack(float quality);

	void signal_Count_Run(int count);
	void signal_Count_Pass(int count);
	void signal_Count_Ng(int count);

};

#endif // CGETIMAGETHREAD_H
