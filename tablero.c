1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
83
84
85
86
87
88
89
90
91
92
93
94
95
96
97
98
99
100
101
102
103
104
105
106
107
108
109
110
111
112
113
114
115
116
117
118
119
120
121
122
123
124
125
126
127
128
129
130
131
132
133
134
135
136
137
138
139
140
141
142
143
144
145
146
147
148
149
150
151
152
153
154
155
156
157
158
159
160
161
162
163
164
165
166
167
168
169
170
171
172
173
174
175
176
177
178
179
180
181
182
183
184
185
186
187
188
189
190
191
192
193
194
195
196
197
198
199
200
201
202
203
204
205
206
207
208
209
210
211
212
213
214
215
216
217
218
219
220
221
222
223
224
225
226
227
228
229
230
231
232
233
234
235
236
237
238
239
240
241
242
243
244
245
246
247
248
#include "FastLED.h"
#include <stdint.h>
#define NUM_LEDS 50
#define ROWS 3
#define DATA_PIN 3
#define ROLL_PIN 4
CRGB leds[NUM_LEDS];
 
 
//States
    enum State{
        INITIAL_PLACEMENT,
        ROLL_DICE,
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
    const int N_ROWS = 7;                       // Number of Rows
    const int N_COLS = 7;                       // Number of Columns
    const int LCD_NUM_CHARS = 25;
 
    const int setupPattern[N_ROWS][N_COLS] = {
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0}
    };  
     
    // Vars
    uint8_t current_row = 0;                    // Current row index
    uint8_t current_row_values = 0;             // Switch values at the current row
    uint8_t dice[2] = {0,0};                    // Two die
    State   prev_state = INITIAL_PLACEMENT,     // Game States
            curr_state = INITIAL_PLACEMENT,
            next_state = INITIAL_PLACEMENT;
    char    displayString[LCD_NUM_CHARS + 1] = "";                  // String to display to the LCD screen
     
    // The "grid" represents information about each square on the grid
    struct square_t {
        uint8_t color;
        bool old_occupied;
        bool new_occupied;
        bool placement_option;
        bool valid;
    } grid[N_ROWS][N_COLS];
     
    // Misc.
    uint8_t i = 0;      // Loop iterator
     
     
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
                grid[current_row][i].color = OPTION;
            else if(grid[current_row][i].new_occupied){
                if (grid[current_row][i].valid)
                    grid[current_row][i].color = OCCUPIED;
                else
                    grid[current_row][i].color = INVALID;
            }
            else
                grid[current_row][i].color = NONE;
        }
         
        // TODO:
        updateLEDs(); 
         
        //TODO: change the row in GPIO
        current_row = (current_row + 1) % N_ROWS;
    }
     
    // function to roll dice
    void rollDice(uint8_t dice[2])
    {
        dice[0] = random(1,6);
        delay(2);
        dice[1] = random(1,6);
    }
     
    //TODO:
    updateDisplay(displayString);
     
    void main(){
 
        // if this goes back to Arduino, this part needs to be "setup"
          FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
   
          // initialize the pushbutton pin as an input
          pinMode(buttonPin, INPUT);
   
          // initialize the pins for reed switch matrix input and output
          pinMode(11, INPUT);
          pinMode(12, INPUT);
          pinMode(13, INPUT);
          pinMode(8, OUTPUT);
          pinMode(9, OUTPUT);
          pinMode(10, OUTPUT);
   
          digitalWrite(8, LOW);
          digitalWrite(9, LOW);
          digitalWrite(10, LOW);
   
          // set all LEDs to off when power on
          for (int i = 0; i < NUM_LEDS; i++){
            leds[i] = CRGB::Black;
          }
          FastLED.show();
 
 
 
 
        //TODO: initialize GPIOS
        //TODO: initialize TWI
        //TODO: initialize addressable LEDs
        //TODO: setup repeating interrupt timer
        //TODO: enable interrupts
        //TODO: start timer
    }
     
     
     
     
 
    uint16_t XY( uint8_t x, uint8_t y)
    {
      uint16_t i;
 
        if( y & 0x01) {
          // Odd rows run backwards
          uint8_t reverseX = (N_COLS - 1) - x;
          i = (y * N_COLS) + reverseX;
        } else {
          // Even rows run forwards
          i = (y * N_COLS) + x;
        }
   
      return i;
    }
 
 
     
     
    void checkAllSquares(){
        for (int i = 0; i < ROWS; i++){
        pinMode(i + 8, OUTPUT);
        digitalWrite(i + 8, HIGH);
        delay(2);
                for (int j = 0; j < ROWS; j++){
            if (digitalRead(11 + j))
                leds[XY(i,j)] = CRGB::Red;
            else
                leds[XY(i,j)] = CRGB::Black;
                }
        digitalWrite(i + 8, LOW);
        delay(2);
        pinMode(i + 8, INPUT);
        delay(2);
    }
 
 
         
  FastLED.show();
 
     
    void gamePlay(){
        switch (curr_state)
        {
             case INITIAL_PLACEMENT:
                int num_correct = 0;
                int num_incorrect = 0;
                for (i = 0; i < N_ROWS; i++){
                    for (j = 0; j < N_COLS; j++){
                        if (setupPattern[i][j]){
                            if (grid[i][j].new_occupied){
                                grid[i][j].color = OCCUPIED;
                                num_correct++;
                            }
                            else {
                                grid[i][j].color = OPTION;
                            }
                        }
                        else {
                            if (grid[i][j].new_occupied){
                                grid[i][j].color = INVALID;
                                num_incorrect++;
                            }
                            else {
                                grid[i][j].color = NONE;
                            }
                        }
                    }
                }
                if (num_correct == 7 && num_incorrect == 0) next_state == ROLL_DICE;
            break;
            case ROLL_DICE:
                // TODO:
 
 
 
        }
         
    }
     
     
     
}
