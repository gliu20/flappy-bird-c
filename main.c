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
#define PIPE_SPACING 140
#define PIPE_START_X 140

/* Birds */
#define BIRD_WIDTH 32
#define BIRD_HEIGHT 24
#define BIRD_INITIAL_X 20
#define BIRD_INITIAL_Y 100
#define BIRD_INITIAL_VELOCITY 0.1
#define BIRD_INITIAL_ANGLE 0
#define BIRD_JUMP_VELOCITY -2
#define BIRD_GRAVITY 0.1

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
#define BACK_SPACE_KEY 0x66

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

    // Keeps track of whether we already added going
    // through this pipe to the score
    bool did_score_update;
} pipe_t;


typedef struct grass {
    // Unlike pipe, grass coordinates are not centered for
    // ease of programming
    int left_x;
    int right_x;
} grass_t;


typedef struct cloud {
    int left_x;
    int top_y;

    int type;
} cloud_t;

typedef short int color_t;


typedef struct game_state {
    grass_t grasses[NUM_GRASS_SQUARE];
    pipe_t pipes[NUM_PIPES];
    bird_t bird;

    // Can be any of the MODE_* in the #define
    int mode;
    int score;
    int best_score;

    // Used to keep track of when we passed a pipe
    int time_since_seen_pipe;
} game_state_t;



// Helpers
bool bird_in_screen(bird_t bird);
bool did_collide(bird_t bird, pipe_t pipe);
bool is_game_over(game_state_t *game);
bool is_out_of_bounds(int x, int min, int max);
void change_mode(game_state_t *game);

// Game logic
void do_bird_jump(bird_t* bird);
void do_bird_velocity(bird_t* bird);
void do_scroll_clouds(game_state_t *game);
void do_scroll_grasses(game_state_t *game);
void do_scroll_pipes(game_state_t *game);
void do_scroll_view(game_state_t *game);
void do_update_best_score(game_state_t *game);
void do_update_score(game_state_t *game);

// Draw code
void draw_background(game_state_t *game);
void draw_bird(bird_t bird);
void draw_digit(int digit, int x_offset, int x, int y, color_t color);
void draw_flappy_bird(int x, int y, color_t line_color);
void draw_game(game_state_t *game);
void draw_game_over(game_state_t *game);
void draw_grasses(grass_t grass[]);
void draw_integer(int n, int x, int y, color_t color);
void draw_menu(game_state_t *game, bird_t bird);
void draw_pipe(pipe_t pipe);
void draw_pipes(pipe_t pipes[]);
void draw_pixel(int x, int y, color_t color);
void draw_rect(int x0, int y0, int x1, int y1, color_t line_color);
void draw_rect_outline(int x0, int y0, int x1, int y1, color_t line_color);
void draw_score(int score, int x, int y);
void draw_slanted_rect(int x0, int y0, int x1, int y1, color_t color);
void draw_slanted_rect_outline(int x0, int y0, int x1, int y1, color_t line_color);
void draw_word_game_over(int x, int y, color_t line_color);

// Erase text code
void erase_game_over_texts();
void erase_menu_texts();

// Initializers 
void initialize_bird(bird_t *bird);
void initialize_game(game_state_t *game);
void initialize_grass(grass_t *grass, int i);
void initialize_grasses(grass_t grasses[]);
void initialize_pipe(pipe_t *pipe, int i);
void initialize_pipes(pipe_t pipes[]);
void initialize_screen(game_state_t *game);


// Screen/VGA
void clear_read_FIFO();
void clear_screen();
void next_frame();
void video_text(int x, int y, char * text_ptr);
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
    game->mode = MODE_MENU;
    game->score = 0;
    game->best_score = 0;

    initialize_pipes(game->pipes);
    initialize_grasses(game->grasses);
    initialize_bird(&game->bird);
}

