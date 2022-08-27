// (c) 2021 Jens Ganter-Benzing. Licensed under the MIT license.
#include <cstdint>
#include <iostream>
#include <utility>
#include <cctype>
#include <cstdlib>
#include <cassert>
#include <colmc/colmc.h>

using namespace std;
using namespace colmc;

namespace {

enum class cell: uint8_t {
	empty,
	player_1,
	player_2
};

//        Z  Y  X
cell cube[4][4][4] = { cell::empty };

enum class player: uint8_t {
	player_1, player_2
};

cell current_player = cell::player_1;

constexpr char ball = '%';
const char* const color_player_1 = fore::red;
const char* const color_player_2 = fore::cyan;

}

/*
2D views:

   front           left          right           back            top             bottom

 A  B  C  D     4  3  2  1     1  2  3  4     D  C  B  A      A  B  C  D       A  B  C  D

 |  |  |  |     |  |  |  |     |  |  |  |     |  |  |  |   4 ( )( )( )( )   1 ( )( )( )( )
 |  |  |  |     |  |  |  |     |  |  |  |     |  |  |  |   3 ( )( )( )( )   2 ( )( )( )( )
 |  |  |  |     |  |  |  |     |  |  |  |     |  |  |  |   2 ( )( )( )( )   3 ( )( )( )( )
 |  |  |  |     |  |  |  |     |  |  |  |     |  |  |  |   1 ( )( )( )( )   4 ( )( )( )( )


3D view:

           A       B       C       D

      4    |       |       |       |
    3    | |     | |     | |     | |
  2    | | |   | | |   | | |   | | |
1    | | | | | | | | | | | | | | | |
     | | | | | | |   | | | | | | |
     | |     | |     | |     | |
     |       |       |       |
*/

void print_cell_content(cell c, char alternative = '|') {
	if (c == cell::player_1) {
		cout << color_player_1 << ball << fore::reset;
	}
	else if (c == cell::player_2) {
		cout << color_player_2 << ball << fore::reset;
	}
	else {
		cout << alternative;
	}
}

pair<int, int> read_coordinate() {
	char entered[2];
	cin >> entered[0];
	if (tolower(entered[0]) == 'z') {
		return {-2, -2};
	}
	cin >> entered[1];
	if (isdigit(entered[1])) {
		swap(entered[0], entered[1]);
	}
	entered[1] = static_cast<char>(tolower(entered[1]));
	if ((entered[0] >= '1') && (entered[0] <= '4')) {
		const int y = static_cast<int>(entered[0]) - static_cast<int>('1');
		if ((entered[1] >= 'a') && (entered[1] <= 'd')) {
			const int x = static_cast<int>(entered[1]) - static_cast<int>('a');
			return {x, y};
		}
	}
	return {-1, -1};
}

int get_col_height(int x, int y) {
	int height = 0;
	for (int i = 0; i < 4; ++i) {
		if (cube[i][y][x] != cell::empty) {
			++height;
		}
	}
	return height;
}

