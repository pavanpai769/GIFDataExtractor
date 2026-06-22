#pragma once
#include<cstdint>
#include<string>
#include<format>

class LogicalDescriptor {

public:
	class PackedFlag {
	private:
		bool gctExists;
		uint8_t colorResolution;
		bool gctSorted;
		uint8_t gctSizeCode;

	public:
		PackedFlag(bool gctExists, uint8_t colorResolution, bool gctSorted, uint8_t gctSizeCode)
			: gctExists(gctExists),
			colorResolution(colorResolution),
			gctSorted(gctSorted),
			gctSizeCode(gctSizeCode) {
		}

		bool isGctExists() const {
			return gctExists;
		}

		uint8_t getColorResolution() const {
			return colorResolution;
		}

		bool isGctSorted() const {
			return gctSorted;
		}

		uint8_t getGctSizeCode() const {
			return gctSizeCode;
		}

		const std::string toString() const {
			return std::format("	gctExists: {}\n		colorResolution: {}\n	gctSorted: {}\n		gctSizeCode: {} \n\n",gctExists,colorResolution,gctSorted,gctSizeCode);
		}

	};


	LogicalDescriptor(uint16_t width, uint16_t height, const PackedFlag& packedFlag, uint8_t bgIndex, uint8_t pixelAspectRatio)
		: width(width),
		height(height),
		packedFlag(packedFlag),
		bgIndex(bgIndex),
		pixelAspectRatio(pixelAspectRatio) {
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

	uint8_t getBgIndex() const {
		return bgIndex;
	}

	uint8_t getPixelAspectRatio() const {
		return pixelAspectRatio;
	}

	const std::string toString() const {
		return std::format("width: {}\nheight: {} \npackedFlag: {} \nBgIndex: {} \nPixelAspectRatio:\n{} \n\n",width,height,packedFlag.toString(),bgIndex,pixelAspectRatio);
	}

private:
	uint16_t width;
	uint16_t height;
	PackedFlag packedFlag;
	uint8_t bgIndex;
	uint8_t pixelAspectRatio;


};