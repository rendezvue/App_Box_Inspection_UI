#include "csavelogfile.h"

CSaveLogFile::CSaveLogFile()
{

}

void CSaveLogFile::SaveLogFile(const int face, const bool ng_crack, const bool ng_color)
{
	//---------------------------------------------------------------------------------
	//check Base folder
	if(!boost::filesystem::exists(LOG_BASE_FOLDER))
	{
		//Create Base Folder
		//Create Directory
		boost::filesystem::path directory(LOG_BASE_FOLDER);
		boost::filesystem::create_directory(directory);	
	}

	//---------------------------------------------------------------------------------
	//Check Today folder
	// Get current system time
	boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
	// Get Date object from ptime object
	boost::gregorian::date dateObj = timeLocal.date();
	// Get Date object from ptime object
	//boost::gregorian::date dateObj = timeLocal.date();
 
	//std::cout<<"Date Object = "<<dateObj<<std::endl;
    std::string str_path_today = std::string(LOG_BASE_FOLDER) + "/" + to_iso_string(dateObj) ;
	if(!boost::filesystem::exists(str_path_today))
	{
		//Create Directory
		boost::filesystem::path directory(str_path_today);
		boost::filesystem::create_directory(directory);	
	}

	//---------------------------------------------------------------------------------
	//check Top Folder
	std::string str_path_top = str_path_today + "/top" ;
	if(!boost::filesystem::exists(str_path_top))
	{
		//Create Directory
		boost::filesystem::path directory(str_path_top);
		boost::filesystem::create_directory(directory);	
	}

	//---------------------------------------------------------------------------------
	//check Bottom Folder
	std::string str_path_bottom = str_path_today + "/bottom" ;
	if(!boost::filesystem::exists(str_path_bottom))
	{
		//Create Directory
		boost::filesystem::path directory(str_path_bottom);
		boost::filesystem::create_directory(directory);	
	}

	//Save Information
	FILE *fp ;
	fp = fopen(filename, "w+") ;
	
}

