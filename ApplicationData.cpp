#pragma once
#include<string>
#include<format>

class ApplicationData {
private:
	std::string asciiNameAndVersion;
	uint16_t loopCount;

public:
	ApplicationData(const std::string& asciiNameAndVersion, uint16_t loopCount)
		: asciiNameAndVersion(asciiNameAndVersion),
		loopCount(loopCount) {
	}

	const std::string& getAsciiNameAndVersion() const {
		return asciiNameAndVersion;
	}

	uint16_t getLoopCount() const {
		return loopCount;
	}

	const std::string toString() const {
		return std::format("asciiNameAndVersion: {} \nloopCount: {}\n", asciiNameAndVersion, loopCount);
	}
};