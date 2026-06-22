#pragma once
#include<vector>
#include<stdexcept>
#include<string>
#include<format>

class ColorTable {

public:
	struct Color {
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};
	ColorTable(const std::vector<Color>& colors) : colors(colors) {
		if (colors.size() > 256) {
			throw std::runtime_error("size of GCT cannot be more than 256");
		}

	}

	const std::vector<Color>& getColors() const {
		return colors;
	}

	const std::string toString() const{
		std::string str;

		for (size_t i = 0; i<colors.size();i++) {
			str += std::format(" index {} : ( {}, {}, {})\n", i, colors[i].r, colors[i].g, colors[i].b);
		}

		str += "\n";

		return std::string(str);
	}

private:
	std::vector<Color> colors;
};
