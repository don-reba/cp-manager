#pragma once

#include <string>

class IProtocol;

class Admin {
	public:
		Admin(IProtocol & protocol);

	public:
		void exit();
		void loadHandler(const std::string & handlerName);

	private:
		IProtocol & protocol;
};
