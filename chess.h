#ifndef CHESS_H
#define CHESS_H

#include <stdbool.h>

// Board Size //
#define CHESS_BOARD_WIDTH 8
#define CHESS_BOARD_HEIGHT 8

// Player Macros //
enum {
	White,
	Black,
};

//              //
// Piece Macros //
//              //

// Generic Names for pieces //
enum {
Empty_Square,
Pawn,
Knight,
Bishop,
Rook,
Queen,
King,
PIECE_MAX
};

// Long Form Names for Color Pieces //
enum {
	No_Piece,
	White_Pawn,
	White_Knight,
	White_Bishop,
	White_Rook,
	White_Queen,
	White_King,
	Black_Pawn,
	Black_Knight,
	Black_Bishop,
	Black_Rook,
	Black_Queen,
	Black_King,
};
/* Used when I want the code to be easy to read */

// Short Form for Color Pieces //
enum {
	NP,
	WP,
	WN,
	WB,
	WR,
	WQ,
	WK,
	BP,
	BN,
	BB,
	BR,
	BQ,
	BK,
};
/*
Used when I want the code to be easy to write and 
for board initialization so it can fit with in 80
characters of the terminal
*/

//              //
// Piece Macros //
//              //

//            //
// Cordinates //
//            //

enum {
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
};

enum {
	a,
	b,
	c,
	d,
	e,
	f,
	g,
	h,
};

//            //
// Structures //
//            //
typedef struct {
	int8_t x;
	int8_t y;
} Chess_Cord;

//            //
// Cordinates //
//            //

//           //
// Functions //
//           //

bool is_same_type(uint8_t piece1,uint8_t piece2);
char* get_chess_symbol_from_enumerated_value(uint8_t value);
void chess_board_print(uint8_t board [CHESS_BOARD_HEIGHT][CHESS_BOARD_WIDTH]);
bool chess_validate_move(uint8_t board [CHESS_BOARD_HEIGHT][CHESS_BOARD_WIDTH],
						Chess_Cord Source,Chess_Cord Dest,bool player);
void chess_play(uint8_t board [CHESS_BOARD_HEIGHT][CHESS_BOARD_WIDTH],bool player);

//           //
// Functions //
//           //

//                 //
// Macro Functions //
//                 //

#define chess_move_cord(board,sx,sy,dx,dy) {\
	board[CHESS_BOARD_HEIGHT-dy][dx] = board[CHESS_BOARD_HEIGHT-sy][sx];\
	board[CHESS_BOARD_HEIGHT-sy][sx] = No_Piece;\
}
#define chess_move(board,sx,sy,dx,dy) {\
	board[dy][dx] = board[sy][sx];\
	board[sy][sx] = No_Piece;\
}
#define is_white(piece) (piece >= White_Pawn && piece <= White_King)
// #define is_black(piece) (piece >= Black_Pawn && piece <= Black_King)
#define is_black(piece) (piece >= Black_Pawn)
#define is_empty(piece) (piece == No_Piece);
#define get_color(piece) is_black(piece)
// #define get_color_safe(piece) ((piece-White_King) > 0)
#define is_same_color(piece1,piece2) ((is_black(piece1) && is_black(piece2)) || (is_white(piece1) && is_white(piece2)))
#define is_same_color_or_empty(piece1,piece2) (get_color_fast(piece1) == get_color_fast(piece2))

//                 //
// Macro Functions //
//                 //

//               //
// Constant Text //
//               //

extern const char PlayerColorText [][5];

//               //
// Constant Text //
//               //

#endif // CHESS_H //