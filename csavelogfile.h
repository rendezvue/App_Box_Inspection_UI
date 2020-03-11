#ifndef CSAVELOGFILE_H
#define CSAVELOGFILE_H

//boost
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

#define LOG_BASE_FOLDER	"Log"

class CSaveLogFile
{
public:
    CSaveLogFile();

	void SaveLogFile(const int face, const bool ng_crack, const bool ng_color) ;
};

#endif // CSAVELOGFILE_H
