#pragma once

#include <ostream>
#include <fstream>

class Writer
{
public:
	Writer(std::ostream& outf);

	void SetOutputStream(std::ostream& outf);

	void WriteBit(bool bit, std::ostream& outf);
	void WriteBit(bool bit);

	void Write(const std::string& str, std::ostream& outf);
	void Write(const std::string& str);

	void Write(size_t number, size_t length, std::ostream& outf);
	void Write(size_t number, size_t length);

	void Flush(std::ostream& outf);
	void Flush();

private:
	std::ostream* output_stream_ = nullptr;
	char buffer_ = 0;
	size_t buffer_size_ = 0;
};