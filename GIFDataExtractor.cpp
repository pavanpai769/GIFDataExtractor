#pragma once
#include"Reader.cpp"
#include"GIF.cpp"
class GIFDataExtractor {
private:
	Reader reader;

	std::string readHeader() {
		std::vector<uint8_t> header = reader.readBytes(6);
		return std::string(header.begin(), header.end());
	}

	LogicalDescriptor readLogicalDescriptor() {

		uint16_t width = reader.readUInt16_le();
		uint16_t height = reader.readUInt16_le();
		uint8_t packed = reader.readByte();
		uint8_t bgColorIndex = reader.readByte();
		uint8_t pixelAspectRatio = reader.readByte();

		bool isGctExists = (packed & 0x80) >> 7;
		uint8_t colorResolution = (packed & 0x70) >> 4;
		bool isGctSorted = (packed & 0x8) >> 3;
		uint8_t gctSize = (packed & 0x7);

		LogicalDescriptor::PackedFlag packedFlag(
			isGctExists,
			colorResolution,
			isGctSorted,
			gctSize
		);

		return LogicalDescriptor(
			width,
			height,
			packedFlag,
			bgColorIndex,
			pixelAspectRatio
		);
	}

	ColorTable readColorTable(int gctSize) {

		std::vector<ColorTable::Color> colors;
		for (int i = 0; i < gctSize;i++) {
			ColorTable::Color color;
			color.r = reader.readByte();
			color.g = reader.readByte();
			color.b = reader.readByte();

			colors.push_back(color);
		}
		
		ColorTable gct(colors);

		return gct;
	}

	ApplicationData readApplicationData() {
		reader.readByte(); // read the size, always 11 so ignore

		std::vector<uint8_t> asciiNameAndVersionBuffer = reader.readBytes(11);
		uint16_t loopCount = 0;

		std::string asciiNameAndVersion(asciiNameAndVersionBuffer.begin(), asciiNameAndVersionBuffer.end());

		if (asciiNameAndVersion == "NETSCAPE2.0") {

			reader.readByte(); // sub block size, ignore it.
			reader.readByte(); // sub block id, ignore

			loopCount = reader.readUInt16_le();

			reader.readByte();
			
			// terminator, ignore
		}
		else {
			// not netscape, ignore the sub blocks
			uint8_t subBlockSize;
			while ((subBlockSize = reader.readByte()) != 0) {
				reader.readBytes(subBlockSize);
			}
		}

		return ApplicationData(asciiNameAndVersion, loopCount);
	}

	ImageDescriptor readImageDescriptor() {

		uint16_t left = reader.readUInt16_le();
		uint16_t top = reader.readUInt16_le();
		uint16_t width = reader.readUInt16_le();
		uint16_t height = reader.readUInt16_le();

		uint8_t packed = reader.readByte();

		bool    hasLct = (packed >> 7) & 0x01;
		bool    interlaced = (packed >> 6) & 0x01;
		bool    sorted = (packed >> 5) & 0x01;
		uint8_t lctSize = (packed >> 0) & 0x07;



		ImageDescriptor::PackedFlag packedFlag(
			hasLct,
			interlaced,
			sorted,
			lctSize
		);

		return ImageDescriptor(
			left,
			top,
			width,
			height,
			packedFlag
		);
	}


	std::optional<GCE> readGce() {
	

		reader.readByte(); // block size, alway 4 so ignore

		uint8_t packed = reader.readByte();


		// extract disposal method and transparency flag from packed byte
		uint8_t disposal = (packed >> 2) & 0x07;
		bool hasTransparency = (packed & 0x01) != 0;

		GCE::PackedFlag packedFlag(
			disposal, 
			hasTransparency
		);

		uint16_t delay = reader.readUInt16_le();
		uint8_t transparentColorIndex = reader.readByte();

		reader.readByte(); // terminator, just ignore

		
		return GCE(
			packedFlag,
			delay,
			transparentColorIndex
		);

	}
	
	std::vector<uint8_t> readCompressedData(){

		std::vector<uint8_t> compressedData;

		uint8_t subSize;
		while ((subSize = reader.readByte()) != 0)
		{
			std::vector<uint8_t> subBlock = reader.readBytes(subSize);
			compressedData.insert(compressedData.end(), subBlock.begin(), subBlock.end());
		}

		return compressedData;
	}



	GIF buildGIF() {
		std::string header = readHeader();
		LogicalDescriptor logicalDescriptor = readLogicalDescriptor();

		std::optional<ColorTable>  gct = std::nullopt;

		if (logicalDescriptor.getPackedFlag().isGctExists()) {
			int gctSizeCode = logicalDescriptor.getPackedFlag().getGctSizeCode();

			int gctSize = 1 << (gctSizeCode + 1);

			gct = readColorTable(gctSize);
		}

		std::optional<ApplicationData> applicationData = std::nullopt;
		std::vector<Frame> frames;

		
		std::optional<GCE> gce = std::nullopt;

		while (true) {
			uint8_t currentByte = reader.readByte();

			if (currentByte == 0x3B){
				break; //terminator. (EOF)
			}

			if (currentByte == 0x21) { // extension introducer
				uint8_t label = reader.readByte();

				if (label == 0xF9) {
					gce = readGce();
				}
				else if (label == 0xFF) {
					applicationData = readApplicationData();
				}
				else {
					// Unknown extension (e.g. 0xFE Comment, 0x01 Plain Text) — skip sub-blocks
					uint8_t subBlockSize;
					while ((subBlockSize = reader.readByte()) != 0) {
						reader.readBytes(subBlockSize);
					}
				}

			}
			else if (currentByte == 0x2C) {
				ImageDescriptor imageDescriptor = readImageDescriptor();

				std::optional<ColorTable> lct = std::nullopt;

				if (imageDescriptor.getPackedFlag().isLctExists()) {
					int lctSizeCode = imageDescriptor.getPackedFlag().getLctSizeCode();
					int lctSize = 1 << (lctSizeCode + 1);
					lct = readColorTable(lctSize);
				}

				uint8_t minimumCodeSize = reader.readByte();

				std::vector<uint8_t> compressedData = readCompressedData();

				Frame frame(
					imageDescriptor,
					gce,
					lct,
					minimumCodeSize,
					compressedData
				);

				frames.push_back(frame);
			}
		
		}

		return GIF(
			header,
			logicalDescriptor,
			gct,
			applicationData,
			frames
		);
		
	}

public:
	GIFDataExtractor(const std::string& filePath): reader(filePath) {}

	// apply singleton later. for now assume this method is called only once.
	GIF extractGIF() {
		return buildGIF();
	}

};


