#include<iostream>
#include<fstream>
#include<vector>
#include<bitset>
#include<math.h>


//const std::vector<unsigned char> readByte(std::ifstream& file, int bytes) {
//	std::vector<char> buffer(bytes);
//	file.read(buffer.data(), bytes);
//	std::vector<unsigned char> data;
//	for (char c : buffer) {
//		data.push_back((unsigned char)c);
//	}
//	return data;
//}
//
//const unsigned char readByte(std::ifstream& file) {
//	char buffer[1];
//	file.read(buffer, 1);
//	return (unsigned char)(buffer[0]);
//}
//
//uint16_t readUInt16(std::vector<unsigned char> buffer, int offset) {
//	uint8_t* data = reinterpret_cast<uint8_t*>(buffer.data());
//	return data[offset] | data[offset + 1] << 8;
//}
//
//uint8_t readUInt8(std::vector<unsigned char> buffer, int offset) {
//	uint8_t* data = reinterpret_cast<uint8_t*>(buffer.data());
//	return data[offset];
//}
//
//void printBuffer(const std::vector<unsigned char> buffer) {
//	for (unsigned char c : buffer) {
//		std::cout << (int)c << " ";
//	}
//	std::cout << "\n";
//}
//
//int main() {
//	std::ifstream file("C:\\Users\\pavan\\Downloads\\FILE1(1).gif", std::ios::binary);
//
//	std::vector<unsigned char> GIFHeader = readByte(file,6);
//
//	printBuffer(GIFHeader);
//
//	std::vector<unsigned char> logicalDescriptor = readByte(file, 7);
//
//	uint16_t width = readUInt16(logicalDescriptor, 0);
//	uint16_t height = readUInt16(logicalDescriptor, 2);
//	uint8_t packed = readUInt8(logicalDescriptor, 4);
//	uint8_t bgColorIndex = readUInt8(logicalDescriptor, 5);
//	uint8_t pixelAspectRatio = readUInt8(logicalDescriptor, 6);
//
//	uint8_t isGctExists = (packed & 0x80) >> 7;
//	uint8_t colorResolution = (packed & 0x70) >> 4;
//	uint8_t isGctSorted = (packed & 0x8) >> 3;
//	uint8_t gctSize = (packed & 0x7);
//
//	std::cout 
//		<< width << " " 
//		<< height << " "
//		<< (int)packed << " "
//		<< (int)bgColorIndex << " "
//		<< (int)pixelAspectRatio << " " << std::endl;
//
//	std::cout
//		<< "isGCTExists " << (bool)isGctExists<< "\n"
//		<< "colorResolution " << (int)colorResolution << "\n"
//		<< "isGctSorted " << (bool)isGctSorted << "\n"
//		<< "gctSize " << (int)gctSize << "\n";
//
//	int gctSizeInBytes = (int)std::pow(2, gctSize + 1)*3;
//	
//	std::vector<unsigned char> gct = readByte(file, gctSizeInBytes);
//
//	for (int i = 0; i < gct.size(); i++) {
//		if (i % 3 == 0) std::cout << "\n" << i / 3 << " : ";
//		std::cout << (int)gct[i]<< " ";
//	}
//
//
//
//
//	int frame_index = 0;
//
//	while (true) {
//		int curr = (int) readByte(file);
//
//		std::cout << "currentByte " << curr << "\n";
//
//		if (curr == 0x3B) {
//			//terminator
//			break;
//		}
//		else if (curr == 0x21) {                                   
//			int label = readByte(file);
//
//
//			if (label == 0xF9) {
//				//handle gct
//				std::vector<unsigned char> gce = readByte(file, 6);
//
//				std::cout << "------------------------------\n";
//				std::cout << "              GCE             \n";
//				std::cout << "------------------------------\n";
//
//				std::cout << "block size(always 4) " << (int)gce[0]  <<"\n";
//				std::cout << "------------------------\n";
//				std::cout << "packed byte " << (int)gce[1] << "\n";
//				std::cout << std::bitset<8>(gce[1]);
//				std::cout << "------------------------\n";
//				std::cout << "delay low " << (int)gce[2] << "\n";
//				std::cout << "delay high " << (int)gce[3] << "\n";
//				std::cout << "transparent color index " << (int)gce[4] << "\n";
//				std::cout << "block terminator " << (int)gce[5] << "\n";
//				std::cout << "------------------------------";
//
//			}
//			else if (label == 0xFF) {
//				readByte(file);
//				std::vector<unsigned char> asciiNameAndVersionBuffer = readByte(file, 11);
//
//				std::string asciiNameAndVersion(asciiNameAndVersionBuffer.begin(), asciiNameAndVersionBuffer.end());
//
//				std::cout << asciiNameAndVersion << "\n";
//
//				if (asciiNameAndVersion == "NETSCAPE2.0") {
//
//					std::vector<unsigned char> subBlock = readByte(file, 5);
//					uint8_t subBlockSize = readUInt8(subBlock, 0);
//					uint8_t subBlockId = readUInt8(subBlock, 1);
//
//					uint16_t loopCount = readUInt16(subBlock, 2);
//
//					uint8_t terminator = readUInt8(subBlock, 4);
//
//					std::cout
//						<< "subBlocksize " << (int)subBlockSize << "\n"
//						<< "subBlockId " << (int)subBlockId << "\n"
//						<< "loop count " << (int)loopCount << "\n"
//						<< "terminator " << (int)terminator << "\n";
//				}
//				else {
//					uint8_t subBlockSize;
//					while ((subBlockSize = readByte(file, 1)[0]) != 0) {
//						readByte(file, (int)subBlockSize);
//					}
//				}
//			}
//		}
//		else if ( curr == 0x2C) {
//			uint16_t left = readUInt16(readByte(file, 2),0);
//			uint16_t top = readUInt16(readByte(file, 2),0);
//			uint16_t width = readUInt16(readByte(file, 2),0);
//			uint16_t height = readUInt16(readByte(file, 2),0);
//
//			uint8_t packed = readByte(file);
//
//			bool    has_lct = (packed >> 7) & 0x01;
//			uint8_t lct_flag = (packed >> 0) & 0x07;
//			int     lct_count = 1 << (lct_flag + 1);
//			if (has_lct) {
//				for (int i = 0; i < lct_count * 3; i++) readByte(file);
//			}
//
//			uint8_t min_code_size = readByte(file);
//
//			// print frame header
//			printf("\n--- FRAME %d ---\n", frame_index);
//			printf("position: left=%d top=%d\n", left, top);
//			printf("size:      width=%d height=%d\n", width, height);
//			printf("lzw min code size: %d\n", min_code_size);
//			printf("compressed bytes:\n");
//
//			// read and print sub-blocks
//			uint8_t sub_size;
//
//			int byteCount = 0;
//			while ((sub_size = readByte(file)) != 0)
//			{
//				for (int i = 0; i < sub_size; i++)
//				{
//					uint8_t byte = readByte(file);
//					printf("%02X ", byte);
//					byteCount++;
//				}
//			}
//			printf("byte count %d", byteCount);
//
//			if (frame_index == 3) break;
//			frame_index++;
//		}
//
//	}
//
//}
