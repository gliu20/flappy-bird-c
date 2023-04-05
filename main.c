/* This files provides address values that exist in the system */
#define SDRAM_BASE            0xC0000000
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_CHAR_BASE        0xC9000000

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define TIMER_BASE            0xFF202000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030
#define PS2_BASE              0xFF200100


/* VGA colors */
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00
#define SAND 0xFEF0
#define SKY 0x96FF 
#define LIGHT_GREEN 0xA6F0
#define DARK_GREEN 0x0460
// need add blank screen color
#define BLACK 0x0


/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

/* Flappy bird specific constants */
#define SCROLL_VIEW_AMOUNT 2

/* Score */
#define NUM_DIGITS 10

// Dimensions of the character font
#define SCORE_CHAR_WIDTH 4
#define SCORE_CHAR_HEIGHT 6

// Scaling; For example if 2, then font is drawn at twice
// its normal size
#define SCORE_CHAR_SCALE 2
#define SCORE_POS_X (RESOLUTION_X - 30)
#define SCORE_POS_Y 10

// We wait this many frames before we update
// the score as a hack to make sure we update the score
// at the same time as when a player passes a pipe
#define SCORE_UPDATE_TIME_OFFSET 10

/* Pipes */
#define NUM_PIPES 5
#define PIPE_COLOR 0x06F0
#define PIPE_WIDTH 30
#define PIPE_HEAD_HEIGHT 15
#define PIPE_VOID_HEIGHT 60
#define PIPE_SPACING 90

/* Birds */
#define BIRD_WIDTH 32
#define BIRD_HEIGHT 24
#define BIRD_INITIAL_X 20
#define BIRD_INITIAL_Y 100
#define BIRD_INITIAL_VELOCITY 0.1
#define BIRD_INITIAL_ANGLE 0
#define BIRD_VELOCUTY_FACTOR_POS 1.2
#define BIRD_VELOCUTY_FACTOR_NEG 0.8
#define BIRD_JUMP_VELOCITY 2

/* Modes */
#define MODE_MENU 0
#define MODE_GAME 1
#define MODE_GAME_OVER 2

/* Background */
#define GROUND_THICKNESS 20
#define GRASS_THICKNESS 10
#define TOTAL_FLOOR_HEIGHT (GROUND_THICKNESS + GRASS_THICKNESS)
#define SKY_THICKNESS (RESOLUTION_Y - TOTAL_FLOOR_HEIGHT)
#define GRASS_SQUARE_WIDTH 10

// This is calulated from (RESOLUTION_X) / GRASS_SQUARE_WIDTH
#define NUM_GRASS_SQUARE 34

/* Key data */
#define SPACE_KEY 0x29
#define ENTER_KEY 0x5A
#define BACK_KEY 0x66

/* Macro for absolute value */
#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Includes */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


volatile int pixel_buffer_start;
volatile int *pixel_ctrl_ptr = (int *) 0xFF203020;

