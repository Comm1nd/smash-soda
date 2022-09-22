#pragma once

#include <iostream>
#include <sstream>
#include "ACommand.h"
#include "../GamepadClient.h"
#include "../Guest.h"

class CommandFF : public ACommand
{
public:
	const COMMAND_TYPE type() override { return COMMAND_TYPE::FF; }

	CommandFF(Guest &sender, GamepadClient &gamepadClient)
		: _sender(sender), _gamepadClient(gamepadClient), _droppedPadCount(0)
	{}

	bool run() override
	{
		_droppedPadCount = _gamepadClient.onQuit(_sender);

		std::ostringstream reply;
		if (_droppedPadCount > 1)
		{
			reply << "[MasterHand] | " << _sender.name << " has dropped " << _droppedPadCount << " gamepads!\0";

			if (MetadataCache::preferences.hotseat) MetadataCache::preferences.hotseatReset = true;
		}
		else if (_droppedPadCount > 0)
		{
			reply << "[MasterHand] | " << _sender.name << " has dropped " << _droppedPadCount << " gamepad!\0";

			if (MetadataCache::preferences.hotseat) MetadataCache::preferences.hotseatReset = true;
		}
		else
		{
			reply <<  "[MasterHand] | " << _sender.name << " has no gamepads to drop.\0";
		}

		_replyMessage = reply.str();
		reply.clear();

		return true;
	}

	int droppedPadCount()
	{
		return _droppedPadCount;
	}

	static vector<const char*> prefixes()
	{
		return vector<const char*> { "!ff", "!drop", "!quit" };
	}

protected:
	Guest& _sender;
	GamepadClient &_gamepadClient;
	int _droppedPadCount;
};