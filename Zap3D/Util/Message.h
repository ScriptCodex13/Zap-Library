#pragma once

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <iostream>

enum class MessageTypes
{
	None = 0,
	notification,
	important_notification,
	warning,
	error,
	fatal_error,
	api_core_info
};

namespace messages
{
	std::ostream& PrintMessage(const std::string& message_content, const std::string& File /*which File sends the Message ?*/, MessageTypes Type, bool show_file = true);
}

#endif