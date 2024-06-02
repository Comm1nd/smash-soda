#pragma once

#include "ACommandPrefix.h"
#include "parsec-dso.h"
#include "../../../GuestList.h"
#include "../../../GuestDataList.h"

enum class SEARCH_USER_HISTORY_RESULT
{
	FAILED = -1,
	NOT_FOUND = 0,
	ONLINE = 1,
	OFFLINE = 2
};

class ACommandSearchUserHistory : public ACommandPrefix
{
public:

	/**
	 * @brief Construct a new ACommandSearchUserHistory object
	 * 
	 * @param msg The message to parse
	 * @param prefixes The prefixes to check for
	 * @param guests The list of online guests
	 * @param guestHistory The list of offline guests
	 */
	ACommandSearchUserHistory(const char* msg, vector<const char*> prefixes, GuestList &guests, GuestDataList &guestHistory)
		: ACommandPrefix(msg, prefixes), _guests(guests), _guestHistory(guestHistory),
		_searchResult(SEARCH_USER_HISTORY_RESULT::FAILED), _onlineGuest(Guest()), _offlineGuest(GuestData())
	{}

	/**
	 * @brief Get the search result
	 * @return The search result
	 */
	bool run() override {
		if (!ACommandPrefix::run())
		{
			_searchResult = SEARCH_USER_HISTORY_RESULT::FAILED;
			return false;
		}

		try
		{
			_searchResult = SEARCH_USER_HISTORY_RESULT::NOT_FOUND;

			std::string str = _msg;
			size_t cmdSize = strlen(_prefix);
			_targetUsername = str.substr(cmdSize);

			bool found = false;
			try
			{
				uint32_t userID = stoul(_targetUsername);

				found = _guests.find(userID, &_onlineGuest);
				if (found)
				{
					_searchResult = SEARCH_USER_HISTORY_RESULT::ONLINE;
				}
				else
				{
					found = _guestHistory.find(userID, [&](GuestData& guest) { _offlineGuest = guest; });
					_searchResult = found ? SEARCH_USER_HISTORY_RESULT::OFFLINE : SEARCH_USER_HISTORY_RESULT::NOT_FOUND;
				}
			}
			catch (const std::exception&) {}

			if (!found)
			{
				found = _guests.find(_targetUsername, &_onlineGuest);
				if (found)
				{
					_searchResult = SEARCH_USER_HISTORY_RESULT::ONLINE;
				}
				else
				{
					found = _guestHistory.find(_targetUsername, [&](GuestData& guest) { _offlineGuest = guest; });
					_searchResult = found ? SEARCH_USER_HISTORY_RESULT::OFFLINE : SEARCH_USER_HISTORY_RESULT::NOT_FOUND;
				}
			}

			return found;
		}
		catch (const std::exception&)
		{
			_searchResult = SEARCH_USER_HISTORY_RESULT::FAILED;
			return false;
		}
	}

protected:
	GuestDataList& _guestHistory;
	SEARCH_USER_HISTORY_RESULT _searchResult;
	GuestList& _guests;
	Guest _onlineGuest;
	GuestData _offlineGuest;
	string _targetUsername;
};

