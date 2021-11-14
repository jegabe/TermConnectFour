// (c) 2021 Jens Ganter-Benzing. Licensed under the MIT license.
#include <cstdint>
#include <iostream>
#include <colmc/setup.h>
#include <colmc/sequences.h>

namespace {

enum class cell: std::uint8_t {
	empty,
	player_1,
	player_2
};

//        Z  Y  X
cell cube[4][4][4] = { cell::empty };

}

int main() {
	colmc::setup();
	std::cout << colmc::fore::red << "Hello, World" << colmc::reset_all << std::endl;
	std::cin.get();
	return 0;
}
