#include "Writer.h"

Writer::Writer(std::ostream& outf) {
	SetOutputStream(outf);
}

void Writer::SetOutputStream(std::ostream& outf) {
	output_stream_ = static_cast<std::ostream*>(&outf);
}

void Writer::WriteBit(bool bit, std::ostream& outf) {
	if (buffer_size_ == 8) {
		outf << buffer_;
		buffer_ = 0;
		buffer_size_ = 0;
	}
	buffer_ += (1 << buffer_size_) * bit;
	++buffer_size_;
}

void Writer::WriteBit(bool bit) {
	WriteBit(bit, *output_stream_);
}

void Writer::Write(const std::string& str, std::ostream& outf) {
	for (size_t i = 0; i < str.size(); ++i) {
		WriteBit(static_cast<bool>(str[i] - '0'), outf);
	}
}

void Writer::Write(const std::string& str) {
	Write(str, *output_stream_);
}

void Writer::Write(size_t number, size_t length, std::ostream& outf) {
	for (size_t i = 0; i < length; ++i) {
		WriteBit((number >> i) & 1, outf);
	}
}

void Writer::Write(size_t number, size_t length) {
	Write(number, length, *output_stream_);
}


void Writer::Flush(std::ostream& outf) {
	if (buffer_size_ > 0) {
		outf << buffer_;
		buffer_ = 0;
		buffer_size_ = 0;
	}
}

void Writer::Flush() {
	Flush(*output_stream_);
}