void print() {
	cout << '\n';
	cout << "2D views:\n\n";
	cout << "   front           left          right           back            top             bottom\n\n";
	cout << " A  B  C  D     4  3  2  1     1  2  3  4     D  C  B  A      A  B  C  D       A  B  C  D\n\n";
	for (int i = 0; i < 4; ++i) {
		// front view
		cout << ' ';
		for (int j = 0; j < 4; ++j) {
			if (j > 0) {
				cout << "  ";
			}
			// j is X axis, i is Z axis, Y axis is always 0 (front)
			const auto c = cube[3-i][0][j];
			print_cell_content(c);
		}
		cout << "     ";
		// left view
		for (int j = 3; j >= 0; --j) {
			if (j < 3) {
				cout << "  ";
			}
			// j is Y axis, i is Z axis, X axis is always 0 (left)
			const auto c = cube[3-i][j][0];
			print_cell_content(c);
		}
		cout << "     ";
		// right view
		for (int j = 0; j < 4; ++j) {
			if (j > 0) {
				cout << "  ";
			}
			// j is Y axis, i is Z axis, X axis is always 3 (right)
			const auto c = cube[3-i][j][3];
			print_cell_content(c);
		}
		cout << "     ";
		// back view
		for (int j = 3; j >= 0; --j) {
			if (j < 3) {
				cout << "  ";
			}
			// j is X axis, i is Z axis, Y axis is always 3 (back)
			const auto c = cube[3-i][3][j];
			print_cell_content(c);
		}
		cout << "   ";
		cout << static_cast<char>('4' - i);
		cout << ' ';
		// top view
		for (int j = 0; j < 4; ++j) {
			cout << '(';
			// j is X axis, i is Y axis, Z is the hightest visible;
			const auto col_height = get_col_height(j, 3-i);
			if (col_height == 0) {
				cout << ' ';
			}
			else {
				const auto z = col_height - 1;
				const auto c = cube[z][3-i][j];
				print_cell_content(c, ' ');
			}
			cout << ')';
		}
		cout << "   ";
		cout << static_cast<char>('1' + i);
		cout << ' ';
		// bottom view
		for (int j = 0; j < 4; ++j) {
			cout << '(';
			// j is X axis, i Y axis, Z is always 0
			const auto c = cube[0][i][j];
			print_cell_content(c, ' ');
			cout << ')';
		}
		cout << '\n';
	}
	/*
	3D view:

	                    A       B       C       D

	Row 1:         4    |       |       |       |
	Row 2:       3    | |     | |     | |     | |
	Row 3:     2    | | |   | | |   | | |   | | |
	Row 4:   1    | | | | | | | | | | | | | | | |
	Row 5:        | | |   | | |   | | | | | | |
	Row 6:        | |     | |     | |     | |
	Row 7:        |       |       |       |
	*/
	cout << "\n3D view:\n\n";
	cout << "           A       B       C       D\n\n";
	// Row 1:
	cout << "      4    ";
	for (int i = 0; i < 4; ++i) {
		if (i > 0) {
			cout << "       ";
		}
		print_cell_content(cube[3][3][i]);
	}
	// Row 2:
	cout << "\n    3    ";
	for (int i = 0; i < 4; ++i) {
		if (i > 0) {
			cout << "     ";
		}
		print_cell_content(cube[3][2][i]);
		cout << ' ';
		print_cell_content(cube[2][3][i]);
	}
	cout << "\n  2    ";
	for (int i = 0; i < 4; ++i) {
		if (i > 0) {
			cout << "   ";
		}
		print_cell_content(cube[3][1][i]);
		cout << ' ';
		print_cell_content(cube[2][2][i]);
		cout << ' ';
		print_cell_content(cube[1][3][i]);
	}
	// Row 4:
	cout << "\n1    ";
	for (int i = 0; i < 4; ++i) {
		if (i > 0) {
			cout << " ";
		}
		print_cell_content(cube[3][0][i]);
		cout << ' ';
		print_cell_content(cube[2][1][i]);
		cout << ' ';
		print_cell_content(cube[1][2][i]);
		cout << ' ';
		print_cell_content(cube[0][3][i]);
	}
	// Row 5:
	cout << "\n     ";
	for (int i = 0; i < 4; ++i) {
		if (i > 0) {
			cout << "   ";
		}
		print_cell_content(cube[2][0][i]);
		cout << ' ';
		print_cell_content(cube[1][1][i]);
		cout << ' ';
		print_cell_content(cube[0][2][i]);
	}
	// Row 6:
	cout << "\n     ";
	for (int i = 0; i < 4; ++i) {
		if (i > 0) {
			cout << "     ";
		}
		print_cell_content(cube[1][0][i]);
		cout << ' ';
		print_cell_content(cube[0][1][i]);
	}
	// Row 7:
	cout << "\n     ";
	for (int i = 0; i < 4; ++i) {
		if (i > 0) {
			cout << "       ";
		}
		print_cell_content(cube[0][0][i]);
	}
	std::cout << '\n' << endl;
}

void copy_plane(cell(&out_plane)[4][4], int dimension, int run_variable) {
	if (dimension == 0) {
		// running x axis
		for (int y = 0; y < 4; ++y) {
			for (int z = 0; z < 4; ++z) {
				out_plane[z][y] = cube[z][y][run_variable];
			}
		}
	}
	else if (dimension == 1) {
		// running y axis
		for (int x = 0; x < 4; ++x) {
			for (int z = 0; z < 4; ++z) {
				out_plane[z][x] = cube[z][run_variable][x];
			}
		}
	}
	else {
		// running z axis
		for (int x = 0; x < 4; ++x) {
			for (int y = 0; y < 4; ++y) {
				out_plane[y][x] = cube[run_variable][y][x];
			}
		}
	}
}

void rotate_plane(cell(&plane)[4][4]) {
	cell temp[4][4];
	for (int y = 0; y < 4; ++y) {
		for (int x = 0; x < 4; ++x) {
			temp[y][x] = plane[x][y]; // swap x with y
		}
	}
    // and copy result back:
	for (int y = 0; y < 4; ++y) {
		for (int x = 0; x < 4; ++x) {
			plane[y][x] = temp[y][x];
		}
	}
}

