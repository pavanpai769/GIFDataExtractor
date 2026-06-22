#pragma once
#include<string>
#include<optional>
#include"LogicalDescriptor.cpp"
#include"ApplicationData.cpp"
#include"Frame.cpp"
class GIF {
private:
	std::string header;
	LogicalDescriptor logicalDescriptor;
	std::optional<ColorTable> gct;
	std::optional<ApplicationData> applicationData;
	std::vector<Frame> frames;

public:

	GIF(
		const std::string& header,
		const LogicalDescriptor& logicalDescriptor,
		std::optional<ColorTable> gct,
		std::optional<ApplicationData> applicationData,
		const std::vector<Frame>& frames
	) :	header(header),
		logicalDescriptor(logicalDescriptor),
		gct(gct),
		applicationData(applicationData),
		frames(frames){

		if (header != "GIF87a" && header != "GIF89a") {
			throw std::runtime_error("Wrong GIF header");
		}
	}

	const std::string& getHeader() const {
		return header;
	}

	const LogicalDescriptor& getLogicalDescriptor() const {
		return logicalDescriptor;
	}

	bool hasGct() const {
		return gct.has_value();
	}

	const std::optional<ColorTable>& getGct() const {
		return gct;
	}

	bool hasApplicationData() const {
		return applicationData.has_value();
	}

	const std::optional<ApplicationData>& getApplicationData() const{
		return applicationData;
	}


	const std::vector<Frame>& getFrames() const{
		return frames;
	}

};