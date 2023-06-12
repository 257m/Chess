#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "chess.h"

//               //
// Constant Text //
//               //

const char PlayerColorText [][5] = {
"White",
"Black",
};

//               //
// Constant Text //
//               //

bool is_same_type(uint8_t piece1,uint8_t piece2)
{
	if (piece1 == No_Piece) {
		if (piece2 == No_Piece)
			return true;
		else
			return false;
	}
	if (piece2 == No_Piece)
		return false;

	return (get_color(piece1) == get_color(piece2));
}

char* get_chess_symbol_from_enumerated_value(uint8_t value)
{
	switch(value) {
		case No_Piece:
			return "-";
		case White_Pawn:
			return "♙";
		case White_Knight:
			return "♘";
		case White_Bishop:
			return "♗";
		case White_Rook:
			return "♖";
		case White_Queen:
			return "♕";
		case White_King:
			return "♔";
		case Black_Pawn:
			return "♟";
		case Black_Knight:
			return "♞";
		case Black_Bishop:
			return "♝";
		case Black_Rook:
			return "♜";
		case Black_Queen:
			return "♛";
		case Black_King:
			return "♚";
		default:
			return " ";
	}
}

void chess_board_print(uint8_t board [CHESS_BOARD_HEIGHT][CHESS_BOARD_WIDTH])
{
	for (int i = 0;i < CHESS_BOARD_HEIGHT;i++) {
		for (int j = 0;j < CHESS_BOARD_WIDTH;j++) {
			printf(" %s",get_chess_symbol_from_enumerated_value(board[i][j]));
		}
		printf("\n");
	}
}

#define is_position_in_board(x,y) (y < CHESS_BOARD_HEIGHT && y >= 0 && x < CHESS_BOARD_WIDTH && x >= 0)
#define player_piece(player,piece) (piece + player*King)

bool is_in_check(uint8_t board [CHESS_BOARD_HEIGHT][CHESS_BOARD_WIDTH],bool player) {
	Chess_Cord King_Pos = {0,0};
	// Get player's king position
	for (;King_Pos.y < CHESS_BOARD_HEIGHT;King_Pos.y++) {
		for (King_Pos.x = 0;King_Pos.x < CHESS_BOARD_WIDTH;King_Pos.x++) {
			if (board[King_Pos.y][King_Pos.x] == player_piece(player,King))
				goto found_king;
		}
	}
	found_king:;
	// Check for Knight
	Chess_Cord Temp_Pos = {1,2};
	int i = 0;
	goto for_jump;
	for (;i < 8;i++) {
		if (i % 2) {
			if (abs(Temp_Pos.x) == 1)
				Temp_Pos.x &= 0x82;
			else
				Temp_Pos.x &= 0x81;
			if (abs(Temp_Pos.y) == 1)
				Temp_Pos.y &= 0x82;
			else
				Temp_Pos.y &= 0x81;
		}
		else {
			Temp_Pos.x *= -1;
			Temp_Pos.y *= -1;
		}
		for_jump:
		if (is_position_in_board(Temp_Pos.x+King_Pos.x,Temp_Pos.y+King_Pos.y))
			if (board[Temp_Pos.x+King_Pos.x][Temp_Pos.y+King_Pos.y] == player_piece(!player,Knight))
				return true;
	}

	// Check for Rook
	for (int i = 0;i < 4;i++) {
		Temp_Pos = King_Pos;
		Chess_Cord Temp_Speed = {
		.x = (i < 2)*((i == 0)*1 + (i == 1)*-1),
		.y = (i > 1)*((i == 2)*1 + (i == 3)*-1)
		};
		while (1) {
			Temp_Pos.x += Temp_Speed.x;
			Temp_Pos.y += Temp_Speed.y;
			if (!is_position_in_board(Temp_Pos.x,Temp_Pos.y))
				break;
			if (board[Temp_Pos.y][Temp_Pos.x] == player_piece(!player,Rook) || board[Temp_Pos.y][Temp_Pos.x] == player_piece(!player,Queen))
				return true;
			if (board[Temp_Pos.y][Temp_Pos.x] != Empty_Square)
				break;
		}
	}
	
	// Check for Bishop
	for (int i = 0;i < 4;i++) {
		Temp_Pos = King_Pos;
		Chess_Cord Temp_Speed = {
		.x = (i % 2 == 0)*2 - 1,
		.y = (i < 2)*2 - 1,
		};
		while (1) {
			Temp_Pos.x += Temp_Speed.x;
			Temp_Pos.y += Temp_Speed.y;
			if (!is_position_in_board(Temp_Pos.x,Temp_Pos.y))
				break;
			if (board[Temp_Pos.y][Temp_Pos.x] == player_piece(!player,Bishop) || board[Temp_Pos.y][Temp_Pos.x] == player_piece(!player,Queen))
				return true;
			if (board[Temp_Pos.y][Temp_Pos.x] != Empty_Square)
				break;
		}
	}
	
	// Check For Pawn
	if (is_position_in_board(King_Pos.y+((player == White)*-2 + 1),King_Pos.x+1))
		if (board[King_Pos.y+((player == White)*-2 + 1)][King_Pos.x+1] == player_piece(!player,Pawn))
			return true;
	if (is_position_in_board(King_Pos.y+((player == White)*-2 + 1),King_Pos.x-1))
		if (board[King_Pos.y+((player == White)*-2 + 1)][King_Pos.x-1] == player_piece(!player,Pawn))
			return true;

	// Check for opposing king
	for (int i = -1;i < 2;i++) {
		for (int j = -1;j < 2;j++) {
			if (is_position_in_board(King_Pos.y+i,King_Pos.x+j))
				if (board[King_Pos.y+i,King_Pos.x+j] == player_piece(!player,King))
					return true;
		}
	}
	
	return false;
}

