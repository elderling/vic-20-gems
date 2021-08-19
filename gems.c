#include <vic20.h>
#include <conio.h>
#include <peekpoke.h>

#define EMPTY_SLOT 0
#define PLAYFIELD_X 8
#define PLAYFIELD_Y 8
#define CURSOR_UP 0x91
#define CURSOR_DOWN 0x11
#define CURSOR_LEFT 0x9d
#define CURSOR_RIGHT 0x1d
#define RETURN_KEY 0x0d
#define NUMBER_OF_GEMS 7
#define PETSCII_FOR_ZERO_CHAR 48
#define RASTER_REGISTER_LO 0x9004
#define START_CHAR 1

struct coordinate {
    char x;
    char y;
};

enum gamestate { first_selection, second_selection };

void initialize_display(void);
void initialize_game_state(void);
void initialize_playfield(void);
void render_display(void);
char get_command(void);
void do_command(char command);
void draw_playfield(void);
char up_down_match( struct coordinate *gem_location, char gem );
char left_right_match( struct coordinate *gem_location, char gem );
char double_down_match( struct coordinate *gem_location, char gem );
char double_up_match( struct coordinate *gem_location, char gem );
char double_left_match( struct coordinate *gem_location, char gem );
char double_right_match( struct coordinate *gem_location, char gem );
char gem_matches( struct coordinate *gem_location, char gem);
void update_raster_rand(void);
void randomize_playfield(void);
void swap_gems( struct coordinate *from, struct coordinate *to );
char is_valid_swap( struct coordinate *from, struct coordinate *to );
void notify_invalid( void );
char populate_match_grid( void );
void remove_gems( void );

char raster_rand;
enum gamestate the_game_state;

char playfield[PLAYFIELD_X][PLAYFIELD_Y];
char match_grid[PLAYFIELD_X][PLAYFIELD_Y];

struct coordinate game_cursor;
struct coordinate first_gem;
struct coordinate second_gem;

unsigned int game_score = 0;

int main()
{

    the_game_state = first_selection;
    initialize_display();
    initialize_game_state();
    initialize_playfield();
    randomize_playfield();
    render_display();

    while (1) {
        do_command(get_command());
        render_display();
    }

    return 0;
}

void initialize_display()
{

    clrscr();
    gotoxy( 0, 0 );
    bgcolor( COLOR_BLACK );
    bordercolor( COLOR_BLACK );
    cursor(1);

    return;
}

void initialize_game_state()
{
    game_cursor.x = 0;
    game_cursor.y = 0;

    return;
}

void render_display()
{
    draw_playfield();
    gotoxy(0, PLAYFIELD_Y + 1);
    textcolor( COLOR_WHITE );
    cprintf("x=%2d,y=%2d", game_cursor.x, game_cursor.y);
    gotoxy(0, PLAYFIELD_Y + 2);
    cprintf("x=%2d,y=%2d", first_gem.x, first_gem.y);
    gotoxy(0, PLAYFIELD_Y + 3);
    cprintf("x=%2d,y=%2d", second_gem.x, second_gem.y);
    gotoxy(0, PLAYFIELD_Y + 4);
    cprintf("score=%5d", game_score);
    gotoxy( game_cursor.x, game_cursor.y );
}

char get_command()
{
    return cgetc();
}

void do_command(char command)
{
    char removed_gems = 0;
    char score = 0;

    if ( command == CURSOR_UP && game_cursor.y != 0 ) {
        game_cursor.y--;
    } else if ( command == CURSOR_DOWN && game_cursor.y != PLAYFIELD_Y - 1) {
        game_cursor.y++;
    } else if ( command == CURSOR_LEFT && game_cursor.x != 0 ) {
        game_cursor.x--;
    } else if ( command == CURSOR_RIGHT && game_cursor.x != PLAYFIELD_X - 1 ) {
        game_cursor.x++;
    } else if ( command == RETURN_KEY ) {
        if ( the_game_state == first_selection ) {
            first_gem.x = game_cursor.x;
            first_gem.y = game_cursor.y;
            the_game_state = second_selection;
        } else if ( the_game_state == second_selection ) {
            second_gem.x = game_cursor.x;
            second_gem.y = game_cursor.y;
            if ( is_valid_swap( &first_gem, &second_gem ) ) {

                swap_gems( &first_gem, &second_gem );

                while ( score = populate_match_grid() ) {
                    game_score += score;
                    remove_gems();
                    removed_gems = 1;
                }

                if (!removed_gems) {
                    swap_gems( &first_gem, &second_gem );
                    notify_invalid();
                }

                randomize_playfield();

            } else {
                notify_invalid();
            }
            the_game_state = first_selection;
        }
    }

    return;
}

void initialize_playfield()
{
    char x,y;
    for (x = 0; x < PLAYFIELD_X; x++) {
        for (y = 0; y < PLAYFIELD_Y; y++) {
            playfield[x][y] = EMPTY_SLOT;
        }
    }
}

void draw_playfield()
{

    char x,y;
    for (x = 0; x < PLAYFIELD_X; x++) {
        for (y = 0; y < PLAYFIELD_Y; y++) {
            textcolor(playfield[x][y]);
            cputcxy(x, y, playfield[x][y] + PETSCII_FOR_ZERO_CHAR);
        }
    }
}

