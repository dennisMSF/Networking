

#include <termios.h>
#include <string.h>

/* Color s */
#define BLACK        0
#define BLUE         1
#define GREEN        2
#define CYAN         3
#define RED          4
#define MAGENTA      5
#define BROWN        6
#define LIGHTGRAY    7
#define DARKGRAY     8
#define LIGHTBLUE    9
#define LIGHTGREEN   10
#define LIGHTCYAN    11
#define LIGHTRED     12
#define LIGHTMAGENTA 13
#define YELLOW       14
#define WHITE        15

#define UNDERLINE    64
#define BLINK        128

#define UNDERLINE_ATTR 4
#define BLINK_ATTR     5
#define BRILLO_ATTR    1

#define BRILLO_MIN     9

#define EXIT_CODE 1

typedef struct
{
  int x, y;
} coord_t;

/**
 * Clears screen
 */
void clrscr();

/**
 * Lets us move in the terminal window to an x, y position
 * 
 * @param x Position in X axis
 * @param y Position in Y axis
 */
void gotoxy(int x, int y);

/**
 * Restore terminal color
 */
void restore_color();

/**
 * Restores al terminal attributes
 */
void restore_terminal();

/**
 * Gets screen height (or terminal window height
 *
 * @return height
 */
int screenheight();

/**
 * Gets screen width (or terminal window width
 *
 * @return width
 */
int screenwidth();

/**
 * Disables keyboard echo
 *
 * @param canon Makes canon mode ON. This avoids us to press enter to press keys
 */
void termnoecho(int canon);

/**
 * Sets background colod
 * 
 * @param color Background color
 */
void textbackground(int color);

/**
 * Sets foreground color
 *
 * @param color foreground color
 */
void textcolor(int color);

/**
 * Gets current position or screen dimensions
 *
 * @param abs if 1 it returns screen dimensions
 *
 * @return Current position or screen dimensions
 */
coord_t wherexy(int abs);

/**
 * Gets current screen X position
 * 
 * @return X position
 */
int wherex();

/**
 * Gets current screen Y position
 * 
 * @return Y position
 */
int wherey();

/**
 * Asks for a character. No need to press enter to get character.
 * No echo on screen.
 *
 * @return Character code
 */
int getch();

/**
 * Asks for a character. No need to press enter to get character.
 * Character echoed on screen
 *
 * @return Character code
 */
int getche();			/* Simula getche()- Pide un carácter (sin INTRO) y con echo */

/**
 * Tells us if a key has been pressed without stopping execution
 *
 * @return true if a key has been pressed
 */
int kbhit();			/* Devuelve si se ha pulsado una tecla o no (no detiene ejecución) */

/**
 * Tells us if a key has been pressed without stopping execution
 *
 * @return keycode of the pressed key. 0 if none pressed
 */
int kbhit2();			/* Si se ha pulsado una tecla, devuelve su código (no detiene ejecución) */ 

/**
 * Tells us if a key has been pressed without stopping execution.
 * It behaves like kbhit2() but doesn't restore terminal properties.
 * Useful when asking so many keys avoiding modifying and restoring
 * terminal all the time, just call kbhit_pre() before making calls
 * to __kbhit().
 *
 * @return keycode of the pressed key. 0 if none pressed
 */
int __kbhit();

/**
 * Prepares the terminal for kbhit 
 */
void kbhit_pre();

/**
 * Saves terminal information in temporary variables
 */
void save_term_info();

/**
 * Restores terminal information
 */
void term_defaults();

/**
 * Restores terminal, with custom values.
 */
void restore_term(struct termios *tty, int *stdinflags);

/**
 * Writes error in stderr and closes program
 */
void error(char *err);