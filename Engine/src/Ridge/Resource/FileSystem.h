#pragma once

#include <Ridge/Common.h>
#include <Ridge/Types.h>

#include <string>
#include <fstream>

namespace Ridge {
	class FileSystem {
	public:
		std::string ReadTextFile(const std::string& vPath);
		uint8* ReadBinaryFile(const std::string& vPath);
		size_t ReadBinaryFile(const std::string& vPath, uint8* buffer, size_t length);

		size_t WriteTextFile(const std::string& vPath, const std::string& text);
		size_t WriteBinaryFile(const std::string& vPath, uint8* buffer, size_t length);

	private:
	};
}