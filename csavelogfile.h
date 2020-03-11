#ifndef CSAVELOGFILE_H
#define CSAVELOGFILE_H

//boost
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

#include <iostream>
#include <fstream>

#include "opencv2/opencv.hpp"

#define LOG_BASE_FOLDER	"Log"

class CSaveLogFile
{
public:
    CSaveLogFile();

    void SaveLogFile(const int run_count, const int face, const float inspect_level_crack, const float crack_quality, const bool ng_crack, const float sensitivity_level_color, const float inspect_level_color, const float color_quality, const bool ng_color, cv::Mat image) ;
};

#endif // CSAVELOGFILE_H
