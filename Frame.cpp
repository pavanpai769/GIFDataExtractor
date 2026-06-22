#pragma once
#include<fstream>
#include<vector>
#include<optional>
#include"ImageDescriptor.cpp"
#include"GCE.cpp"
#include"ColorTable.cpp"


class Frame {
private:
	ImageDescriptor imageDescriptor;
	std::optional<GCE> gce;
	std::optional<ColorTable> lct;
	uint8_t minimumCodeSize;
	std::vector<uint8_t> compressedData;

public:
	Frame(const ImageDescriptor& imageDescriptor,
		std::optional<GCE> gce,
		std::optional<ColorTable> lct,
		uint8_t minimumCodeSize,
		const std::vector<uint8_t>& compressedData
	) : imageDescriptor(imageDescriptor),
		gce(gce),
		lct(lct),
		minimumCodeSize(minimumCodeSize),
		compressedData(compressedData) {}


	const ImageDescriptor& getImageDescriptor() const {
		return imageDescriptor;
	}

	const std::optional<GCE>& getGce() const{
		return gce;
	}

	const std::optional<ColorTable>& getLct() const{
		return lct;
	}

	uint8_t getMinimumCodeSize() const {
		return minimumCodeSize;
	}

	const std::vector<uint8_t>& getCompressedData() const{
		return compressedData;
	}

	bool hasGce() const {
		return gce.has_value();
	}

	bool hasLct() const {
		return lct.has_value();
	}

	const std::string toString() const {
		std::string str;

		str += imageDescriptor.toString() + "\n";
		
		if (hasGce()) {
			str += gce.value().toString() + "\n";
		}

		if (hasLct()) {
			str += lct.value().toString() + "\n";
		}

		str += std::format("minimumCodeSize: {}" ,minimumCodeSize);

		for (uint8_t data : compressedData) {
			str += std::to_string(data) + " ";
		}

		return std::string(str);
	}

};






