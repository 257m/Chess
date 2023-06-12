#include <stdio.h>
#include <stdint.h>
#include "chess.h"

uint8_t Chess_Board [][8] = {
	{BR,BN,BB,BQ,BK,BB,BN,BR},
	{BP,BP,BP,BP,BP,BP,BP,BP},
	{NP,NP,NP,NP,NP,NP,NP,NP},
	{NP,NP,NP,NP,NP,NP,NP,NP},
	{NP,NP,NP,NP,NP,NP,NP,NP},
	{NP,NP,NP,NP,NP,NP,NP,NP},
	{WP,WP,WP,WP,WP,WP,WP,WP},
	{WR,WN,WB,WQ,WK,WB,WN,WR},
};

int main(void)
{
	chess_play(Chess_Board,White);
	return 0;
}