#pragma once
#include<cstdint>
#include<string>
#include<format>

class ImageDescriptor {

public:
	class PackedFlag {
	private:
		bool lctExists;
		bool interlaced;
		bool sorted;
		uint8_t lctSizeCode;

	public:
		PackedFlag(bool lctExists, bool interlaced, bool sorted, uint8_t lctSizeCode)
			: lctExists(lctExists),
			interlaced(interlaced),
			sorted(sorted),
			lctSizeCode(lctSizeCode) {
		}

		bool isLctExists() const {
			return lctExists;
		}

		bool isInterlaced() const {
			return interlaced;
		}

		bool isSorted() const {
			return sorted;
		}

		uint8_t getLctSizeCode() const {
			return lctSizeCode;
		}

		const std::string toString() const{
			return std::format("	lctExists: {}\n		interlaced:	{}\n	sorted: {}\n	lctSizeCode: {}\n", lctExists, interlaced, sorted, lctSizeCode);
		}

	};


	ImageDescriptor(
		uint16_t left,
		uint16_t top,
		uint16_t width,
		uint16_t height,
		const PackedFlag& packedFlag
	) : left(left),
		top(top),
		width(width),
		height(height),
		packedFlag(packedFlag) {
	}

	uint16_t getLeft() const {
		return left;
	}

	uint16_t getTop() const {
		return top;
	}

	uint16_t getWidth() const {
		return width;
	}

	uint16_t getHeight() const {
		return height;
	}

	const PackedFlag& getPackedFlag() const {
		return packedFlag;
	}

	const std::string toString() const {
		return std::format("left: {} \ntop: {} \nwidth: {}\n height: {}\n packedFlag: {}",left,top,width,height,packedFlag.toString());
	}

private:
	uint16_t left;
	uint16_t top;
	uint16_t width;
	uint16_t height;
	PackedFlag packedFlag;
};