/*
 * logger.cpp
 *
 * Jared Koenig and Emery Weist
 *
 * 12/10/2020
 */

#include "../includes/logger.h"

using namespace std;

Logger::Logger(std::string filename) {
	this->filename = filename;
}

Logger::~Logger() {
	if (myFile.is_open()) {
		myFile.close();
	}
}

/*
 * This function will be threadsafe
 * because we us an explicitly scoped lock_guard
 */
void Logger::clearlogfile() {
	{
		std::lock_guard<std::mutex> lock(m);

		myFile.open(filename, std::fstream::trunc);

		if (myFile.is_open()) {
			myFile.close();
		}
	}
}

/*
 * This function will be threadsafe
 * because we us an explicitly scoped lock_guard
 */
void Logger::log(std::string data) {

	{
		lock_guard<mutex> lock(m);

		myFile.open(filename, std::fstream::app);
		if (!myFile.is_open()) {
			return;
		}

		std::string myline;

		myFile << data;

		if (myFile.is_open()) {
			myFile.close();
		}
	}
}
