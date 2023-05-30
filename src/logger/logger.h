#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <ctime>

class logger
{

protected:

	static std::map<std::string, std::pair<std::ofstream*, size_t>> _collection_streams;

public:

	enum class severity
	{
		TRACE,
		DEBUG,
		INFORMATION,
		WARNING,
		ERROR,
		CRITICAL
	};

	virtual logger* log(
            const std::string&,
            severity) = 0;

    virtual ~logger();

};

#endif // LOGGER_H