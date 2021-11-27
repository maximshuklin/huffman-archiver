#include "Encoder.h"
#include "Writer.h"

#include <fstream>
#include <iostream>
#include <string>

Encoder::Encoder() {
	++symbols_count_[FILENAME_END];
	++symbols_count_[ONE_MORE_FILE];
	++symbols_count_[ARCHIVE_END];
}

void Encoder::CompressFilesIntoArchive(const std::vector<std::string>& filenames, const std::string& archive_name) {
	for (const auto& filename : filenames) {
		AddFileForPrecalculation(filename);
	}

	Trie huffman_trie = Trie(symbols_count_);
	std::vector<CannonicalCode> cannonical_pairs = huffman_trie.BuildCannonicalHuffman();

	std::unordered_map<Symbol, std::string> symbols_code;

	GetSymbolsCodes(cannonical_pairs, symbols_code);

	size_t SYMBOLS_COUNT = symbols_count_.size();
	size_t MAX_SYMBOL_CODE_SIZE = 0;

	std::unordered_map<size_t, size_t> number_codes_by_length;

	for (auto [symbol, code] : symbols_code) {
		MAX_SYMBOL_CODE_SIZE = std::max(MAX_SYMBOL_CODE_SIZE, code.size());
		++number_codes_by_length[code.size()];
	}

	std::ofstream to_archiv_stream = std::ofstream(archive_name);
	Writer writer = Writer(to_archiv_stream);

	writer.Write(SYMBOLS_COUNT, 9);

	for (size_t i = 0; i < SYMBOLS_COUNT; ++i) {
		writer.Write(cannonical_pairs[i].symbol, 9);
	}

	for (size_t i = 1; i <= MAX_SYMBOL_CODE_SIZE; ++i) {
		writer.Write(number_codes_by_length[i], 9);
	}

	for (size_t i = 0; i < filenames.size(); ++i) {
		std::string filename = filenames[i];
		for (const auto& symbol : filename) {
			writer.Write(symbols_code[symbol]);
		}
		writer.Write(symbols_code[FILENAME_END]);

		unsigned char ch;
		std::ifstream input_stream(filename);
		while (!input_stream.eof()) {
			ch = input_stream.get();
			if (input_stream.eof()) {
				break;
			}
			writer.Write(symbols_code[ch]);
		}
		if (i + 1 < filenames.size()) {
			writer.Write(symbols_code[ONE_MORE_FILE]);
		}
	}
	writer.Write(symbols_code[ARCHIVE_END]);
	writer.Flush();
	to_archiv_stream.close();
}

void Encoder::AddFileForPrecalculation(const std::string& filename) {
	std::ifstream inf(filename);
	AddInputStreamForPrecalculation(inf);
	inf.close();

	for (const auto& ch : filename) {
		++symbols_count_[ch];
	}
}

void Encoder::AddInputStreamForPrecalculation(std::ifstream& input_stream) {
	unsigned char ch;
	while (!input_stream.eof()) {
		ch = input_stream.get();
		if (input_stream.eof()) {
			break;
		}
		++symbols_count_[ch];
	}
}
