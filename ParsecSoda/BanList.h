#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include "Models/GuestData.h"
#include "GuestDataList.h"
#include "MetadataCache.h"

class BanList : GuestDataList
{
public:
	BanList();
	BanList(const std::vector<GuestData> bannedUsers);
	bool ban (const GuestData user);
	const bool unban(const uint32_t userID, function<void(GuestData&)> callback);
	const bool unban(string guestName, function<void(GuestData&)> callback);
	const bool isBanned(const uint32_t userID);
	vector<GuestData>& getGuests();
};

