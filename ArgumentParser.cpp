#include "ArgumentParser.h"

#include <iostream>
#include <vector>
#include <exception>


ArgumentParser::ArgumentParser(int argc, char const* argv[]) {
	for (size_t i = 0; i < static_cast<size_t>(argc); ++i) {
		keys_.push_back(argv[i]);
	}
}

bool ArgumentParser::IsEncoder() const {
	return Contains("-c");
}

bool ArgumentParser::IsDecoder() const {
	return Contains("-d");
}

bool ArgumentParser::IsHelp() const {
	return Contains("-h");
}

bool ArgumentParser::Contains(const std::string& argument) const {
	return std::find(keys_.begin(), keys_.end(), argument) != keys_.end();
}

std::vector<std::string> ArgumentParser::GetFilesToArchive() const {
	std::vector<std::string> files_to_archive;
	for (size_t i = 0; i < keys_.size(); ++i) {
		if (keys_[i] == "-c") {
			for (size_t j = i + 2; j < keys_.size(); ++j) {
				files_to_archive.push_back(keys_[j]);
			}
		}
	}
	if (files_to_archive.empty()) {
		throw std::runtime_error("Error: There are no files to archive");
	}
	return files_to_archive;
}

std::string ArgumentParser::GetArchiveName() const {
	for (size_t i = 0; i < keys_.size() - 1; ++i) {
		if (keys_[i] == "-c" || keys_[i] == "-d") {
			return keys_[i + 1];
		}
	}
	throw std::runtime_error("Error: No archiver name specified");
}
