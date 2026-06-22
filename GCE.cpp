#pragma once
#include<cstdint>
#include<string>
#include<format>

class GCE {
public:
	class PackedFlag {
	private:
		uint8_t disposal;
		bool transparentColorExists;

	public:
		PackedFlag(uint8_t disposal, bool transparentColorExists)
			: disposal(disposal),
			transparentColorExists(transparentColorExists) {
		}

		uint8_t getDisposal() const {
			return disposal;
		}

		bool isTrasparentColorExsists() const {
			return transparentColorExists;
		}

		const std::string toString() const {
			return std::format("	disposal: {}\n		transparentColorExists: {}\n",disposal,transparentColorExists);
		}


	};

	const PackedFlag& getPackedFlag() const{
		return packedFlag;
	}

	uint16_t getDelay() const {
		return delay;
	}

	uint8_t getTransparentColor() const {
		return transparentColorIndex;
	}

	const std::string toString() const {
		return std::format("packedFlag: {} \ndelay: {} \ntransparentColorIndex: {}\n", packedFlag.toString(), delay, transparentColorIndex);
	}


	GCE(const PackedFlag& packedFlag, uint16_t delay, uint8_t transparentColorIndex)
		: packedFlag(packedFlag),
		delay(delay),
		transparentColorIndex(transparentColorIndex) {
	}

private:
	PackedFlag packedFlag;
	uint16_t delay;
	uint8_t transparentColorIndex;
};