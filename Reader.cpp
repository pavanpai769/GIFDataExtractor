#pragma once
#include<fstream>
#include<vector>
#include <stdexcept>

class Reader {
private:
	std::ifstream file;

public:
	Reader(const std::string& path) {
		file.open(path, std::ios::binary);
		if (!file.is_open()) throw std::runtime_error("cannot open file: " + path);
	}

	Reader(const Reader&) = delete;            
	Reader& operator=(const Reader&) = delete; 

	uint8_t readByte() {
		uint8_t byte;

		if (!file.read(reinterpret_cast<char*>(&byte), 1)) {
			throw std::runtime_error("some error occured while reading file");
		}

		return byte;
	}

	uint16_t readUInt16_le() {
		uint8_t low = readByte();
		uint8_t high = readByte();

		return (uint16_t)(low | (high << 8));
	}

	std::vector<uint8_t> readBytes(size_t size) {
		std::vector<uint8_t> buffer(size);

		if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
			throw std::runtime_error("some error occured while reading file");
		}

		return buffer;
	}


};