cell won_plane(cell(&plane)[4][4]) {
	// This performs only half of the check. For a
    // full check, the plane needs to be rotated and checked again.

	// check rows:
	for (int y = 0; y < 4; ++y) {
		cell player = plane[y][0];
		if (player != cell::empty) {
			bool won = true;
			for (int x = 1; x < 4; ++x) {
				if (plane[y][x] != player) {
					won = false;
					break;
				}
			}
			if (won) {
				return player;
			}
		}
	}
	// check diagonal:
	cell player = plane[0][0];
	if (player != cell::empty) {
		bool won = true;
		for (int i = 1; i < 4; ++i) {
			if (plane[i][i] != player) {
				won = false;
				break;
			}
		}
		if (won) {
			return player;
		}
	}
	return cell::empty;
}

cell won_room_diagonal(int dimension) {
	if (dimension == 0) {
		cell player = cube[0][0][0];
		if (player != cell::empty) {
			bool won = true;
			for (int i = 1; i < 4; ++i) {
				if (cube[i][i][i] != player) {
					won = false;
					break;
				}
			}
			if (won) {
				return player;
			}
		}
	}
	else if (dimension == 1) {
		cell player = cube[3][0][3];
		if (player != cell::empty) {
			bool won = true;
			for (int i = 1; i < 4; ++i) {
				if (cube[3-i][i][3-i] != player) {
					won = false;
					break;
				}
			}
			if (won) {
				return player;
			}
		}
	}
	else {
		cell player = cube[3][3][0];
		if (player != cell::empty) {
			bool won = true;
			for (int i = 1; i < 4; ++i) {
				if (cube[3-i][3-i][i] != player) {
					won = false;
					break;
				}
			}
			if (won) {
				return player;
			}
		}
	}
	return cell::empty;
}

cell won_player() {
	// check planes, then room (3d) diagonals. Planes first:
	cell plane[4][4];
	for (int dimension = 0; dimension < 3; ++dimension) {
		for (int run_var = 0; run_var < 4; ++run_var) {
			// copy one plane of the cube into output variable 'plane'
			copy_plane(plane, dimension, run_var);
			auto won = won_plane(plane);
			if (won != cell::empty) {
				return won;
			}
			rotate_plane(plane); // check the same, but 90° rotated
			won = won_plane(plane);
			if (won != cell::empty) {
				return won;
			}
		}
		// check the room diagonal:
		const auto won = won_room_diagonal(dimension);
		if (won != cell::empty) {
			return won;
		}
	}
	return cell::empty;
}

void check_won() {
	const auto won = won_player();
	if (won != cell::empty) {
		cout << "Congratulations, ";
		if (won == cell::player_1) {
			cout << color_player_1 << "Player 1" << fore::reset;
		}
		else {
			cout << color_player_2 << "Player 2" << fore::reset;
		}
		cout << ", you won. New game is started." << std::endl;
		current_player = cell::player_1;
		for (int x = 0; x < 4; ++x) {
			for (int y = 0; y < 4; ++y) {
				for (int z = 0; z < 4; ++z) {
					cube[z][y][x] = cell::empty;
				}
			}
		}
		print();
	}
}

int main() {
	::colmc::setup();
	cout << "This is a 3D connect-four game. \n";
	cout << "There are 16 vertical rods in a grid (4 by 4) and on each rod,\n";
	cout << "4 balls with a hole in the center can be stuck onto. They'll then fall\n";
	cout << "down the rod. When, like in a normal connect-four game, one player can\n";
	cout << "build a line or diagonal with 4 balls, he/she wins. In this 3D version however,\n";
	cout << "there are multiple planes and even room diagonals that are valid options!\n";
	cout << "Have fun!";
	for(;;) {
		print();
		check_won();
		if (current_player == cell::player_1) {
			cout << color_player_1 << "Player 1:";
		}
		else {
			cout << color_player_2 << "Player 2:";
		}
		cout << fore::reset << " Please enter a letter A-D and a digit 1-4 where to place the ball or Z to quit:";
		const auto coord = read_coordinate();
		const auto x = get<0>(coord);
		const auto y = get<1>(coord);
		if (x == -2) {
			exit(0);
		}
		if (x < 0) {
			cout << "Invalid input.\n";
			continue;
		}
		assert(x >= 0);
		assert(x < 4);
		assert(y >= 0);
		assert(y < 4);
		const auto col_height = get_col_height(x, y);
		if (col_height >= 4) {
			std::cout << "That column is already filled. Please choose another coordinate.\n";
			continue;
		}
		const auto z = col_height;
		cube[z][y][x] = current_player;
		current_player = (current_player == cell::player_1) ? cell::player_2 : cell::player_1;
	}
	return 0;
}
