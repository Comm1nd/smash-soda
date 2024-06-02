#pragma once

#include "ACommandPrefix.h"
#include "parsec-dso.h"
#include "../../../Helpers/Stringer.h"
#include "../../../GuestList.h"

enum class SEARCH_USER_RESULT
{
	FAILED = -1,
	NOT_FOUND = 0,
	FOUND = 1
};

class ACommandSearchUser : public ACommandPrefix
{
public:

	/**
	 * @brief Constructor
	 * @param msg The message to parse
	 * @param prefixes The prefixes to check for
	 * @param guests The list of guests
	 */
	ACommandSearchUser(const char* msg, vector<const char*> prefixes, GuestList &guests)
		: ACommandPrefix(msg, prefixes), _guests(guests),
		_searchResult(SEARCH_USER_RESULT::FAILED), _targetGuest(Guest())
	{}

	/**
	 * @brief Get the search result
	 * @return The search result
	 */
	bool run() override {
		if (!ACommandPrefix::run())
		{
			_searchResult = SEARCH_USER_RESULT::FAILED;
			return false;
		}

		try
		{
			std::string str = _msg;
			size_t cmdSize = strlen(_prefix);
			_targetUsername = str.substr(cmdSize);

			bool found = false;
			try
			{
				found = _guests.find(stoul(_targetUsername), &_targetGuest);
			}
			catch (const std::exception&) {}

			if (!found)
			{
				found = _guests.find(_targetUsername, &_targetGuest);
			}
			
			_searchResult = found ? SEARCH_USER_RESULT::FOUND : SEARCH_USER_RESULT::NOT_FOUND;
			return found;
		}
		catch (const std::exception&)
		{
			_searchResult = SEARCH_USER_RESULT::FAILED;
			return false;
		}
	}

protected:
	SEARCH_USER_RESULT _searchResult;
	GuestList& _guests;
	Guest _targetGuest;
	string _targetUsername;
};