// Custom font for digits of image
int digits_image[NUM_DIGITS][SCORE_CHAR_HEIGHT][SCORE_CHAR_WIDTH] = {

    // Zero
    {
        { 0, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 1, 0, 1, 0 },
        { 1, 0, 1, 0 },
        { 1, 0, 1, 0 },
        { 1, 1, 1, 0 },
    },

    // One
    {
        { 0, 0, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 1, 0 },
    },

    // Two
    {
        { 0, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 1, 0 },
        { 1, 1, 1, 0 },
        { 1, 0, 0, 0 },
        { 1, 1, 1, 0 },
    },

    // Three
    {
        { 0, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 1, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 1, 0 },
        { 1, 1, 1, 0 },
    },

    // Four
    {
        { 0, 0, 0, 0 },
        { 1, 0, 1, 0 },
        { 1, 0, 1, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 1, 0 },
    },

    // Five
    {
        { 0, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 1, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 1, 0 },
        { 1, 1, 1, 0 },
    },

    // Six
    {
        { 0, 0, 0, 0 },
        { 0, 1, 1, 0 },
        { 1, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 1, 0, 1, 0 },
        { 1, 1, 1, 0 },
    },

    // Seven
    {
        { 0, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
    },

    // Eight
    {
        { 0, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 1, 0, 1, 0 },
        { 1, 1, 1, 0 },
        { 1, 0, 1, 0 },
        { 1, 1, 1, 0 },
    },

    // Nine
    {
        { 0, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 1, 0, 1, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 1, 0 },
    }
};


// Global state
typedef struct bird {
    //(x, y) is the top left point of the bird
    int x;

    // This needs to be a double so if we have a 0.1 velocity
    // for example we don't round down
    double y; 

    // y_velocity is a double so we can
    // make gravity not jumpy
    double y_velocity;

    // For animating angle of bird as we tap
    double angle;
} bird_t;

typedef struct pipe {
    // Coordinates of the centre of void for the pipe
    int x;
    int y;

    // How wide the pipe should be
    int width;

    // The height of the void where the bird
    // can fit through
    int void_height;
} pipe_t;


typedef struct grass {
    // Unlike pipe, grass coordinates are not centered for
    // ease of programming
    int left_x;
    int right_x;
} grass_t;

typedef short int color_t;


typedef struct game_state {
    grass_t grasses[NUM_GRASS_SQUARE];
    pipe_t pipes[NUM_PIPES];
    bird_t bird;

    // Can be any of the MODE_* in the #define
    int mode;
    int score;

    // Used to keep track of when we passed a pipe
    int time_since_seen_pipe;
} game_state_t;



// Initializers
void initialize_game(game_state_t *game);
void initialize_pipe(pipe_t *pipe, int i);
void initialize_pipes(pipe_t pipes[]);
void initialize_bird(bird_t *bird);
void initialize_screen(game_state_t *game);
void initialize_grasses(grass_t grasses[]);


// Helpers
int clamp(int x, int min, int max);
void video_text(int x, int y, char * text_ptr);
void erase_menu_texts();
void erase_game_over_texts();

// Graphics
void draw_pixel(int x, int y, color_t line_color);
void draw_rect(int x0, int y0, int x1, int y1, color_t line_color);
void draw_rect_outline(int x0, int y0, int x1, int y1, color_t line_color);
void draw_pipe(pipe_t pipe);
void draw_bird(bird_t bird);
void draw_game(game_state_t *game);
void draw_game_over(game_state_t *game);
void draw_menu(game_state_t *game, bird_t bird);
void draw_grasses(grass_t grass[]);
void draw_background(game_state_t *game);
bool is_out_of_bounds(int x, int min, int max);

// Control bird's position
void do_bird_velocity(bird_t* bird);
void do_bird_jump(bird_t* bird);
bool did_collide(bird_t bird, pipe_t pipe);

// Game logic
bool is_game_over(game_state_t *game);
bool bird_in_screen(bird_t bird);
void change_mode(game_state_t *game);
void do_scroll_view(game_state_t *game);
void do_update_score(game_state_t *game);

// Screen/VGA
void next_frame();
void clear_screen();
void wait_for_vsync();

int main(void) {
    game_state_t game;

    initialize_game(&game);
    initialize_screen(&game);

    while (true) {
        clear_screen();
        switch (game.mode) {
            case MODE_GAME: draw_game(&game); break;
            case MODE_GAME_OVER: draw_game_over(&game); break;
            case MODE_MENU: draw_menu(&game, game.bird); break;

            // By default, go to menu
            default: draw_menu(&game, game.bird); break;
        }
    }
}


// Initializers
void initialize_game(game_state_t *game) {
    // TODO @debug for testing we start at MODE_GAME
    // In practice, we want to change this to MODE_MENU
    game->mode = MODE_GAME;
    game->score = 0;

    initialize_pipes(game->pipes);
    initialize_grasses(game->grasses);
    initialize_bird(&game->bird);
}

void initialize_pipe(pipe_t *pipe, int i) {
    pipe->x = i * PIPE_SPACING;
    pipe->y = rand() % (RESOLUTION_Y - PIPE_VOID_HEIGHT * 2 - TOTAL_FLOOR_HEIGHT) + PIPE_VOID_HEIGHT;
    pipe->width = PIPE_WIDTH;
    pipe->void_height = PIPE_VOID_HEIGHT;
}

void initialize_grass(grass_t *grass, int i) {
    grass->right_x = i * GRASS_SQUARE_WIDTH;  
    grass->left_x = grass->right_x - GRASS_SQUARE_WIDTH;
}

void initialize_grasses(grass_t grasses[]) {
    for (int i = 0; i < NUM_GRASS_SQUARE; i++) {
        initialize_grass(&grasses[i], i);
    }
}

void initialize_pipes(pipe_t pipes[]) {
    for (int i = 0; i < NUM_PIPES; i++) {
        initialize_pipe(&pipes[i], i);
    }
}

void initialize_bird(bird_t *bird) {
    bird->x = BIRD_INITIAL_X;
    bird->y = BIRD_INITIAL_Y;
    bird->y_velocity = BIRD_INITIAL_VELOCITY;
    bird->angle = BIRD_INITIAL_ANGLE;
}

void initialize_screen(game_state_t *game) {
    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a boxer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    draw_background(game); // pixel_buffer_start boxs to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    draw_background(game); // pixel_buffer_start boxs to the pixel buffer
}


// Graphics
inline bool is_out_of_bounds(int x, int min, int max) {
    if (x > max) return true;
    if (x < min) return true;

    return false;
}

inline void draw_pixel(int x, int y, color_t color) {
    // Don't display offscreen pixels
    if (is_out_of_bounds(x, 0, RESOLUTION_X - 1)) return;
    if (is_out_of_bounds(y, 0, RESOLUTION_Y - 1)) return;
    
    // Actually plot pixel
    *(color_t *)(pixel_buffer_start + (y << 10) + (x << 1)) = color;
}

/**
 * Draws a rectangle where the coordinates are as specified
 * Note: We expect x0 < x1 and y0 < y1
 * @param x0 - top left corner
 * @param y0 - top left corner
 * @param x1 - bottom right corner
 * @param y1 - bottom right corner
 * @param line_color - color
*/
inline void draw_rect(int x0, int y0, int x1, int y1, color_t line_color) {
    for (int x = x0; x <= x1; x++) {
        for (int y = y0; y <= y1; y++) {
            draw_pixel(x, y, line_color);
        }
    }
}

/**
 * Draws a rectangle outline where the coordinates are as specified
 * Note: We expect x0 < x1 and y0 < y1
 * @param x0 - top left corner
 * @param y0 - top left corner
 * @param x1 - bottom right corner
 * @param y1 - bottom right corner
 * @param line_color - color
*/
void draw_rect_outline(int x0, int y0, int x1, int y1, color_t line_color) {
    for (int x = x0; x <= x1; x++) {
        draw_pixel(x, y0, line_color);
        draw_pixel(x, y1, line_color);
    }
    for (int y = y0; y <= y1; y++) {
        draw_pixel(x0, y, line_color);
        draw_pixel(x1, y, line_color);
    }
}

void draw_pipe(pipe_t pipe) {
    int x0 = pipe.x - (pipe.width / 2);
    int x1 = pipe.x + (pipe.width / 2);

    // Top Pipe
    int y_screen_top = 0;
    int y_top_pipe_edge = pipe.y - (pipe.void_height / 2);

    // Bottom pipe
    int y_bottom_pipe_edge = pipe.y + (pipe.void_height / 2);
    int y_screen_bottom = RESOLUTION_Y - TOTAL_FLOOR_HEIGHT - 1;

    // Draw top pipe, outline, pipe head outline
    draw_rect(x0, y_screen_top, x1, y_top_pipe_edge, PIPE_COLOR);
    draw_rect_outline(x0, y_screen_top - 1, x1, y_top_pipe_edge - PIPE_HEAD_HEIGHT, BLACK);
    draw_rect_outline(x0 - 1, y_top_pipe_edge - PIPE_HEAD_HEIGHT, x1 + 1, y_top_pipe_edge, BLACK);

    // Draw bottom pipe
    draw_rect(x0, y_bottom_pipe_edge, x1, y_screen_bottom, PIPE_COLOR);
    draw_rect_outline(x0, y_bottom_pipe_edge + PIPE_HEAD_HEIGHT, x1, y_screen_bottom, BLACK);
    draw_rect_outline(x0 - 1, y_bottom_pipe_edge, x1 + 1, y_bottom_pipe_edge + PIPE_HEAD_HEIGHT, BLACK);
}

void draw_pipes(pipe_t pipes[]) {
    for (int i = 0; i < NUM_PIPES; i++) {
        draw_pipe(pipes[i]);
    }
}

void draw_bird(bird_t bird){
    //modified based on this to draw bird: https://www.pinterest.com/pin/559924166147577544/
    draw_rect(bird.x, bird.y + 8, bird.x + 1, bird.y + 15, BLACK);
    draw_rect(bird.x + 10, bird.y, bird.x + 21, bird.y + 1, BLACK);
    draw_rect(bird.x + 6, bird.y + 2, bird.x + 9, bird.y + 3, BLACK);
    draw_rect(bird.x + 16, bird.y + 2, bird.x + 17, bird.y + 3, YELLOW);
    draw_rect(bird.x + 18, bird.y + 2, bird.x + 19, bird.y + 3, BLACK);
    draw_rect(bird.x + 20, bird.y + 2, bird.x + 21, bird.y + 3, WHITE);
    draw_rect(bird.x + 22, bird.y + 2, bird.x + 23, bird.y + 3, BLACK); //7
    draw_rect(bird.x + 4, bird.y + 4, bird.x + 5, bird.y + 5, BLACK);
    draw_rect(bird.x + 6, bird.y + 4, bird.x + 9, bird.y + 5, YELLOW);
    draw_rect(bird.x + 16, bird.y + 4, bird.x + 17, bird.y + 9, BLACK);
    draw_rect(bird.x + 22, bird.y + 4, bird.x + 23, bird.y + 5, WHITE);
    draw_rect(bird.x + 24, bird.y + 4, bird.x + 25, bird.y + 5, BLACK); //12
    draw_rect(bird.x + 2, bird.y + 6, bird.x + 7, bird.y + 7, BLACK);
    draw_rect(bird.x + 8, bird.y + 6, bird.x + 9, bird.y + 7, YELLOW);
    draw_rect(bird.x + 18, bird.y + 10, bird.x + 19, bird.y + 11, BLACK);
    draw_rect(bird.x + 18, bird.y + 4, bird.x + 21, bird.y + 9, WHITE); 
    draw_rect(bird.x + 22, bird.y + 6, bird.x + 23, bird.y + 9, BLACK);
    draw_rect(bird.x + 24, bird.y + 6, bird.x + 25, bird.y + 11, WHITE); 
    draw_rect(bird.x + 26, bird.y + 6, bird.x + 27, bird.y + 11, BLACK); //19
    draw_rect(bird.x + 2, bird.y + 8, bird.x + 7, bird.y + 13, WHITE);
    draw_rect(bird.x + 8, bird.y + 8, bird.x + 9, bird.y + 9, BLACK);
    draw_rect(bird.x + 10, bird.y + 2, bird.x + 15, bird.y + 9, YELLOW);
    draw_rect(bird.x + 20, bird.y + 10, bird.x + 23, bird.y + 11, WHITE); //23
    draw_rect(bird.x + 8, bird.y + 10, bird.x + 9, bird.y + 13, YELLOW);
    draw_rect(bird.x + 10, bird.y + 10, bird.x + 11, bird.y + 13, BLACK);
    draw_rect(bird.x + 12, bird.y + 10, bird.x + 17, bird.y + 15, YELLOW); //26
    draw_rect(bird.x + 18, bird.y + 12, bird.x + 19, bird.y + 13, YELLOW);
    draw_rect(bird.x + 20, bird.y + 12, bird.x + 29, bird.y + 13, BLACK); //28
    draw_rect(bird.x + 2, bird.y + 14, bird.x + 7, bird.y + 15, YELLOW);
    draw_rect(bird.x + 8, bird.y + 14, bird.x + 9, bird.y + 15, BLACK);
    draw_rect(bird.x + 10, bird.y + 14, bird.x + 11, bird.y + 15, YELLOW);
    draw_rect(bird.x + 18, bird.y + 14, bird.x + 19, bird.y + 15, BLACK);
    draw_rect(bird.x + 20, bird.y + 14, bird.x + 29, bird.y + 15, ORANGE);
    draw_rect(bird.x + 30, bird.y + 14, bird.x + 31, bird.y + 15, BLACK); //34
    draw_rect(bird.x + 2, bird.y + 16, bird.x + 7, bird.y + 17, BLACK);
    draw_rect(bird.x + 8, bird.y + 16, bird.x + 15, bird.y + 19, SAND);
    draw_rect(bird.x + 16, bird.y + 16, bird.x + 17, bird.y + 17, BLACK);
    draw_rect(bird.x + 18, bird.y + 16, bird.x + 19, bird.y + 17, ORANGE);
    draw_rect(bird.x + 20, bird.y + 16, bird.x + 29, bird.y + 17, BLACK); //39
    draw_rect(bird.x + 6, bird.y + 18, bird.x + 7, bird.y + 19, BLACK);
    draw_rect(bird.x + 16, bird.y + 18, bird.x + 17, bird.y + 19, SAND);
    draw_rect(bird.x + 18, bird.y + 18, bird.x + 19, bird.y + 19, BLACK);
    draw_rect(bird.x + 20, bird.y + 18, bird.x + 27, bird.y + 19, ORANGE);
    draw_rect(bird.x + 28, bird.y + 18, bird.x + 29, bird.y + 19, BLACK); //44
    draw_rect(bird.x + 8, bird.y + 20, bird.x + 11, bird.y + 21, BLACK);
    draw_rect(bird.x + 12, bird.y + 20, bird.x + 19, bird.y + 21, SAND);
    draw_rect(bird.x + 20, bird.y + 20, bird.x + 29, bird.y + 21, BLACK);
    draw_rect(bird.x + 12, bird.y + 22, bird.x + 19, bird.y + 23, BLACK); //48
}

void draw_digit(int digit, int x_offset, int x, int y, color_t color) {
    for (int i = 0; i < SCORE_CHAR_WIDTH; i++) {
        for (int j = 0; j < SCORE_CHAR_HEIGHT; j++) {
            int x_start = x + (i + x_offset) * SCORE_CHAR_SCALE;
            int y_start = y + j * SCORE_CHAR_SCALE;
            int x_end = x_start + SCORE_CHAR_SCALE;
            int y_end = y_start + SCORE_CHAR_SCALE;

            if (digits_image[digit][j][i])
                draw_rect(x_start, y_start, x_end, y_end, color);
        }
    }
}

void draw_integer(int n, int x, int y, color_t color) {
    int x_offset = 0;
    int digit_count = 0;

    if (n == 0) return draw_digit(n, x_offset, x, y, color);

    while (n > 0) {
        int digit = n % 10;

        draw_digit(digit, x_offset, x, y, color);

        // Move to the left to render next digit. We have to move left
        // since we get digits in reverse order so we display digits
        // from right to left
        x_offset -= SCORE_CHAR_WIDTH;

        // Move on to next digit
        n = n / 10;
        digit_count++;
    }
}

/**
 * Draws the score at x, y where x, y specifies the 
 * top-right corner of the text to be drawn. This means text
 * will appear to the left of x, y
 * @param score
 * @param x
 * @param y
*/
void draw_score(int score, int x, int y) {
    // Cheat to get outline on score
    draw_integer(score, x + 1, y + 1, BLACK);
    draw_integer(score, x - 1, y - 1, BLACK);
    draw_integer(score, x + 1, y - 1, BLACK);
    draw_integer(score, x - 1, y + 1, BLACK);

    // Draw score
    draw_integer(score, x, y, WHITE);
}

void draw_game(game_state_t *game) {
    
    initialize_pipes(game->pipes);
    initialize_bird(&game->bird);

    while (!is_game_over(game)) {
        draw_background(game);
        draw_pipes(game->pipes);
        draw_bird(game->bird);
        draw_score(game->score, SCORE_POS_X, SCORE_POS_Y);

        do_scroll_view(game);
        do_bird_velocity(&game->bird);
        do_update_score(game);

        // TODO: need to add clear screen for performance
        // TODO: need code for bounce up
        next_frame();
    }

    game->mode = MODE_GAME_OVER;
}

void draw_game_over(game_state_t *game) {
    while (game -> mode == MODE_GAME_OVER) {
        draw_background(game);

        //display "GAME OVER"
        //display "SCORE: "
        //display "PRESS ENTER TO RESTART"
        //display "PRESS BACK TO GO TO MENU"
        char text_for_game_state[] = "GAME OVER\0";
        char text_for_score[] = "SCORE:\0 ";
        char text_for_restart[] = "PRESS ENTER TO RESTART\0";
        char text_for_menu[] = "PRESS BACK TO GO TO MENU\0";

       // char score[10];
        // 10 here is a string length we dont follow itoa spec
       // int_to_string(game -> score, score, 10);
       // strcat(text_for_score,score);

        //use character buffer
        video_text(35, 12, text_for_game_state);
        video_text(36, 22, text_for_score);
        video_text(29, 32, text_for_restart);
        video_text(28, 42, text_for_menu);

        //check whether Enter or Back has pressed
        change_mode(game);
        next_frame();
    }
}

void erase_game_over_texts(){
    //display "GAME OVER"
    //display "SCORE: "
    //display "PRESS ENTER TO RESTART"
    //display "PRESS BACK TO GO TO MENU"
    char text_for_game_state[] = "         \0";
    char text_for_score[] = "                        \0 ";
    char text_for_restart[] = "                      \0";
    char text_for_menu[] = "                        \0";

    //use character buffer
    video_text(35, 12, text_for_game_state);
    video_text(36, 22, text_for_score);
    video_text(29, 32, text_for_restart);
    video_text(28, 42, text_for_menu);
}

void draw_menu(game_state_t *game, bird_t bird) {
    while (game -> mode == MODE_MENU) {
        draw_background(game);
        draw_bird(bird);

        //display "FLAPPY BIRD"
        //display "PRESS ENTER TO START"
        char text_for_game_name[] = "FLAPPY BIRD\0";
        char text_to_display[] = "PRESS ENTER TO START\0";

        //use character buffer
        video_text(44, 15, text_for_game_name);
        video_text(40, 44, text_to_display);

        //check whether Enter has pressed
        change_mode(game);
        next_frame();
    }
}


void erase_menu_texts(){
    //erase "FLAPPY BIRD"
    //erase "PRESS ENTER TO START"
    char text_to_erase_game_name[] = "           \0";
    char text_to_erase_display[] = "                    \0";

    //use character buffer
    video_text(44, 15, text_to_erase_game_name);
    video_text(40, 44, text_to_erase_display);
}

void draw_grasses(grass_t grass[]){
    int grass_top = RESOLUTION_Y - TOTAL_FLOOR_HEIGHT;
    int grass_bottom = RESOLUTION_Y - GROUND_THICKNESS;

    
    // Draw grass blocks
    for (int i = 0; i < NUM_GRASS_SQUARE; i++){
        color_t grass_color = i % 2 == 0 ? LIGHT_GREEN : DARK_GREEN;

        draw_rect(
            grass[i].left_x, 
            grass_top, 
            grass[i].right_x, 
            grass_bottom, 
            grass_color
        );
    }

    // Draw grass block outlines    
    for (int i = 0; i < NUM_GRASS_SQUARE; i++){
        draw_rect_outline(
            grass[i].left_x, 
            grass_top - 1, 
            grass[i].right_x, 
            grass_bottom + 1, 
            BLACK
        );
    }
}

void draw_background(game_state_t *game) {
    //draw sky
    draw_rect(0, 0, RESOLUTION_X, SKY_THICKNESS - 1, SKY);
    //draw ground
    draw_rect(0, RESOLUTION_Y - GROUND_THICKNESS + 1, RESOLUTION_X, RESOLUTION_Y, SAND);
    //draw grass
    draw_grasses(game->grasses);
}

// Control bird's position
void do_bird_velocity(bird_t* bird){
    //update y position
    (bird -> y) = (bird -> y) + (bird -> y_velocity);

    //update y velocity
    //to simulate gravity, subtract 20% of y_velocity as bird moves 
    if((bird -> y_velocity) > 0){
        (bird -> y_velocity) = BIRD_VELOCUTY_FACTOR_POS * (bird -> y_velocity);
    }
    else if((bird -> y_velocity) == 0){
        (bird -> y_velocity) = BIRD_INITIAL_VELOCITY;
    }
    else{
        (bird -> y_velocity) = BIRD_VELOCUTY_FACTOR_NEG * (bird -> y_velocity);
    }
}

void do_bird_jump(bird_t* bird){
    //bird will jump when the user pressed space key
    volatile int * PS2_ptr = (int *)PS2_BASE;
    int PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
    int RVALID = PS2_data & 0x8000; // extract the RVALID field
    if (RVALID) {
        char key_data = PS2_data & 0xFF;
        if (key_data == (char)SPACE_KEY){
            *(PS2_ptr) = 0xF4;
            (bird -> y_velocity) = BIRD_JUMP_VELOCITY;
        }
    }
}


void do_scroll_view(game_state_t *game) {   

    // Scroll pipes 
    for (int i = 0; i < NUM_PIPES; i++) {
        pipe_t *pipe = &game->pipes[i];

        // Index NUM_PIPES is to rename this pipe to be the last pipe
        // at end of screen
        if (pipe->x < -PIPE_WIDTH / 2) {
            initialize_pipe(pipe, NUM_PIPES);

            // Before resetting when we've last seen a pipe, we must
            // request a score update so in case it's been overdue
            // we don't accidentally miss out on updating the score
            do_update_score(game);
            game->time_since_seen_pipe = 0;
        }
        pipe->x -= SCROLL_VIEW_AMOUNT;
    }

    // Scroll grasses
    grass_t *prev_grass = &game->grasses[NUM_GRASS_SQUARE - 1];
    grass_t *curr_grass;


    // Wrap around grasses
    for (int i = 0; i < NUM_GRASS_SQUARE; i++) {
        curr_grass = &game->grasses[i];

        if (curr_grass->right_x < 0) {
            // Place grass square to the right of the rightmost square
            curr_grass->left_x = prev_grass->right_x;
            curr_grass->right_x = prev_grass->right_x + GRASS_SQUARE_WIDTH;  
        }

        prev_grass = curr_grass;
    }

    // Move all grasses
    for (int i = 0; i < NUM_GRASS_SQUARE; i++) {
        grass_t *grass = &game->grasses[i];

        grass->left_x -= SCROLL_VIEW_AMOUNT;
        grass->right_x -= SCROLL_VIEW_AMOUNT;
    }
        
}

void do_update_score(game_state_t *game) {
    if (game->time_since_seen_pipe++ > SCORE_UPDATE_TIME_OFFSET) {
        game->score++;
        game->time_since_seen_pipe = 0;
    }
}

/**
 * return true when the bird and the pipe collide, return false when they don't collide
*/
bool did_collide(bird_t bird, pipe_t pipe){
    //these four lines form a rectangle of void space between top pipe and bottom pipe
    //pipe_void_x1 < pipe_void_x2, pipe_void_y1 < pipe_void_y2
    int pipe_void_x1 = pipe.x - (pipe.width / 2);
    int pipe_void_x2 = pipe.x + (pipe.width / 2);
    int pipe_void_y1 = pipe.y - (pipe.void_height / 2);
    int pipe_void_y2 = pipe.y + (pipe.void_height / 2);

    //check whether the bird and the pipe collides
    //the bird hasn't reached the pipe or the bird has already passed the pipe
    if(bird.x + BIRD_WIDTH - 1 < pipe_void_x1 || bird.x > pipe_void_x2){
        return false;
    }
    //some portion of the bird is in between the top and bottom pipe
    else{
        //y coordinate of the bird is within the void region
        if(bird.y >= pipe_void_y1 && bird.y + BIRD_HEIGHT - 1 <= pipe_void_y2){
            return false;
        }
        //the bird and the pipe collide
        else{
            return true;
        }
    }
}


inline bool bird_in_screen(bird_t bird) {
    return is_out_of_bounds(bird.y, 0, RESOLUTION_Y - TOTAL_FLOOR_HEIGHT);
}

// Game logic
inline bool is_game_over(game_state_t *game) {
    for (int i = 0; i < NUM_PIPES; i++) {
        if (did_collide(game->bird, game->pipes[i]))
            return true;
    }

    return bird_in_screen(game->bird);
}


void change_mode(game_state_t *game){
    volatile int * PS2_ptr = (int *)PS2_BASE;
    int PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
    int RVALID = PS2_data & 0x8000; // extract the RVALID field
    if (RVALID) {
        char key_data = PS2_data & 0xFF;
        //Enter has pressed when the mode is menu
        if((game -> mode) == MODE_MENU&& key_data == (char)ENTER_KEY){
            erase_menu_texts();
            *(PS2_ptr) = 0xF4;
            (game -> mode) = MODE_GAME;
            (game->score) = 0;
            initialize_pipes(game->pipes);
            initialize_grasses(game->grasses);
            initialize_bird(&game->bird);
        }
        else if((game -> mode) == MODE_GAME_OVER && key_data == (char)ENTER_KEY){
            erase_game_over_texts();
            *(PS2_ptr) = 0xF4;
            (game -> mode) = MODE_GAME;
            (game->score) = 0;
            initialize_pipes(game->pipes);
            initialize_grasses(game->grasses);
            initialize_bird(&game->bird);
        }
        else if ((game -> mode) == MODE_GAME_OVER && key_data == (char)BACK_KEY){
            erase_game_over_texts();
            *(PS2_ptr) = 0xF4;
            (game -> mode) = MODE_MENU;
            (game->score) = 0;
            initialize_pipes(game->pipes);
            initialize_grasses(game->grasses);
            initialize_bird(&game->bird);
        }
    }
}

// Screen/VGA
void clear_screen() {
    for (int x = 0; x < RESOLUTION_X; x++) {
        for (int y = 0; y < RESOLUTION_Y; y++) {
            draw_pixel(x, y, BLACK);
        }
    }
}

void next_frame() {
    // Swap front and back buffers on vsync and update buffer pointer
    wait_for_vsync();
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);
}

void wait_for_vsync() {
    volatile int *status = pixel_ctrl_ptr + 3;

    // Write one to buffer register to request Vsync
    *pixel_ctrl_ptr = 1;

    // Poll for Vsync
    while (true) {
        // Wait until S bit becomes zero
        if (((*status) & 1) == 0) return;
    }
}

//taken from DE1-SoC_Computer_ARM.pdf
void video_text(int x, int y, char * text_ptr) {
    int offset;
    volatile char * character_buffer = (char *)FPGA_CHAR_BASE; // video character buffer
    /* assume that the text string fits on one line */
    offset = (y << 7) + x;
    while (*(text_ptr)) {
        *(character_buffer + offset) = *(text_ptr); // write to the character buffer
        ++text_ptr;
        ++offset;
    }
}

