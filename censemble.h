#ifndef CENSEMBLE_H
#define CENSEMBLE_H

#include "opencv2/opencv.hpp"
#include <QThread>

//API
#include "EnsembleAPI.h"
#include "ImgDec.h"

#define DISPLAY_IMAGE_WIDTH		640
#define DISPLAY_IMAGE_HEIGHT	480

class CEnsemble : public QThread
{
    Q_OBJECT

public:
    explicit CEnsemble(QObject *parent = 0):
            QThread(parent)
          , m_thread_run(true)
    {
    }
    ~CEnsemble() {}
public:
	void SetIP(const std::string str_ip) ;
	void SetPort(const unsigned int port) ;
	
	bool m_thread_run ;

private:
	std::string m_str_ip ;
	unsigned int m_port ;

	CEnsembleAPI m_cls_api ;
	cv::Mat m_mat_input_image ;
	
protected:
    void run(void) ;

signals:
	void Done(cv::Mat image);

};

#endif // CGETIMAGETHREAD_H
