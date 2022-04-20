#include <gb/gb.h>
#include <stdio.h>
void init();
void updateSwitches();
void checkInput();
unsigned char sprites[] =
{
  0x7E,0x7E,0x81,0x81,0xA5,0xA5,0x81,0x81,
  0x81,0x81,0xBD,0xBD,0x81,0x81,0x7E,0x7E,
  0x7E,0x7E,0x81,0x81,0xA5,0xA5,0x81,0x81,
  0x99,0x99,0xA5,0xA5,0x81,0x81,0x7E,0x7E
};
UINT8 values[]={7, 8, 9, 4, 5, 6, 1, 2, 3, 0, 0, 10};
unsigned char sym[]={'+', '-', '*', '/'};
//waitpad(J_START);//wait for start to be pressed
// The player array will hold the player's position as X ([0]) and Y ([1])
UINT8 player[2];
UINT8 first, sign, second;
UINT8 x, y;
void main() {
	init();
	while(1) {
		checkInput();				// Check for user input (and act on it)
		updateSwitches();			// Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
		wait_vbl_done();			// Wait until VBLANK to avoid corrupting memory
	}
}
void init() {
	DISPLAY_ON;
	printf("789+\n456-\n123*\n=0 /\n");
	// Load the the 'sprites' tiles into sprite memory
	set_sprite_data(0, 1, sprites);
	// Set the first movable sprite (0) to be the first tile in the sprite memory (0)
	set_sprite_tile(0, 0);
	player[0] = 80;
	player[1] = 72;
	first=10;
	sign=10;
	second=10;
}
void updateSwitches() {
	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
}
void checkInput() {
	if(joypad()&J_A)
	{
		x=(player[0]-8)/7;
		y=(player[1]-16)/7;
		if(x<4&&y<4)
			if(first==10)
			{
				if(x!=3&&(x!=0||y!=3))
				{
					first=values[y*3+x];
					printf("%u", first);
				}
			}
			else if(sign==10)
			{
				if(x==3)
				{
					sign=y;
					printf("%c", sym[y]);
				}
			}
			else if(second==10)
			{
				if(x!=3&&(x!=0||y!=3))
				{
					second=values[y*3+x];
					printf("%u", second);
				}
			}
			else if(x==0&&y==3)
			{
				printf("=");
				if(sign==0)
					printf("%u\n", first+second);
				else if(sign==1)
					printf("%u\n", first-second);
				else if(sign==2)
					printf("%u\n", first*second);
				else
					printf("%u\n", first/second);
				first=10;
				sign=10;
				second=10;
			}
			else
				printf("\b");
		waitpadup();
	}
	// UP
	if (joypad() & J_UP) {
		if(player[1]>16)
			player[1]--;
	}
	// DOWN
	if (joypad() & J_DOWN) {
		if(player[1]<152)
			player[1]++;
	}
	// LEFT
	if (joypad() & J_LEFT) {
		if(player[0]>8)
			player[0]--;
	}
	// RIGHT
	if (joypad() & J_RIGHT) {
		if(player[0]<160)
			player[0]++;
	}
	// Move the sprite in the first movable sprite list (0)
	//  the the position of X (player[0]) and y (player[1])
	move_sprite(0, player[0], player[1]);
}
