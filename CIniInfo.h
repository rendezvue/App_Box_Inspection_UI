#ifndef CINIINFO_H
#define CINIINFO_H


//boost 
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>

#include <boost/range/iterator_range.hpp>
#include <boost/system/error_code.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/iostreams/categories.hpp>  // sink_tag
#include <iterator>  // back_inserter
#include "boost/date_time/local_time/local_time.hpp"

#define INI_PATH_FILE	"./info.ini"

//base information
#define TOP_IP			"192.168.56.102"
#define TOP_PORT		4000

#define BOTTOM_IP		"192.168.56.102"
#define BOTTOM_PORT		4001

class CIniInfo
{
public:
    CIniInfo();

public:
	void Save(const std::string str_top_ip, const int top_port, const std::string str_bottom_ip, const int bottom_port) ;

	std::string Get_Top_Ip_Addr(void) ;
	int Get_Top_Port_Num(void) ;

	std::string Get_Bottom_Ip_Addr(void) ;
	int Get_Bottom_Port_Num(void) ;
};

#endif // CINIINFO_H
