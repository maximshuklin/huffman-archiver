#pragma once

#include "Trie.h"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

const size_t FILENAME_END = 256;
const size_t ONE_MORE_FILE = 257;
const size_t ARCHIVE_END = 258;


class Encoder
{
public:
	Encoder();

	void CompressFilesIntoArchive(const std::vector<std::string>& filenames, const std::string& archive_name);

	void AddFileForPrecalculation(const std::string& filename);

	void AddInputStreamForPrecalculation(std::ifstream& input_stream);

private:
	std::unordered_map<Symbol, size_t> symbols_count_;
};
