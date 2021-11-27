#pragma once

#include <fstream>
#include <istream>

class Reader
{
public:
	Reader() = default;
	Reader(std::istream& inf);
	Reader(const std::string& filename);

	void SetInputStream(std::istream& inf);
	void SetInputFileStream(std::string filename);

	bool ReadBit(std::istream& inf);
	bool ReadBit();
	std::string ReadNBits(size_t n, std::istream& inf);
	std::string ReadNBits(size_t n);

	bool HasSomethingToRead();
private:
	std::istream* input_stream_ = nullptr;
	unsigned char buffer_ = 0;
	size_t buffer_size_ = 0;
};