/*bool piece_has_legal_move(uint8_t board [CHESS_BOARD_HEIGHT][CHESS_BOARD_WIDTH],Chess_Cord Piece_Pos) {
	
}

bool player_has_legal_move(uint8_t board [CHESS_BOARD_HEIGHT][CHESS_BOARD_WIDTH],bool player)
{
	for (int i = 0;i < )
}*/

bool chess_validate_move(uint8_t board [CHESS_BOARD_HEIGHT][CHESS_BOARD_WIDTH],
						Chess_Cord Source,Chess_Cord Dest,bool player_color)
{
	if (board[Source.y][Source.x] == No_Piece)
		return false;
	if (is_same_color(board[Source.y][Source.x],board[Dest.y][Dest.x]))
		return false;
	if (get_color(board[Source.y][Source.x]) != player_color)
		return false;

	switch(board[Source.y][Source.x] - (player_color*King)) {
		case Pawn: {
			/*
				If the move is not along the same file.
				Return false if it is not a adjacent
				file. Also return false if the
				destination in the adjacent file is not
				an enemy piece.
			*/
			if (Source.x != Dest.x) {
				if (!(abs(Source.x - Dest.x) == 1))
					return false;
				else
					if (board[Dest.y][Dest.x] == No_Piece)
						return false;
			}

			// Get the difference in the y position between the squares
			// and also save the y position of the source square
			int8_t yDif = Source.y - Dest.y;
			int8_t ySourcePos = Source.y;

			// If the player color is black
			// Invert the y position and dif now so I don't have to check
			// later whether it's black or not
			if (player_color == Black) {
				yDif *= -1;
				ySourcePos = (CHESS_BOARD_HEIGHT-1) - ySourcePos;
			}

			// If the pawn is moving backwards return false
			if (yDif <= 0)
				return false;

			/*
				If the pawn is not moving a single square forward.
				Return false is not a difference of two and if it
				is two and you are not moving along the same file
				return false. Also check if the pawn has moved
				before and if so return false because you can only
				move two spaces if the pawn has not moved yet.
			*/
			if (yDif != 1) {
				if (yDif != 2)
					return false;
				if (Source.x != Dest.x)
					return false;
				if (ySourcePos != 6)
					return false;
			}
			
			break;
		}
		case Knight: {
			// Check whether the difference on the y axis is one
			// If so check whether the difference on the x axis
			// is two. If both are true this is a valid move
			if (abs(Source.y - Dest.y) == 1) {
				if (abs(Source.x - Dest.x) == 2)
					break;
			}
			// Same as above but this time check whether the difference
			// on y is two and on the x axis it is one.
			else if (abs(Source.y - Dest.y) == 2)
				if (abs(Source.x - Dest.x) == 1)
					break;
			return false;
		}
		case Bishop: {
			// Check whether the bishop has moved diagonally.
			// Elsewise return false
			if (abs(Source.x - Dest.x) != abs(Source.y - Dest.y))
				return false;
			Bishop_Queen:; // For Queen to use Bishop Logic without having duplicate code.
			// Also skip the diagonal check because that happened in the Queen logic if 
			// this was a conditional jump.
			
			// Check if piece is blocking path
			// If so return false

			// Initialize Speeds
			int8_t BishopXSpeed = 0;
			int8_t BishopYSpeed = 0;

			// If the move is moving towards the left set the speed to -1
			// Elsewise it should 1
			if (Dest.x - Source.x < 0)
				BishopXSpeed = -1;
			else
				BishopXSpeed = 1;

			// Same as above but this time if moving upwards
			if (Dest.y - Source.y < 0)
				BishopXSpeed = -1;
			else
				BishopXSpeed = 1;

			// Initialize a chess cordinate and store the source cordinate inside
			Chess_Cord Block_Check_Cord = {
			.x = Source.x,
			.y = Source.y,
			};

			// While the bishop has not hit the edge or hasn't
			// reached it's destination move diagonally and check
			// whether that square is empty. If not return false.
			while (Block_Check_Cord.x == (BishopXSpeed == 1)*7 || Block_Check_Cord.y == (BishopYSpeed == 1)*7) {
				Block_Check_Cord.x += BishopXSpeed;
				Block_Check_Cord.y += BishopYSpeed;
				if (Block_Check_Cord.x == Dest.x && Block_Check_Cord.y == Dest.y)
					break;
				if (board[Block_Check_Cord.y][Block_Check_Cord.x] != Empty_Square)
					return false;
			}
			break;
		}
		case Rook: {
			Rook_Queen: // For Queen to use Rook Logic without having duplicate code
			// Check whether it is linear movement
			if (Source.x != Dest.x && Source.y == Dest.y)
				return false;

			Chess_Cord Block_Check_Cord = {
			.x = Source.x,
			.y = Source.y,
			};

			int8_t RookXSpeed = (Source.x - Dest.x);
			int8_t RookYSpeed = (Source.y - Dest.y);

			while (Block_Check_Cord.x == (RookXSpeed)*7 || Block_Check_Cord.y == (RookYSpeed)*7) {
				Block_Check_Cord.x += RookXSpeed;
				Block_Check_Cord.y += RookYSpeed;
				if (Block_Check_Cord.x == Dest.x && Block_Check_Cord.y == Dest.y)
					break;
				if (board[Block_Check_Cord.y][Block_Check_Cord.x] != Empty_Square)
					return false;
			}
			break;
		}
		case Queen: {
			// Check whether it is a rook or queen and jump to the appropriate logic
			if (abs(Source.x - Dest.x) != abs(Source.y - Dest.y))
				goto Bishop_Queen;
			else
				goto Rook_Queen;
		}
		case King: {
			// If it has moved more than one square on either axis return false
			if (abs(Source.x - Dest.x) > 1 || abs(Source.y - Dest.y) > 1)
				return false;
			break;
		}
	}
	uint8_t Dest_Save = board[Dest.y][Dest.x];
	uint8_t Source_Save = board[Source.y][Source.x];
	chess_move(board,Source.x,Source.y,Dest.x,Dest.y);
	bool result_save = is_in_check(board,player_color);
	board[Dest.y][Dest.x] = Dest_Save;
	board[Source.y][Source.x] = Source_Save;
	return !result_save;
}

