#include "CIniInfo.h"

CIniInfo::CIniInfo()
{

}

void CIniInfo::Save(const std::string str_top_ip, const int top_port, const std::string str_bottom_ip, const int bottom_port)
{
	boost::property_tree::ptree pt;

    pt.put("top.ip", str_top_ip) ;
	pt.put("top.port", top_port) ;

	pt.put("bottom.ip", str_bottom_ip) ;
	pt.put("bottom.port", bottom_port) ;

	boost::property_tree::ini_parser::write_ini( INI_PATH_FILE , pt );
}

std::string CIniInfo::Get_Top_Ip_Addr(void)
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(INI_PATH_FILE, pt);

	std::string str_ip = TOP_IP ;
	try
	{
		str_ip = pt.get<std::string>("top.ip") ;

	}
	catch(std::exception e)
	{
		str_ip = TOP_IP ;
	}

	return str_ip ;
}

int CIniInfo::Get_Top_Port_Num(void)
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(INI_PATH_FILE, pt);

	int port = TOP_PORT ;
	try
	{
		port = pt.get<int>("top.port") ;

	}
	catch(std::exception e)
	{
        port = TOP_PORT ;
	}

	return port ;
}

std::string CIniInfo::Get_Bottom_Ip_Addr(void)
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(INI_PATH_FILE, pt);

	std::string str_ip = BOTTOM_IP ;
	try
	{
		str_ip = pt.get<std::string>("bottom.ip") ;

	}
	catch(std::exception e)
	{
        str_ip = BOTTOM_IP ;
	}

	return str_ip ;
}

int CIniInfo::Get_Bottom_Port_Num(void)
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(INI_PATH_FILE, pt);

	int port = BOTTOM_PORT ;
	try
	{
		port = pt.get<int>("bottom.port") ;

	}
	catch(std::exception e)
	{
        port = BOTTOM_PORT ;
	}

	return port ;
}