void initialize_pipe(pipe_t *pipe, int i) {
    pipe->x = i * PIPE_SPACING + PIPE_START_X;
    pipe->y = rand() % (RESOLUTION_Y - PIPE_VOID_HEIGHT * 2 - TOTAL_FLOOR_HEIGHT) + PIPE_VOID_HEIGHT;
    pipe->width = PIPE_WIDTH;
    pipe->void_height = PIPE_VOID_HEIGHT;
    pipe->did_score_update = false;
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
 * Draws a slanted rectangle where the coordinates are as specified
 * Note: We expect x0 < x1 and y0 < y1
 * @param x0 - top left corner
 * @param y0 - top left corner
 * @param x1 - bottom right corner
 * @param y1 - bottom right corner
 * @param line_color - color
*/
inline void draw_slanted_rect(int x0, int y0, int x1, int y1, color_t color) {
    for (int x = x0; x <= x1; x++) {
        for (int y = y0, i = 0; y <= y1; y++, i++) {
            draw_pixel(x - i, y, color);
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


/**
 * Draws a slanted rectangle outline
 * Note: We expect x0 < x1 and y0 < y1
 * @param x0 - top left corner
 * @param y0 - top left corner
 * @param x1 - bottom right corner
 * @param y1 - bottom right corner
 * @param line_color - color
*/
void draw_slanted_rect_outline(int x0, int y0, int x1, int y1, color_t line_color) {
    // TODO: known bug: we dont properly draw the horizontal lines of a slanted
    // rect in the right spots but this bug is not visually observable since
    // this is only used for drawing grass
    for (int x = x0; x <= x1; x++) {
        draw_pixel(x, y0, line_color);
        draw_pixel(x, y1, line_color);
    }
    for (int y = y0, i = 0; y <= y1; y++, i++) {
        draw_pixel(x0 - i, y, line_color);
        draw_pixel(x1 - i, y, line_color);
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
    clear_read_FIFO();
    initialize_pipes(game->pipes);
    initialize_bird(&game->bird);

    while (!is_game_over(game)) {
        draw_background(game);
        draw_pipes(game->pipes);
        draw_bird(game->bird);
        draw_score(game->score, SCORE_POS_X, SCORE_POS_Y);

        do_scroll_view(game);
        do_bird_jump(&game->bird);
        do_bird_velocity(&game->bird);
        do_update_score(game);

        // TODO: need to add clear screen for performance
        // TODO: need code for bounce up
        next_frame();
    }

    game->mode = MODE_GAME_OVER;
}

void draw_game_over(game_state_t *game) {
    clear_read_FIFO();
    do_update_best_score(game);
    while (game -> mode == MODE_GAME_OVER) {
        draw_background(game);

        //display "GAME OVER"
        //display "SCORE: "
        //display "BEST: "
        //display "PRESS ENTER TO RESTART"
        //display "PRESS BACK TO GO TO MENU"
        draw_word_game_over(50, 30, WHITE);
        char text_for_score[] = "SCORE:\0";
        char text_for_best_score[] = "BEST:\0";
        char text_for_restart[] = "PRESS ENTER TO PLAY AGAIN\0";
        char text_for_menu[] = "PRESS BACKSPACE TO GO TO MENU\0";

       // char score[10];
        // 10 here is a string length we dont follow itoa spec
       // int_to_string(game -> score, score, 10);
       // strcat(text_for_score,score);

        //use character buffer
        video_text(36, 22, text_for_score);
        video_text(37, 27, text_for_best_score);
        video_text(29, 32, text_for_restart);
        video_text(26, 42, text_for_menu);

        //check whether Enter or Back has pressed
        change_mode(game);
        next_frame();
    }
}

void erase_game_over_texts(){
    //erase "SCORE: "
    //erase "BEST: "
    //erase "PRESS ENTER TO RESTART"
    //erase "PRESS BACK TO GO TO MENU"
    char text_for_score[] = "                        \0 ";
    char text_for_best_score[] = "                     \0 ";
    char text_for_restart[] = "                         \0";
    char text_for_menu[] = "                             \0";

    //use character buffer
    video_text(36, 22, text_for_score);
    video_text(37, 27, text_for_best_score);
    video_text(29, 32, text_for_restart);
    video_text(26, 42, text_for_menu);
}

void draw_menu(game_state_t *game, bird_t bird) {
    clear_read_FIFO();
    while (game -> mode == MODE_MENU) {
        draw_background(game);
        draw_bird(bird);

        //display "FLAPPY BIRD"
        //display "PRESS SPACE TO LET THE BIRD JUMP"
        //display "PRESS ENTER TO START"
        draw_flappy_bird(87, 45, BLACK);
        draw_flappy_bird(89, 45, BLACK);
        draw_flappy_bird(88, 44, BLACK);
        draw_flappy_bird(88, 46, BLACK);
        draw_flappy_bird(88, 45, WHITE);
        char text_for_instruction[] = "PRESS SPACE TO LET THE BIRD JUMP\0";
        char text_to_display[] = "PRESS ENTER TO START\0";

        //use character buffer
        video_text(30, 30, text_for_instruction);
        video_text(36, 44, text_to_display);

        //check whether Enter has pressed
        change_mode(game);
        next_frame();
    }
}


void erase_menu_texts(){
    //erase "PRESS SPACE TO LET THE BIRD JUMP"
    //erase "PRESS ENTER TO START"
    char text_to_erase_instruction[] = "                                 \0";
    char text_to_erase_display[] = "                    \0";

    //use character buffer
    video_text(30, 30, text_to_erase_instruction);
    video_text(36, 44, text_to_erase_display);
}

void draw_grasses(grass_t grass[]){
    int grass_top = RESOLUTION_Y - TOTAL_FLOOR_HEIGHT;
    int grass_bottom = RESOLUTION_Y - GROUND_THICKNESS;

    
    // Draw grass blocks
    for (int i = 0; i < NUM_GRASS_SQUARE; i++){
        color_t grass_color = i % 2 == 0 ? LIGHT_GREEN : DARK_GREEN;

        draw_slanted_rect(
            grass[i].left_x, 
            grass_top, 
            grass[i].right_x, 
            grass_bottom, 
            grass_color
        );
    }

    // Draw grass block outlines    
    for (int i = 0; i < NUM_GRASS_SQUARE; i++){
        draw_slanted_rect_outline(
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
    (bird -> y_velocity) += BIRD_GRAVITY;
}

void do_bird_jump(bird_t* bird){
    //bird will jump when the user pressed space key
    volatile int * PS2_ptr = (int *)PS2_BASE;
    int PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
    int RVALID = PS2_data & 0x8000; // extract the RVALID field
    if (RVALID) {
        char key_data = PS2_data & 0xFF;
        if (key_data == (char)SPACE_KEY){
            (bird -> y) -= 7;
            (bird -> y_velocity) -= 0.1;
        }
    }
}


void do_scroll_pipes(game_state_t *game) {

    pipe_t *prev_pipe = &game->pipes[NUM_PIPES - 1];
    pipe_t *curr_pipe;
    
    // Wrap around pipes
    for (int i = 0; i < NUM_PIPES; i++) {
        pipe_t *curr_pipe = &game->pipes[i];

        if (curr_pipe->x < -PIPE_WIDTH / 2) {
            // Place pipe to the right of the previous pipe
            // since we know the previous pipe is the one that's on
            // the very right of the screen and is offscreen
            curr_pipe->did_score_update = false;
            curr_pipe->x = prev_pipe->x + PIPE_SPACING;
        }

        prev_pipe = curr_pipe;
    }

    // Scroll pipes
    for (int i = 0; i < NUM_PIPES; i++) {
        pipe_t *pipe = &game->pipes[i];

        pipe->x -= SCROLL_VIEW_AMOUNT;
    }
}

void do_scroll_grasses(game_state_t *game) {
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

void do_scroll_clouds(game_state_t *game) {

}

void do_scroll_view(game_state_t *game) {   
    do_scroll_pipes(game);
    do_scroll_grasses(game);
    do_scroll_clouds(game);
}

void do_update_score(game_state_t *game) {
    for (int i = 0; i < NUM_PIPES; i++) {
        pipe_t *pipe = &game->pipes[i];

        int bird_center_x = game->bird.x + BIRD_WIDTH / 2;
        bool did_pipe_pass_bird = pipe->x < bird_center_x;
        bool did_score_update = pipe->did_score_update;

        if (did_pipe_pass_bird && !did_score_update) {
            pipe->did_score_update = true;
            game->score++;
        }
    }
}

void do_update_best_score(game_state_t *game){
    if (game->score > game->best_score) {
        game->best_score = game->score;
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
        if((game -> mode) == MODE_MENU && key_data == (char)ENTER_KEY){
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
        else if ((game -> mode) == MODE_GAME_OVER && key_data == (char)BACK_SPACE_KEY){
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

void clear_read_FIFO(){
    volatile int * PS2_ptr = (int *)PS2_BASE;
    *(PS2_ptr) = 0xF5; //disable keyboard input
    //clear read FIFO
    int PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
    int RVALID = PS2_data & 0x8000; // extract the RVALID field
    while (RVALID) {
        PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
        RVALID = PS2_data & 0x8000; // extract the RVALID field
    }
    *(PS2_ptr) = 0xF4; //enable keyboard input
}

void draw_flappy_bird(int x, int y, color_t line_color){
    //draw F
    draw_rect(x + 1, y, x + 3, y + 17, line_color);
    draw_rect(x + 3, y, x + 14, y + 2, line_color);
    draw_rect(x + 3, y + 7, x + 12, y + 9, line_color);
    //draw L
	draw_rect(x + 20, y, x + 22, y + 17, line_color);
    draw_rect(x + 22, y + 15, x + 32, y + 17, line_color);
    //draw A
    draw_rect(x + 44, y, x + 48, y + 1, line_color); 
    draw_rect(x + 43, y + 2, x + 45, y + 4, line_color);
    draw_rect(x + 42, y + 5, x + 44, y + 7, line_color);
    draw_rect(x + 41, y + 8, x + 43, y + 10, line_color);
    draw_rect(x + 40, y + 11, x + 42, y + 13, line_color);
    draw_rect(x + 39, y + 14, x + 41, y + 16, line_color);
    draw_rect(x + 38, y + 17, x + 40, y + 17, line_color);
    draw_rect(x + 47, y + 2, x + 49, y + 4, line_color);
    draw_rect(x + 48, y + 5, x + 50, y + 7, line_color);
    draw_rect(x + 49, y + 8, x + 51, y + 10, line_color);
    draw_rect(x + 50, y + 11, x + 52, y + 13, line_color);
    draw_rect(x + 51, y + 14, x + 53, y + 16, line_color);
    draw_rect(x + 52, y + 17, x + 54, y + 17, line_color);
    draw_rect(x + 43, y + 9, x + 49, y + 11, line_color);
    //draw P
    draw_rect(x + 58, y, x + 60, y + 17, line_color); 
    draw_rect(x + 61, y, x + 66, y + 1, line_color);
    draw_rect(x + 67, y + 1, x + 69, y + 1, line_color);
    draw_rect(x + 67, y + 2, x + 70, y + 2, line_color);
    draw_rect(x + 69, y + 3, x + 71, y + 3, line_color);
    draw_rect(x + 69, y + 4, x + 71, y + 5, line_color);
    draw_rect(x + 69, y + 6, x + 71, y + 6, line_color);
    draw_rect(x + 67, y + 7, x + 70, y + 7, line_color);
    draw_rect(x + 67, y + 8, x + 69, y + 8, line_color);
    draw_rect(x + 61, y + 8, x + 66, y + 9, line_color);
    //draw P
    draw_rect(x + 77, y, x + 79, y + 17, line_color); 
    draw_rect(x + 80, y, x + 85, y + 1, line_color);
    draw_rect(x + 86, y + 1, x + 88, y + 1, line_color);
    draw_rect(x + 86, y + 2, x + 89, y + 2, line_color);
    draw_rect(x + 88, y + 3, x + 90, y + 3, line_color);
    draw_rect(x + 88, y + 4, x + 90, y + 5, line_color);
    draw_rect(x + 88, y + 6, x + 90, y + 6, line_color);
    draw_rect(x + 86, y + 7, x + 89 , y + 7, line_color);
    draw_rect(x + 86, y + 8, x + 88, y + 8, line_color);
    draw_rect(x + 80, y + 8, x + 85, y + 9, line_color);
    //draw Y
    draw_rect(x + 95, y, x + 97, y, line_color); 
    draw_rect(x + 96, y + 1, x + 98, y + 2, line_color);
    draw_rect(x + 97, y + 3, x + 99, y + 3, line_color);
    draw_rect(x + 98, y + 4, x + 100, y + 5, line_color);
    draw_rect(x + 99, y + 6, x + 101, y + 6, line_color);
    draw_rect(x + 100, y + 7, x + 102, y + 7, line_color);
    draw_rect(x + 101, y + 8, x + 104, y + 8, line_color);
    draw_rect(x + 108, y, x + 110, y, line_color);
    draw_rect(x + 107, y + 1, x + 109, y + 2, line_color);
    draw_rect(x + 106, y + 3, x + 108, y + 3, line_color);
    draw_rect(x + 105, y + 4, x + 107, y + 5, line_color);
    draw_rect(x + 104, y + 6, x + 106, y + 6, line_color);
    draw_rect(x + 103, y + 7, x + 105, y + 7, line_color);
    draw_rect(x + 101, y + 9, x + 104, y + 17, line_color);

    //starting point of B
    int bird_x = x + 130;

    //draw B
    draw_rect(bird_x + 1, y, bird_x + 3, y + 17, line_color); 
    draw_rect(bird_x + 4, y, bird_x + 9, y + 1, line_color);
    draw_rect(bird_x + 10, y + 1, bird_x + 12, y + 1, line_color);
    draw_rect(bird_x + 10, y + 2, bird_x + 13, y + 2, line_color);
    draw_rect(bird_x + 12, y + 3, bird_x + 14, y + 3, line_color);
    draw_rect(bird_x + 12, y + 4, bird_x + 14, y + 5, line_color);
    draw_rect(bird_x + 12, y + 6, bird_x + 14, y + 6, line_color);
    draw_rect(bird_x + 10, y + 7, bird_x + 13, y + 7, line_color);
    draw_rect(bird_x + 4, y + 8, bird_x + 12, y + 9, line_color);
    draw_rect(bird_x + 4, y + 16, bird_x + 9, y + 17, line_color);
    draw_rect(bird_x + 10, y + 16, bird_x + 12, y + 16, line_color);
    draw_rect(bird_x + 10, y + 15, bird_x + 13, y + 15, line_color);
    draw_rect(bird_x + 12, y + 14, bird_x + 14, y + 14, line_color);
    draw_rect(bird_x + 12, y + 12, bird_x + 14, y + 13, line_color);
    draw_rect(bird_x + 12, y + 11, bird_x + 14, y + 11, line_color);
    draw_rect(bird_x + 10, y + 10, bird_x + 13, y + 10, line_color);
    //draw I
    draw_rect(bird_x + 21, y, bird_x + 30, y + 2, line_color); 
    draw_rect(bird_x + 24, y + 2, bird_x + 27, y + 15, line_color);
    draw_rect(bird_x + 21, y + 15, bird_x + 30, y + 17, line_color);
    //draw R
    draw_rect(bird_x + 37, y, bird_x + 39, y + 17, line_color); 
    draw_rect(bird_x + 40, y, bird_x + 45, y + 1, line_color);
    draw_rect(bird_x + 46, y + 1, bird_x + 48, y + 1, line_color);
    draw_rect(bird_x + 46, y + 2, bird_x + 49, y + 2, line_color);
    draw_rect(bird_x + 48, y + 3, bird_x + 50, y + 3, line_color);
    draw_rect(bird_x + 48, y + 4, bird_x + 50, y + 5, line_color);
    draw_rect(bird_x + 48, y + 6, bird_x + 50, y + 6, line_color);
    draw_rect(bird_x + 46, y + 7, bird_x + 49, y + 7, line_color);
    draw_rect(bird_x + 46, y + 8, bird_x + 48, y + 8, line_color);
    draw_rect(bird_x + 40, y + 8, bird_x + 45, y + 9, line_color);
    draw_rect(bird_x + 43, y + 9, bird_x + 45, y + 10, line_color);
	draw_rect(bird_x + 44, y + 10, bird_x + 46, y + 10, line_color);
    draw_rect(bird_x + 45, y + 11, bird_x + 47, y + 11, line_color);
    draw_rect(bird_x + 46, y + 12, bird_x + 48, y + 13, line_color);
    draw_rect(bird_x + 47, y + 14, bird_x + 49, y + 14, line_color);
    draw_rect(bird_x + 48, y + 15, bird_x + 50, y + 16, line_color);
    draw_rect(bird_x + 49, y + 17, bird_x + 51, y + 17, line_color);
    //draw D
    draw_rect(bird_x + 57, y, bird_x + 59, y + 17, line_color); 
    draw_rect(bird_x + 60, y, bird_x + 63, y + 1, line_color);
    draw_rect(bird_x + 64, y + 1, bird_x + 65, y + 2, line_color);
    draw_rect(bird_x + 65, y + 2, bird_x + 67, y + 3, line_color); 
    draw_rect(bird_x + 66, y + 3, bird_x + 68, y + 4, line_color);
    draw_rect(bird_x + 67, y + 5, bird_x + 69, y + 6, line_color);
    draw_rect(bird_x + 68, y + 7, bird_x + 70, y + 10, line_color);
    draw_rect(bird_x + 60, y + 16, bird_x + 63, y + 17, line_color);
    draw_rect(bird_x + 64, y + 15, bird_x + 65, y + 16, line_color);
    draw_rect(bird_x + 65, y + 14, bird_x + 67, y + 15, line_color); 
    draw_rect(bird_x + 66, y + 13, bird_x + 68, y + 14, line_color);
    draw_rect(bird_x + 67, y + 11, bird_x + 69, y + 12, line_color);
}

void draw_word_game_over(int x, int y, color_t line_color){
    //draw G 
    draw_rect(x + 1, y + 6, x + 3, y + 11, line_color);
    draw_rect(x + 2, y + 3, x + 4, y + 5, line_color);
    draw_rect(x + 4, y + 1, x + 5, y + 3, line_color);
    draw_rect(x + 6, y, x + 6, y + 2, line_color);
    draw_rect(x + 7, y, x + 10, y + 1, line_color);  
    draw_rect(x + 11, y, x + 11, y + 2, line_color);
    draw_rect(x + 12, y + 1, x + 12, y + 3, line_color);
    draw_rect(x + 13, y + 2, x + 14, y + 3, line_color);
    draw_rect(x + 2, y + 12, x + 4, y + 14, line_color);
    draw_rect(x + 4, y + 14, x + 5, y + 16, line_color);
    draw_rect(x + 6, y + 15, x + 6, y + 17, line_color);
    draw_rect(x + 7, y + 16, x + 10, y + 17, line_color);  
    draw_rect(x + 10, y + 15, x + 11, y + 17, line_color);
    draw_rect(x + 12, y + 14, x + 13, y + 16, line_color);
    draw_rect(x + 14, y + 13, x + 15, y + 14, line_color);
    draw_rect(x + 14, y + 8, x + 16, y + 12, line_color);
    draw_rect(x + 9, y + 8, x + 14, y + 10, line_color);
    //draw A
    draw_rect(x + 26, y, x + 30, y + 1, line_color); 
    draw_rect(x + 25, y + 2, x + 27, y + 4, line_color);
    draw_rect(x + 24, y + 5, x + 26, y + 7, line_color);
    draw_rect(x + 23, y + 8, x + 25, y + 10, line_color);
    draw_rect(x + 22, y + 11, x + 24, y + 13, line_color);
    draw_rect(x + 21, y + 14, x + 23, y + 16, line_color);  
    draw_rect(x + 20, y + 17, x + 22, y + 17, line_color);
    draw_rect(x + 29, y + 2, x + 31, y + 4, line_color);
    draw_rect(x + 30, y + 5, x + 32, y + 7, line_color);
    draw_rect(x + 31, y + 8, x + 33, y + 10, line_color);
    draw_rect(x + 32, y + 11, x + 34, y + 13, line_color);
    draw_rect(x + 33, y + 14, x + 35, y + 16, line_color);
    draw_rect(x + 34, y + 17, x + 36, y + 17, line_color);
    draw_rect(x + 25, y + 9, x + 31, y + 11, line_color);
    //draw M
    draw_rect(x + 40, y, x + 42, y + 17, line_color); 
    draw_rect(x + 43, y, x + 43, y + 2, line_color);
    draw_rect(x + 44, y + 1, x + 45, y + 9, line_color);
    draw_rect(x + 45, y + 10, x + 46, y + 15, line_color);
    draw_rect(x + 47, y + 15, x + 48, y + 17, line_color);
    draw_rect(x + 53, y, x + 55, y + 17, line_color); 
    draw_rect(x + 52, y, x + 52, y + 2, line_color);
    draw_rect(x + 50, y + 1, x + 51, y + 9, line_color);
    draw_rect(x + 49, y + 10, x + 50, y + 15, line_color);
    //draw E
    draw_rect(x + 60, y, x + 62, y + 17, line_color);
    draw_rect(x + 62, y, x + 73, y + 2, line_color);
    draw_rect(x + 62, y + 7, x + 71, y + 9, line_color);
    draw_rect(x + 62, y + 15, x + 73, y + 17, line_color);

    int over_x = x + 76;

    //draw O
    draw_rect(over_x + 1, y + 6, over_x + 3, y + 11, line_color);
    draw_rect(over_x + 2, y + 3, over_x + 4, y + 5, line_color);
    draw_rect(over_x + 4, y + 1, over_x + 5, y + 3, line_color);
    draw_rect(over_x + 6, y, over_x + 6, y + 2, line_color);
    draw_rect(over_x + 7, y, over_x + 10, y + 1, line_color);  
    draw_rect(over_x + 11, y, over_x + 11, y + 2, line_color);
    draw_rect(over_x + 12, y + 1, over_x + 13, y + 3, line_color);
    draw_rect(over_x + 13, y + 2, over_x + 14, y + 3, line_color);
    draw_rect(over_x + 2, y + 12, over_x + 4, y + 14, line_color);
    draw_rect(over_x + 4, y + 14, over_x + 5, y + 16, line_color);
    draw_rect(over_x + 6, y + 15, over_x + 6, y + 17, line_color);
    draw_rect(over_x + 7, y + 16, over_x + 10, y + 17, line_color);  
    draw_rect(over_x + 10, y + 15, over_x + 11, y + 17, line_color);
    draw_rect(over_x + 12, y + 14, over_x + 13, y + 16, line_color);
    draw_rect(over_x + 14, y + 13, over_x + 15, y + 14, line_color);
 
    //draw V
    //draw E
    /*
    draw_rect(x + 60, y, x + 62, y + 17, line_color);
    draw_rect(x + 62, y, x + 73, y + 2, line_color);
    draw_rect(x + 62, y + 7, x + 71, y + 9, line_color);
    draw_rect(x + 62, y + 15, x + 73, y + 17, line_color);
    //draw R
    draw_rect(over_x + 37, y, over_x + 39, y + 17, line_color); 
    draw_rect(over_x + 40, y, over_x + 45, y + 1, line_color);
    draw_rect(over_x + 46, y + 1, over_x + 48, y + 1, line_color);
    draw_rect(over_x + 46, y + 2, over_x + 49, y + 2, line_color);
    draw_rect(over_x + 48, y + 3, over_x + 50, y + 3, line_color);
    draw_rect(over_x + 48, y + 4, over_x + 50, y + 5, line_color);
    draw_rect(over_x + 48, y + 6, over_x + 50, y + 6, line_color);
    draw_rect(over_x + 46, y + 7, over_x + 49, y + 7, line_color);
    draw_rect(over_x + 46, y + 8, over_x + 48, y + 8, line_color);
    draw_rect(over_x + 40, y + 8, over_x + 45, y + 9, line_color);
    draw_rect(over_x + 43, y + 9, over_x + 45, y + 10, line_color);
	draw_rect(over_x + 44, y + 10, over_x + 46, y + 10, line_color);
    draw_rect(over_x + 45, y + 11, over_x + 47, y + 11, line_color);
    draw_rect(over_x + 46, y + 12, over_x + 48, y + 13, line_color);
    draw_rect(over_x + 47, y + 14, over_x + 49, y + 14, line_color);
    draw_rect(over_x + 48, y + 15, over_x + 50, y + 16, line_color);
    draw_rect(over_x + 49, y + 17, over_x + 51, y + 17, line_color);
    */
}