char gem_matches( struct coordinate *gem_location, char gem)
{
    if ( up_down_match( gem_location, gem) ) {
        return 1;
    }
    if ( left_right_match( gem_location, gem) ) {
        return 1;
    }
    if (up_down_match( gem_location, gem) ) {
        return 1;
    }
    if (double_left_match( gem_location, gem) ) {
        return 1;
    }
    if (double_right_match( gem_location, gem) ) {
        return 1;
    }
    if (double_up_match( gem_location, gem) ) {
        return 1;
    }
    if (double_down_match( gem_location, gem) ) {
        return 1;
    }
    return 0;
}

char up_down_match( struct coordinate *gem_location, char gem )
{
    char x = gem_location->x;
    char y = gem_location->y;
    if ( y == 0 || y == PLAYFIELD_Y - 1 ) {
        return 0;
    }
    if ( playfield[x][y-1] == gem && playfield[x][y+1] == gem ) {
        return 1;
    }

    return 0;
}

char left_right_match( struct coordinate *gem_location, char gem)
{
    char x = gem_location->x;
    char y = gem_location->y;

    if ( x == 0 || y == PLAYFIELD_X - 1 ) {
        return 0;
    }
    if ( playfield[x-1][y] == gem && playfield[x+1][y] == gem ) {
        return 1;
    }

    return 0;
}

char double_up_match( struct coordinate *gem_location, char gem)
{
    char x = gem_location->x;
    char y = gem_location->y;

    if ( y > PLAYFIELD_Y - 3  ) {
        return 0;
    }
    if ( playfield[x][y+1] == gem && playfield[x][y+2] == gem ) {
        return 1;
    }

    return 0;
}

char double_down_match( struct coordinate *gem_location, char gem )
{
    char x = gem_location->x;
    char y = gem_location->y;

    if ( y < 2 ) {
        return 0;
    }

    if ( playfield[x][y-1] == gem && playfield[x][y-2] == gem ) {
        return 1;
    }

    return 0;
}

char double_left_match( struct coordinate *gem_location, char gem)
{
    char x = gem_location->x;
    char y = gem_location->y;

    if ( x < 2 ) {
        return 0;
    }
    if ( playfield[x-1][y] == gem && playfield[x-2][y] == gem ) {
        return 1;
    }

    return 0;
}

char double_right_match( struct coordinate *gem_location, char gem)
{
    char x = gem_location->x;
    char y = gem_location->y;

    if ( x > PLAYFIELD_X - 3 ) {
        return 0;
    }

    if ( playfield[x+1][y] == gem && playfield[x+2][y] == gem ) {
        return 1;
    }

    return 0;
}

void update_raster_rand()
{
    raster_rand = ((raster_rand >> 7) + raster_rand << 1) ^ PEEK(RASTER_REGISTER_LO);
}

void randomize_playfield()
{
    char x,y;
    char potential_gem;
    struct coordinate gem_location;
    for (x = 0; x < PLAYFIELD_X; x++) {
        for (y = 0; y < PLAYFIELD_Y; y++) {
            if (playfield[x][y] != EMPTY_SLOT ) {
                continue;
            }
            update_raster_rand();
            potential_gem = START_CHAR + raster_rand % NUMBER_OF_GEMS;
            gem_location.x = x;
            gem_location.y = y;
            while ( gem_matches(&gem_location, potential_gem) ) {
                potential_gem++;
                if ( potential_gem > ( START_CHAR + NUMBER_OF_GEMS - 1 )) {
                    potential_gem = START_CHAR;
                }
            }
            playfield[x][y] = potential_gem;
        }
    }
}

void swap_gems( struct coordinate *from, struct coordinate *to )
{
    char temp;

    temp = playfield[from->x][from->y];
    playfield[from->x][from->y] = playfield[to->x][to->y];
    playfield[to->x][to->y] = temp;

    return;
}

char is_valid_swap( struct coordinate *from, struct coordinate *to )
{

    if ( from->x == to->x && from->y == to->y ) {
        return 0;
    }

    if ( from->x == to->x ) {
        if ( to->y == from->y + 1 || to->y == from->y - 1 ) {
            return 1;
        }
    }

    if ( from->y == to->y ) {
        if ( to->x == from->x + 1 || to->x == from->x - 1 ) {
            return 1;
        }
    }

    return 0;
}

void notify_invalid ()
{
    bgcolor(COLOR_RED);

    cgetc();

    bgcolor(COLOR_BLACK);
    return;
}

char populate_match_grid (void)
{
    char x, y, gem;
    char found_matches = 0;

    struct coordinate the_coordinate;

    for (x = 0; x < PLAYFIELD_X; x++) {
        for (y = 0; y < PLAYFIELD_Y; y++) {
            gem = playfield[x][y];
            if ( gem == EMPTY_SLOT ) {
                match_grid[x][y] = 0;
                continue;
            }
            the_coordinate.x = x;
            the_coordinate.y = y;
            if ( gem_matches( &the_coordinate, gem) ) {
                found_matches++;
                match_grid[x][y] = 1;
            } else {
                match_grid[x][y] = 0;
            }
        }
    }

    return found_matches;
}

void remove_gems()
{
    signed char stack_top, x, y;

    for ( x = 0; x < PLAYFIELD_X; x++ ) {
        stack_top = PLAYFIELD_Y - 1;
        for ( y = PLAYFIELD_Y - 1; y >= 0; y-- ) {
            if ( match_grid[x][y] == 1 ) {
                continue;
            }
            playfield[x][stack_top] = playfield[x][y];
            stack_top--;
        }

        for ( y = stack_top; y >= 0; y-- ) {
            playfield[x][y] = EMPTY_SLOT;
        }
    }

    return;
}