void chess_play(uint8_t board [CHESS_BOARD_HEIGHT][CHESS_BOARD_WIDTH],bool player)
{
	char input [4] = "\0";
	while (1) {
		Chess_Cord Source_Cord;
		Chess_Cord Dest_Cord;
		int i;

		system("clear");
		chess_board_print(board);

		// Get the source square
		printf("Square to move from: ");
		fgets(input,sizeof(input),stdin);
		
		// If not valid input ask again
		if (input[0] < 'a' || input[0] > 'h' || input[1] < '1' || input[1] > '8') {
			fprintf(stderr,"Invalid Input\n");
			continue;
		}

		// Convert characters to cordinates
		Source_Cord.x = (input[0] - 'a');
		Source_Cord.y = (CHESS_BOARD_WIDTH-1) - (input[1] - '1');

		// Same as above but for the destination
		printf("Square to go to: ");
		fgets(input,sizeof(input),stdin);
		
		if (input[0] < 'a' || input[0] > 'h' || input[1] < '1' || input[1] > '8') {
			fprintf(stderr,"Invalid Input\n");
			continue;
		}
		
		Dest_Cord.x = (input[0] - 'a');
		Dest_Cord.y = (CHESS_BOARD_WIDTH-1) - (input[1] - '1');

		if (!chess_validate_move(board,Source_Cord,Dest_Cord,player)) {
			fprintf(stderr,"Invalid Move\n");
			continue;
		}

		chess_move(board,Source_Cord.x,Source_Cord.y,Dest_Cord.x,Dest_Cord.y);

		/*if (check_mate(board)) {
			printf("Check Mate!\n%s Wins!\n",PlayerColorText[player]);
			return;
		}*/

		player = !player;
	}
}