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
/* Pipes */
#define NUM_PIPES 5
#define PIPE_COLOR GREEN
#define PIPE_WIDTH 30
#define PIPE_VOID_HEIGHT 40
#define PIPE_SPACING 90

/* Birds */
#define BIRD_WIDTH 34
#define BIRD_HEIGHT 24
#define BIRD_INITIAL_X 20
#define BIRD_INITIAL_Y 100
#define BIRD_INITIAL_VELOCITY -1
#define BIRD_INITIAL_ANGLE 0
#define BIRD_VELOCUTY_FACTOR_POS 0.8
#define BIRD_VELOCUTY_FACTOR_NEG 1.2
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
#define NUM_GRASS_SQUARE 33

/* Macro for absolute value */
#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Includes */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


volatile int pixel_buffer_start;
volatile int * pixel_ctrl_ptr = (int *)0xFF203020;

// Global state
typedef struct bird {
    //(x, y) is the top left point of the bird
    int x;
    int y;

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

typedef short int color_t;


typedef struct game_state {
    pipe_t pipes[NUM_PIPES];
    bird_t bird;

    // Can be any of the MODE_* in the #define
    int mode;
    int score;
} game_state_t;



// Initializers
void initialize_game(game_state_t *game);
void initialize_pipe(pipe_t *pipe, int i);
void initialize_pipes(pipe_t pipes[]);
void initialize_bird(bird_t *bird);
void initialize_screen();


// Helpers
int clamp(int x, int min, int max);
void video_text(int x, int y, char * text_ptr);

// Graphics
void draw_pixel(int x, int y, color_t line_color);
void draw_rect(int x0, int y0, int x1, int y1, color_t line_color);
void draw_pipe(pipe_t pipe);
void draw_bird(bird_t bird);
void draw_game(game_state_t *game);
void draw_game_over(game_state_t *game);
void draw_menu(game_state_t *game);
void draw_grass();
void draw_background();

// Control bird's position
void do_bird_velocity(bird_t* bird);
void do_bird_jump(bird_t* bird);
bool did_collide(bird_t bird, pipe_t pipe);

// Game logic
bool is_game_over(game_state_t *game);
void change_mode(game_state_t *game);

// Screen/VGA
void next_frame();
void clear_screen();
void wait_for_vsync();

int main(void) {
    game_state_t game;

    initialize_game(&game);
    initialize_screen();

    while (true) {
        clear_screen();
        switch (game.mode) {
            case MODE_GAME: draw_game(&game); break;
            case MODE_GAME_OVER: draw_game_over(&game); break;
            case MODE_MENU: draw_menu(&game); break;

            // By default, go to menu
            default: draw_menu(&game); break;
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
    initialize_bird(&game->bird);
}

void initialize_pipe(pipe_t *pipe, int i) {
    pipe->x = i * PIPE_SPACING;
    pipe->y = rand() % (RESOLUTION_Y - PIPE_VOID_HEIGHT * 2 - TOTAL_FLOOR_HEIGHT) + PIPE_VOID_HEIGHT;
    pipe->width = PIPE_WIDTH;
    pipe->void_height = PIPE_VOID_HEIGHT;
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

void initialize_screen() {
    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync(pixel_ctrl_ptr);
    /* initialize a boxer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start boxs to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen(); // pixel_buffer_start boxs to the pixel buffer
}


// Graphics
int clamp(int x, int min, int max) {
    if (x > max) return max;
    if (x < min) return min;

    return x;
}

void draw_pixel(int x, int y, color_t line_color) {
    // Clamp safely ensures we dont go out of bounds of screen
    x = clamp(x, 0, RESOLUTION_X - 1);
    y = clamp(y, 0, RESOLUTION_Y - 1);
    
    // Actually plot pixel
    *(color_t *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
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
void draw_rect(int x0, int y0, int x1, int y1, color_t line_color) {
    for (int x = x0; x < x1; x++) {
        for (int y = y0; y < y1; y++) {
            draw_pixel(x, y, line_color);
        }
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

    // Draw top pipe
    draw_rect(x0, y_screen_top, x1, y_top_pipe_edge, PIPE_COLOR);

    // Draw bottom pipe
    draw_rect(x0, y_bottom_pipe_edge, x1, y_screen_bottom, PIPE_COLOR);
}

void draw_pipes(pipe_t pipes[]) {
    for (int i = 0; i < NUM_PIPES; i++) {
        draw_pipe(pipes[i]);
    }
}

void draw_bird(bird_t bird){
    //will use this * 2 (in terms of the size) to draw bird: https://www.pinterest.com/pin/559924166147577544/

    draw_rect(bird.x, bird.y, bird.x + BIRD_WIDTH - 1, bird.y + BIRD_HEIGHT - 1, ORANGE);
}

void draw_game(game_state_t *game) {
    
    initialize_pipes(game->pipes);
    initialize_bird(&game->bird);

    while (!is_game_over(game)) {
        draw_background();
        draw_pipes(game->pipes);
        draw_bird(game->bird);

        // TODO: need implement scrolling and call that
        // TODO: need to add clear screen for performance
        // TODO: need code for bounce up
        next_frame();
    }
}

void draw_game_over(game_state_t *game) {
    while (game -> mode == MODE_GAME_OVER) {
        draw_background();

        //display "GAME OVER"
        //display "SCORE: "
        //display "PRESS ENTER TO RESTART"
        //display "PRESS BACK TO GO TO MENU"
        char text_for_game_state[] = "GAME OVER\0";
        char text_for_score[] = "SCORE:\0 ";
        char text_for_restart[] = "PRESS ENTER TO RESTART\0";
        char text_for_menu[] = "PRESS BACK TO GO TO MENU\0";

        //use character buffer

        next_frame();
    }
}

void draw_menu(game_state_t *game) {
    while (game -> mode == MODE_MENU) {
        draw_background();

        //display "FLAPPY BIRD"
        //display "PRESS ENTER TO START"
        char text_for_game_name[] = "FLAPPY BIRD\0";
        char text_to_display[] = "PRESS ENTER TO START\0";

        //use character buffer
        video_text(44, 15, text_for_game_name);
        video_text(40, 44, text_to_display);

        next_frame();
    }
}

void draw_grass(){
    for (int i = 0; i < NUM_GRASS_SQUARE; i++){
        int ith_grass_end_x = i * GRASS_SQUARE_WIDTH - 1;
        int ith_grass_start_x = ith_grass_end_x - GRASS_SQUARE_WIDTH + 1;
        color_t grass_color = i % 2 == 0 ? LIGHT_GREEN : DARK_GREEN;

        draw_rect(
            ith_grass_start_x, 
            RESOLUTION_Y - TOTAL_FLOOR_HEIGHT, 
            ith_grass_end_x, 
            RESOLUTION_Y - GROUND_THICKNESS, 
            grass_color
        );
    }
}

void draw_background() {
    //draw ground
    draw_rect(0, RESOLUTION_Y - GROUND_THICKNESS + 1, RESOLUTION_X, RESOLUTION_Y, SAND);
    //draw grass
    draw_grass();
    //draw sky
    draw_rect(0, 0, RESOLUTION_X, SKY_THICKNESS - 1, SKY);
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
        if (key_data == (char)0x29){
            (bird -> y_velocity) = BIRD_JUMP_VELOCITY;
        }
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


// Game logic
bool is_game_over(game_state_t *game) {
    return false;
}


void change_mode(game_state_t *game){
    volatile int * PS2_ptr = (int *)PS2_BASE;
    int PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
    int RVALID = PS2_data & 0x8000; // extract the RVALID field
    if (RVALID) {
        char key_data = PS2_data & 0xFF;
        //Enter has pressed when the mode is menu
        if(((game -> mode) == MODE_MENU || (game -> mode) == MODE_GAME_OVER) && key_data == (char)0x5A){
            (game -> mode) = MODE_GAME;
        }
        else if ((game -> mode) == MODE_GAME_OVER && key_data == (char)0x66){
            (game -> mode) = MODE_MENU;
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
    wait_for_vsync(pixel_ctrl_ptr);
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

