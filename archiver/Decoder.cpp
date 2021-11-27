#include "Decoder.h"
#include "Reader.h"
#include "Trie.h"
#include "Encoder.h"

#include <istream>
#include <vector>
#include <fstream>

Decoder::Decoder(std::istream& inf) : input_stream_(inf) {}

size_t ConvertStringToSizeT(const std::string str) {
	size_t converted_size_t = 0;
	for (size_t i = 0; i < str.length(); ++i) {
		converted_size_t += (1u << i) * static_cast<size_t>(str[i] - '0');
	}
	return converted_size_t;
}


void Decoder::Decode() {

	Reader reader = Reader(input_stream_);

	size_t SYMBOLS_COUNT = 0;
	std::string kek = reader.ReadNBits(9);

	SYMBOLS_COUNT = ConvertStringToSizeT(kek);

	std::vector<Symbol> cannonical_symbols;

	for (size_t i = 0; i < SYMBOLS_COUNT; ++i) {
		size_t symbol = ConvertStringToSizeT(reader.ReadNBits(9));
		cannonical_symbols.push_back(symbol);
	}

	std::vector<size_t> lengths;
	size_t sum_cnt_symbols = 0;
	size_t current_length = 1;
	while (sum_cnt_symbols < SYMBOLS_COUNT) {
		if (!reader.HasSomethingToRead()) {
			throw std::runtime_error("Input archive is not supported by this program");
		}

		size_t cnt_symbols_with_current_length = ConvertStringToSizeT(reader.ReadNBits(9));
		for (size_t i = 0; i < cnt_symbols_with_current_length; ++i) {
			lengths.push_back(current_length);
		}
		++current_length;
		sum_cnt_symbols += cnt_symbols_with_current_length;
	}

	std::vector<CannonicalCode> cannonical_pairs;
	for (size_t i = 0; i < cannonical_symbols.size(); ++i) {
		cannonical_pairs.push_back({cannonical_symbols[i], lengths[i]});
	}

	Trie trie = Trie(cannonical_pairs);

	bool have_to_stop = false;

	while (!have_to_stop && reader.HasSomethingToRead()) {
		trie.GoToRoot();

		std::string filename;

		while (reader.HasSomethingToRead()) {
			bool bit = reader.ReadBit();
			trie.Descent(bit);
			if (trie.IsLeaf()) {
				size_t ch = trie.GetSymbol();
				if (ch == FILENAME_END) {
					break;
				}
				trie.GoToRoot();

				filename += static_cast<char>(ch);
			}
		}

		std::ofstream outf(filename);
		trie.GoToRoot();
		while (reader.HasSomethingToRead()) {
			bool bit = reader.ReadBit();
			trie.Descent(bit);
			if (trie.IsLeaf()) {
				size_t ch = trie.GetSymbol();
				if (ch == ONE_MORE_FILE) {
					break;
				} else if (ch == ARCHIVE_END) {
					have_to_stop = true;
					break;
				}
				trie.GoToRoot();
				outf << static_cast<char>(ch);
			}
		}
		outf.close();
	}
}
