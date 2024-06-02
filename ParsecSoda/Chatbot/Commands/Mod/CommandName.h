#pragma once

#include "parsec-dso.h"
#include "../Base/ACommandStringArg.h"

class CommandName : public ACommandStringArg
{
public:
	
	/**
	 * @brief Construct a new CommandName object
	 * 
	 * @param msg
	 * @param config
	 */
	CommandName(const char* msg, ParsecHostConfig &config)
		: ACommandStringArg(msg, internalPrefixes()), _config(config)
	{}

	/**
	 * @brief Run the command
	 * 
	 * @return true
	 * @return false
	 */
	bool run() override {
		if ( !ACommandStringArg::run() ) {
			SetReply("Usage: !name <roomname>\nExample: !name Let's Play Gauntlet!\0");
			return false;
		}

		// Replaces \ + n with \n
		size_t index = 0;
		while (true) {
			index = _stringArg.find("\\n", index);
			if (index == std::string::npos) break;

			_stringArg.replace(index, 2, "\n");
			index++;
		}

		strcpy_s(_config.name, _stringArg.c_str());

		SetReply("Room name changed:\n" + _stringArg + "\0");
		return true;
	}

	/**
	 * @brief Get the prefixes object
	 * 
	 * @return std::vector<const char*>
	 */
	static vector<const char*> prefixes() {
		return vector<const char*> { "!name" };
	}

protected:


	static vector<const char*> internalPrefixes() {
		return vector<const char*> { "!name " };
	}
	ParsecHostConfig& _config;
};
