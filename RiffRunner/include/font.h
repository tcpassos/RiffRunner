#pragma once

#include <string>

class Font
{
public:
	std::string source;
	unsigned int size;

	Font(std::string source, unsigned int size = 52) {
		this->source = source;
		this->size = size;
	}
};