#include "CheckLogSize.h"

void CCheckLogSize::run(void)
{	
	//qDebug("Log Thread start = %d", m_thread_run) ;
	
#if 1
    while(m_thread_run)
    {       
    	const unsigned long long log_size = getFoldersize("Log") ;

		if( log_size >= 100000000000 )		//100 Giga byte 
		{
			boost::filesystem::path someDir("Log");
			boost::filesystem::directory_iterator end_iter;

			typedef std::multimap<std::time_t, boost::filesystem::path> result_set_t; 
			result_set_t result_set;

			if ( boost::filesystem::exists(someDir) && boost::filesystem::is_directory(someDir))
			{
			     for( boost::filesystem::directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter)
			     {
			       //if (boost::filesystem::is_regular_file(dir_iter->status()) )
			       {
			         result_set.insert(result_set_t::value_type(boost::filesystem::last_write_time(dir_iter->path()), *dir_iter));
			       }
			     }
			}

			qDebug("------------ db ") ;
			std::vector<std::string> vec_path ;
			std::multimap<std::time_t, boost::filesystem::path>::iterator iter;
		    for (iter = result_set.begin(); iter != result_set.end(); ++iter)
		    {
		    	std::string str_folder = boost::filesystem::canonical(iter->second).string() ;
				qDebug("folder path = %s", str_folder.c_str()) ;
				
		    	vec_path.push_back(str_folder) ;				
		    }

			//delete oldest folder
			if( vec_path.size() > 0 )
			{
				std::string str_del_path = vec_path[0] ;
				if(boost::filesystem::exists(str_del_path.c_str()))
				{
			    	boost::filesystem::remove_all(str_del_path);
				}
			}
			
		}
		
		qDebug("log size = %ld", log_size) ;
		
        QThread::yieldCurrentThread() ;
        QThread::sleep(60) ;		//1 min
    }
#endif	
}

const int CCheckLogSize::GetSize(const std::string path)
{
	//int size = 0 ;

	DIR *d;
	struct dirent *de;
	struct stat buf;
	int exists;
	int total_size;

	d = opendir(path.c_str());
	if (d == NULL) 
	{
		perror("prsize");
		return -1 ;
	}

	total_size = 0;

	for (de = readdir(d); de != NULL; de = readdir(d)) 
	{
		exists = stat(de->d_name, &buf);
		if (exists < 0) 
		{
		  fprintf(stderr, "Couldn't stat %s\n", de->d_name);
		}
		else 
		{
		  total_size += buf.st_size;
		}
	}
	
	closedir(d);
	//printf("%d\n", total_size);

	return total_size ;
}

long long int  CCheckLogSize::getFoldersize(std::string rootFolder)
{
    // command to be executed
    std::string cmd("du -sb ");
    cmd.append(rootFolder);
    cmd.append(" | cut -f1 2>&1");

    // execute above command and get the output
    FILE *stream = popen(cmd.c_str(), "r");
    if (stream) 
	{
        const int max_size = 256;
        char readbuf[max_size];
        if (fgets(readbuf, max_size, stream) != NULL) 
		{
            return atoll(readbuf);
        }   
        pclose(stream);            
    }           
    // return error val
    return -1;
}


