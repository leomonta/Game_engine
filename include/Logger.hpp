#include <iostream>
#define LOG(lvl, mex) Logger::Log(Logger::lvl, mex)

class Logger {
public:
	enum Levels { Info = 0,
				  Warning,
				  Error };

	// remove constructor and copy constructor
	Logger()			   = delete;
	Logger(Logger &logger) = delete;

	static void Log(enum Levels level, const char *message) {

		switch (level) {
		case Info:
			std::cout << "[INFO]: ";
			break;

		case Warning:
			std::cout << "[WARNING]: ";
			break;

		case Error:
			std::cout << "[ERROR]: ";
			break;

		default:
			break;
		}

		std::cout << message << std::endl;
	}

	static void Log(enum Levels level, std::string *message) {
		Log(level, message->c_str());
	}
};
