//States
	enum State{
		INITIAL_PLACEMENT,
		FIRST_PIECE_B4_ROLL,
		FIRST_PIECE_AFTER_ROLL,
		FIRST_PIECE_PICKED,
		FIRST_PIECE_PLACED,
		SECOND_PIECE_PICKED,
		MISPLACED
		};
	
	
	// Colors
	enum Color{
		OPTION = 5,
		INVALID = 4,
		OCCUPIED = 5,
		NONE = 5
	};
	
	// Constants
	const int N_ROWS = 7;						// Number of Rows
	const int N_COLS = 7;						// Number of Columns
	
	// Vars
	uint8_t current_row = 0;					// Current row index
	uint8_t current_row_values = 0;				// Switch values at the current row
	uint8_t dice[2] = {0,0};					// Two die
	State	prev_state = INITIAL_PLACEMENT,		// Game States
			curr_state = INITIAL_PLACEMENT,
			next_state = INITIAL_PLACEMENT;
	char[]  displayString = "";					// String to display to the LCD screen
	
	// The "grid" represents information about each square on the grid
	struct square_t {
		uint8_t color;
		bool old_occupied;
		bool new_occupied;
		bool placement_option;
		bool valid;
	} grid[N_ROWS][N_COLS];
	
	// Misc.
	uint8_t i = 0;		// Loop iterator
	
	
	// onRowChange() should be called every time the row change timer expires. All the processing is done *before* the row changes,
	// allowing time for the signals to propagate before processing them.
	void onRowChange(){
		//TODO: get current row values
		// current_row_values = ??;
		
		// Loop through each column
		for( i = 0 ; i < N_COLS ; i++ ){
			// Assign old and new occupancy values
			grid[current_row][i].old_occupied = grid[current_row][i].new_occupied;
			grid[current_row][i].new_occupied = (current_row_values & 1);
			// Shift the row values to read the next column on the next iteration
			current_row_values >> 1;
			
			// TODO: Check if a piece was picked or placed, update according to algorithm
			if(grid[current_row][i].old_occupied != grid[current_row][i].new_occupied){
				// True if piece was placed
				if(grid[current_row][i].new_occupied){
					//
				}
				else // True if piece was picked
				{
					//
				}
			}
			
			// Update lighting values
			if(grid[current_row][i].placement_option)
				grid[current_row][i].color = Color.OPTION;
			else if(grid[current_row][i].new_occupied){
				if (grid[current_row][i].valid)
					grid[current_row][i].color = Color.OCCUPIED;
				else
					grid[current_row][i].color = Color.INVALID;
			}
			else
				grid[current_row][i].color = Color.NONE;
		}
		
		// TODO:
		updateLEDs(); 
		
		//TODO: change the row in GPIO
		current_row = (current_row + 1) % N_ROWS;
	}
	
	//TODO:
	rollDice(*dice);
	
	//TODO:
	updateDisplay(displayString);
	
	void main(){
		//TODO: initialize GPIOS
		//TODO: initialize TWI
		//TODO: initialize addressable LEDs
		//TODO: setup repeating interrupt timer
		//TODO: enable interrupts
		//TODO: start timer
	}
}