#pragma once

#include <string>
#include <istream>

class Decoder {
public:
	Decoder(std::istream& inf); 

	void Decode();

private:
	std::istream& input_stream_;
};
