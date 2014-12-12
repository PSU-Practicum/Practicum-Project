#include "all_the_crap_at_the_top.h"



void makeSetupPattern(void){
	for(i = 0; i < N_ROWS; i++){
		for(j = 0; j < N_ROWS; j++){
			if (setupPattern[i][j])
			{
				setColor(i,j, COLOR_SETUP);
			}
			else setColor(i,j, COLOR_BLANK);
		}
	}
	ws2812_setleds(led,NUM_LEDS);
}

//case vars
int num_correct = 0;
int num_incorrect = 0;
uint8_t curr_x, curr_y;

/********************* MAIN *********************/
int main(void)
{
	board_init();
	ioport_init();
	makeSetupPattern();
	ioport_set_pin_dir(ROLL_PIN, IOPORT_DIR_INPUT);

	
	// Indicate a refresh 
	{
	setColor(0,0, 0xff0000);
	ws2812_setleds(led,NUM_LEDS);
	_delay_ms(300);
	setColor(0,0, 0x00000);
	ws2812_setleds(led,NUM_LEDS);
	}
		
	while(1){
		checkAllSquares();
		switch (curr_state)
		{
			case INITIAL_PLACEMENT							:
				num_correct = 0;
				num_incorrect = 0;
				for (i = 0; i < N_ROWS; i++){
					for (j = 0; j < N_COLS; j++){
						if (setupPattern[i][j]){
							if (grid[i][j].occupied){
								setColor(i,j, COLOR_READY);
								num_correct++;
							}
							else {
								setColor(i,j, COLOR_SETUP);
							}
						}
						else {
							if (grid[i][j].occupied){
								setColor(i,j, COLOR_INVALID);
								num_incorrect++;
							}
							else {
								setColor(i,j, 0);
								
							}
						}
					}
				}
				if (num_correct == 7 && num_incorrect == 0){
					for(i=0;i<N_COLS;i++){for(j=0;j<N_ROWS;j++){
						if (setupPattern[i][j]) setColor(i,j, COLOR_OCCUPIED);
						}}
					direction = rand()&1;
					switchPlayerAnimation();
					curr_state = ROLL_DICE;
				}
				break;
				
			case ROLL_DICE									:
				if(change){
					makeSetupPattern();
					curr_state = INITIAL_PLACEMENT;
				}

				if(ioport_get_pin_level(ROLL_PIN)){
					winAnimation();
					    dice[0] = (abs(rand()) / (RAND_MAX / (MAX_DICE - 1))) + 1;
					 if((dice[1] = (abs(rand()) / (RAND_MAX / (MAX_DICE - 1))) + 1) == dice[0])
						switchPlayerAnimation();
					else
						curr_state = FIRST_MOVE_BEFORE_PICK;
					break;
				}
				break;
				
				
				
			case FIRST_MOVE_BEFORE_PICK						:
				if(!change) 
					break; // No change, do nothing.
				
					// Save the square that was lifted
				orig_loc[x] = loc[x];
				orig_loc[y] = loc[y];
				setColor(loc[x], loc[y], COLOR_BLANK); // blank the square that was lifted

				// Illuminate possible options
				for(i=0;i<2;i++){for(j=0;j<2;j++){for( k = -1; k<2; k += 2 ){ // Triple nest loop
					curr_x = loc[x] + k*j*dice[i];
					curr_y = loc[y] + k*(!j)*dice[i];
					if (!grid[curr_x][curr_y].occupied			// Not already a piece there
						&&	(curr_x >= 0 && curr_x < N_COLS)	// Is not out of bounds
						&&	(curr_y >= 0 && curr_y < N_ROWS))
					{
						setColor( curr_x, curr_y, COLOR_READY);
						grid[curr_x][curr_y].placement_option = true;
						grid[curr_x][curr_y].die = i;
					}
				}
				}
				}
				curr_state = FIRST_MOVE_AFTER_PICK;
				break;


			case FIRST_MOVE_AFTER_PICK						:
				if(!change)
					break; // No change, do nothing.
					
					// Player picked up another piece
				if( !grid[loc[0]][loc[1]].occupied ){
					err_loc[x] = loc[x];
					err_loc[y] = loc[y];
					setColor(loc[x],loc[y], COLOR_INVALID);
					curr_state = INVALID_PICKED_SECOND_PIECE_IN_FIRST_MOVE;
					break;
				}
					// Player put the piece back in the same spot
				else if ( loc[x] == orig_loc[x] && loc[y] == orig_loc[y] )
				{
					// Undo valid placements
					for(i=0;i<N_COLS;i++){for(j=0;j<N_ROWS;j++){
						if (grid[i][j].placement_option)
						{
							setColor(i,j, COLOR_BLANK);
							grid[i][j].placement_option = false;
						}
					}}
						// Set placed piece back to occupied
					setColor(loc[x],loc[y], COLOR_OCCUPIED);
					curr_state = FIRST_MOVE_BEFORE_PICK;
					break;
				}
					// Placed in invalid location
				else if ( !grid[loc[0]][loc[1]].placement_option )
				{
					err_loc[x] = loc[x];
					err_loc[y] = loc[y];
					setColor(loc[x],loc[y], COLOR_INVALID);
					curr_state = INVALID_PLACEMENT_IN_FIRST_MOVE;
					break;
				}

				// Placed in valid location
				else if ( grid[loc[0]][loc[1]].placement_option ){
					if (isAligned()){
						winAnimation();
						reset();
						break;
					}
					// Undo valid placements
					for(i=0;i<N_COLS;i++){for(j=0;j<N_ROWS;j++){
						if (grid[i][j].placement_option)
						{
							setColor(i,j, COLOR_BLANK);
							grid[i][j].placement_option = false;
						}
					}}
					// Set placed piece back to occupied
					setColor(loc[x],loc[y], COLOR_OCCUPIED);

					// Record which die value was used
					die_of_first_move = grid[loc[0]][loc[1]].die;

					// Record location of first piece
					first_loc[x] = loc[x];
					first_loc[y] = loc[y];

					// Record which axis was taken
					if(loc[x] == orig_loc[x])
						axis_of_first_movement = x;
					else if (loc[y] == orig_loc[y])
						axis_of_first_movement = y;
					else
						badBreak();
					curr_state = SECOND_MOVE_BEFORE_PICK;
					break;
				}
				badBreak();
				
			case SECOND_MOVE_BEFORE_PICK					:
				if(!change)
					break; // No change, do nothing.
				
				if ((loc[x] == first_loc[x]) && (loc[y] == first_loc[y]))
				{
					err_loc[x] = loc[x];
					err_loc[y] = loc[y];
					setColor(loc[x],loc[y], COLOR_INVALID);
					curr_state = INVALID_PICKED_WRONG_PIECE_IN_SECOND_MOVE;
					break;
				}

				// Save the square that was lifted
				orig_loc[x] = loc[x];
				orig_loc[y] = loc[y];
				setColor(loc[x], loc[y], COLOR_BLANK); // blank the square that was lifted

				// Illuminate possible options
				for( k = -1; k<2; k += 2 ){
					if (axis_of_first_movement == x)
					{
						curr_x = loc[x] + k*dice[!die_of_first_move];
						curr_y = loc[y];
					}
					else
					{
						curr_y = loc[y] + k*dice[!die_of_first_move];
						curr_x = loc[x];
					}
					if (!grid[curr_x][curr_y].occupied			// Not already a piece there
						&&	(curr_x >= 0 && curr_x < N_COLS)	// Is not out of bounds
						&&	(curr_y >= 0 && curr_y < N_ROWS))
					{
						setColor( curr_x, curr_y, COLOR_READY);
						grid[curr_x][curr_y].placement_option = true;
					}
				}
				curr_state = SECOND_MOVE_AFTER_PICK;
				break;


			case SECOND_MOVE_AFTER_PICK						:
				if(!change)
				break; // No change, do nothing.
				
				// Player picked up another piece
				if( !grid[loc[0]][loc[1]].occupied ){
					err_loc[x] = loc[x];
					err_loc[y] = loc[y];
					setColor(loc[x],loc[y], COLOR_INVALID);
					curr_state = INVALID_PICKED_SECOND_PIECE_IN_SECOND_MOVE;
					break;
				}
				// Player put the piece back in the same spot
				else if ( loc[x] == orig_loc[x] && loc[y] == orig_loc[y] )
				{
					// Undo valid placements
					for(i=0;i<N_COLS;i++){for(j=0;j<N_ROWS;j++){
						if (grid[i][j].placement_option)
						{
							setColor(i,j, COLOR_BLANK);
							grid[i][j].placement_option = false;
						}
					}}
					// Set placed piece back to occupied
					setColor(loc[x],loc[y], COLOR_OCCUPIED);
					curr_state = SECOND_MOVE_BEFORE_PICK;
					break;
				}
				// Placed in invalid location
				else if ( !grid[loc[0]][loc[1]].placement_option )
				{
					err_loc[x] = loc[x];
					err_loc[y] = loc[y];
					setColor(loc[x],loc[y], COLOR_INVALID);
					curr_state = INVALID_PLACEMENT_IN_SECOND_MOVE;
					break;
				}
				// Placed in valid location
				else if ( grid[loc[0]][loc[1]].placement_option ){
					if (isAligned()){
						winAnimation();
						reset();
						break;
					}

					// Undo valid placements
					for(i=0;i<N_COLS;i++){for(j=0;j<N_ROWS;j++){
						if (grid[i][j].placement_option)
						{
							setColor(i,j, COLOR_BLANK);
							grid[i][j].placement_option = false;
						}
					}}
					// Set placed piece back to occupied
					setColor(loc[x],loc[y], COLOR_OCCUPIED);
					curr_state = ROLL_DICE;
					break;
				}
				badBreak();
				break;


			case INVALID_PICKED_A_PIECE_BEFORE_ROLL			:
				break;


			case INVALID_PLACEMENT_IN_FIRST_MOVE			:
				if(!change)
				break; // No change, do nothing.

				// Player picked up the invalid piece, continue the game
				if (loc[x] == err_loc[x] && loc[y] == err_loc[y])
				{
					setColor(loc[x], loc[y], COLOR_BLANK);
					curr_state = FIRST_MOVE_AFTER_PICK;
					break;
				}
				else reset();
				break;


			case INVALID_PICKED_SECOND_PIECE_IN_FIRST_MOVE	:
				if(!change)
					break; // No change, do nothing.
				
				// Player picked up another piece, just restart the game
				if( !grid[loc[0]][loc[1]].occupied ){
					reset();
					break;
				}

				// Player put the piece back, continue the game
				else if (loc[x] == err_loc[x] && loc[y] == err_loc[y])
				{
					setColor(loc[x], loc[y], COLOR_OCCUPIED);
					curr_state = FIRST_MOVE_AFTER_PICK;
					break;
				}
				break;


			case INVALID_PLACEMENT_IN_SECOND_MOVE			:
				if(!change)
				break; // No change, do nothing.

				// Player picked up the invalid piece, continue the game
				if (loc[x] == err_loc[x] && loc[y] == err_loc[y])
				{
					setColor(loc[x], loc[y], COLOR_BLANK);
					curr_state = SECOND_MOVE_AFTER_PICK;
					break;
				}
				else reset();
				break;


			case INVALID_PICKED_SECOND_PIECE_IN_SECOND_MOVE	:
				if(!change)
				break; // No change, do nothing.
				
				// Player picked up another piece, just restart the game
				if( !grid[loc[0]][loc[1]].occupied ){
					reset();
					break;
				}

				// Player put the piece back, continue the game
				else if (loc[x] == err_loc[x] && loc[y] == err_loc[y])
				{
					setColor(loc[x], loc[y], COLOR_OCCUPIED);
					curr_state = SECOND_MOVE_AFTER_PICK;
					break;
				}
				break;


			case INVALID_PICKED_WRONG_PIECE_IN_SECOND_MOVE	:
				if(!change)
				break; // No change, do nothing.

				// Player picked up the invalid piece, continue the game
				if (loc[x] == err_loc[x] && loc[y] == err_loc[y])
				{
					setColor(loc[x], loc[y], COLOR_OCCUPIED);
					curr_state = SECOND_MOVE_BEFORE_PICK;
					break;
				}
				else reset();
				break;
		}
		//setAllColorsFromGrid();
		ws2812_setleds(led,NUM_LEDS);
		if (change) _delay_ms(50); // Debounce
	}
}





