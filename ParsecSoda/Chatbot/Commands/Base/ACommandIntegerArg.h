#pragma once

#include "ACommandPrefix.h"

class ACommandIntegerArg : public ACommandPrefix
{
public:

	/**
	 * @brief Construct a new ACommandIntegerArg object
	 * 
	 * @param msg The message to parse
	 * @param prefixes The prefixes to check for
	 */
	ACommandIntegerArg(const char* msg, vector<const char*> prefixes)
		: ACommandPrefix(msg, prefixes), _intArg(0)
	{}

	/**
	 * @brief Get the integer argument
	 */
	bool run() override {
		if (!ACommandPrefix::run()) {
			return false;
		}

		try {
			std::string str = _msg;
			size_t cmdSize = strlen(_prefix);
			_intArg = std::stoi(str.substr(cmdSize));

			return true;
			
		} catch (const std::exception&) {
			return false;
		}
	}

protected:
	int _intArg;
};
