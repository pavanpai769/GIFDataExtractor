#pragma once
#include"GIF.cpp"
#include<iostream>

class GIFPrinter {

public:
	void printGIF(const GIF& gif) {
		std::cout << "-----------GIF-----------\n\n";
		std::cout << "header\n" << gif.getHeader() << "\n\n";
		std::cout << "logical Descriptor\n";
		std::cout << gif.getLogicalDescriptor().toString() << "\n";
		
		if (gif.hasApplicationData()) {
			std::cout << gif.getApplicationData().value().toString() << "\n";
		}

		for (auto frame : gif.getFrames()) {
			std::cout << frame.toString();
		}
	}
};