#include "Message.h"

namespace messages
{
	std::ostream& PrintMessage(const std::string& message_content, const std::string& File /*which File sends the Message ?*/, MessageTypes Type, bool show_file)
	{
		if (Type == MessageTypes::notification)
		{
			std::cout << "[INFO] " << message_content;

			if (show_file)
			{
				std::cout << " ( " << File << " )" << std::endl;
			}
			else
			{
				std::cout << "\n";
			}
			return std::cout;
		}
		if (Type == MessageTypes::important_notification)
		{
			std::cout << "\033[0;34m[INFO] " << message_content;

			if (show_file)
			{
				std::cout << " ( " << File << " )" << "\033[0m" << std::endl;
			}
			else
			{
				std::cout << "\033[0m" << std::endl;
			}
			return std::cout;
		}
		if (Type == MessageTypes::warning)
		{
			std::cout << "\033[1;33m[WARNING] " << message_content;

			if (show_file)
			{
				std::cout << " ( " << File << " )" << "\033[0m" << std::endl;
			}
			else
			{
				std::cout << "\033[0m" << std::endl;
			}
			return std::cout;
		}
		if (Type == MessageTypes::error)
		{
			std::cerr << "\033[1;31m[ERROR] " << message_content;

			if (show_file)
			{
				std::cerr << " ( " << File << " )" << "\033[0m" << std::endl;
			}
			else
			{
				std::cerr << "\033[0m" << std::endl;
			}
			return std::cerr;
		}
		if (Type == MessageTypes::fatal_error)
		{
			std::cerr << "\033[38;5;88m[FATAL_ERROR] " << message_content;

			if (show_file)
			{
				std::cerr << " ( " << File << " )" << "\033[0m" << std::endl;
			}
			else
			{
				std::cerr << "\033[0m" << std::endl;
			}
			return std::cerr;
		}
		if (Type == MessageTypes::engine_core_info)
		{
			std::cout << "\033[32m[CORE] " << message_content << "\033[0m" << std::endl;
		}
		return std::cout;
	};
}