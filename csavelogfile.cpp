#include "csavelogfile.h"

CSaveLogFile::CSaveLogFile()
{

}
void CSaveLogFile::SaveLogFile(const int run_count, const int face, const float inspect_level_crack, const float crack_quality, const bool ng_crack, const float sensitivity_level_color, const float inspect_level_color, const float color_quality, const bool ng_color, cv::Mat image)
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
	boost::posix_time::time_duration durObj = timeLocal.time_of_day();
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
	std::string str_base_log_file = str_path_today + "/" + to_iso_string(dateObj) + "_log.csv" ;

	std::string str_save_time = to_simple_string(dateObj) + " " + to_simple_string(durObj);
	std::string str_save_image_file  ;
	std::string str_ng_pass_name ;
    if( ng_crack == false )	str_ng_pass_name += "_NGCRACK" ;
    if( ng_color == false )	str_ng_pass_name += "_NGCOLOR" ;
		

    if( face == 0 )			str_save_image_file = str_path_top + "/top_" + to_iso_string(dateObj) + "_" + std::to_string(run_count) + "_image" + str_ng_pass_name + ".png" ;
    else if( face == 1 )	str_save_image_file = str_path_bottom + "/bottom_" + to_iso_string(dateObj) + "_" + std::to_string(run_count) + "_image" + str_ng_pass_name + ".png" ;

	cv::imwrite(str_save_image_file, image) ;
	
    std::ofstream myfile;
	myfile.open (str_base_log_file, std::ofstream::out | std::ofstream::app);
	myfile << str_save_time << "," << face << "," << inspect_level_crack << "," << crack_quality << "," << ng_crack << "," << sensitivity_level_color << "," << inspect_level_color << "," << color_quality << "," << ng_color << "," << str_save_image_file << std::endl ;
	myfile.close();

}

