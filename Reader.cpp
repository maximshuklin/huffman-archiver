#include "Reader.h"

Reader::Reader(std::istream& inf) {
	SetInputStream(inf);
}

Reader::Reader(const std::string& filename) {
	SetInputFileStream(filename);
}

void Reader::SetInputStream(std::istream& inf) {
	input_stream_ = static_cast<std::istream*>(&inf);
}

void Reader::SetInputFileStream(std::string filename) {
	std::ifstream input_file_stream(filename) ;
	input_stream_ = static_cast<std::istream*>(&input_file_stream);
	input_file_stream.close();
}

bool Reader::ReadBit(std::istream& inf) {
	if (buffer_size_ == 0) {
		buffer_ = inf.get();
		buffer_size_ = 8;
	}

	bool bit_value = buffer_ & 1;
	buffer_ >>= 1;
	--buffer_size_;
	return bit_value;
}

bool Reader::ReadBit() {
	return ReadBit(*input_stream_);
}

std::string Reader::ReadNBits(size_t n, std::istream& inf) {
	std::string str = "";
	for (size_t i = 0; i < n; ++i) {
		bool bit = ReadBit(inf);
		str += static_cast<char>('0' + bit);
	}
	return str;
}

std::string Reader::ReadNBits(size_t n) {
	std::string str = "";
	for (size_t i = 0; i < n; ++i) {
		bool bit = ReadBit();
		str += static_cast<char>('0' + bit);
	}
	return str;
}

bool Reader::HasSomethingToRead() {
	if (buffer_size_ > 0) {
		return true;
	}
	buffer_ = (*input_stream_).get();
	buffer_size_ = 8;
	if (!input_stream_->eof()) {
		return true;
	}
	return false;
}
