#ifndef CCHECK_LOG_SIZE_H
#define CCHECK_LOG_SIZE_H

#include <QThread>
#include <chrono>

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#include <boost/filesystem.hpp>


class CCheckLogSize : public QThread
{
    Q_OBJECT

public:
    explicit CCheckLogSize(QObject *parent = 0):
            QThread(parent)
          , m_thread_run(true)
    {    	
    }
    ~CCheckLogSize() 
	{
	}
public:
	bool m_thread_run ;

protected:
    void run(void) ;

private:
	const int GetSize(const std::string path) ;
	long long int getFoldersize(std::string rootFolder) ;
	
};

#endif // CGETIMAGETHREAD_H
