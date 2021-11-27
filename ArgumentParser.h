#pragma once

#include <vector>
#include <string>

class ArgumentParser
{
public:
	ArgumentParser() = default;
	ArgumentParser(int argc, char const* argv[]);

	bool IsEncoder() const;
	bool IsDecoder() const;
	bool IsHelp() const;
	std::vector<std::string> GetFilesToArchive() const;
	std::string GetArchiveName() const;

private:
	std::vector<std::string> keys_;

	bool Contains(const std::string& argument) const;
};
