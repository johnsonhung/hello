/*
   Creating the bouncing ball.
   Modified by Johnw (2015.12.10) based on Dietel's examples in their textbook CHTP

   busy-waiting while loop
   with boing.wav sound and background music
   with right/left paddles
   with text
*/
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>

#include <windows.h>

/* symbolic constants for the ball’s possible directions */
#define DOWN_RIGHT 0
#define UP_RIGHT 1
#define DOWN_LEFT 2
#define UP_LEFT 3
#define PADDLE_MOVE 3
#define SCREEN_W  640
#define SCREEN_H  480
#define BALL_SIZE 40
#define PADDLE_SIZE 100
#define PLAYER_R 0
#define PLAYER_L 1

/* function prototypes */
void moveBall( void );
void movePaddle( void );
void getScore( int );
void reverseVerticalDirection( void );
void reverseHorizontalDirection( void );

int ball_x; /* the ball’s x-coordinate */
int ball_y; /* the ball’s y-coordinate */
int direction; /* the ball’s direction */

int barL_y; /* y-coordinate of the left paddle */
int barR_y; /* y-coordinate of the right paddle */

int scoreL=0; /* score of the left player */
int scoreR=0; /* score of the right player */

//

ALLEGRO_BITMAP *bar = NULL; /* pointer to the bar's image bitmap */
ALLEGRO_BITMAP *ball = NULL; /* pointer to the ball's image bitmap */
ALLEGRO_DISPLAY *display = NULL;/* pointer to display */
ALLEGRO_SAMPLE *boing = NULL; /* pointer to sound file */
ALLEGRO_SAMPLE *background = NULL; /* pointer to sound file */
ALLEGRO_FONT *pongFont = NULL; /* pointer to Font file */

ALLEGRO_KEYBOARD_STATE KBstate;


int main( void )
{
   /* first, set up Allegro and the graphics mode */
   al_init(); /* initialize Allegro */
   al_install_keyboard(); /* install the keyboard for Allegro to use */
   al_init_image_addon();
   al_install_audio();  // install sound driver
   al_init_acodec_addon();
   al_reserve_samples(9);
   al_init_font_addon();    // install font addons
   al_init_ttf_addon();


   display = al_create_display(SCREEN_W, SCREEN_H);

   ball = al_load_bitmap( "ball.bmp"); /* load the ball bitmap */
   //
   ball_x = SCREEN_W / 2; /* give the ball its initial x-coordinate */
   ball_y = SCREEN_H / 2; /* give the ball its initial y-coordinate */
   srand( time( NULL ) ); /* seed the random function */
   direction = rand() % 4; /* and then make a random initial direction */

   bar = al_load_bitmap( "bar.bmp"); /* load the bar bitmap */
   //
   barL_y = SCREEN_H / 2; /* give left paddle its initial y-coordinate */
   barR_y = SCREEN_H / 2; /* give right paddle its initial y-coordinate */

   pongFont = al_load_ttf_font("ARCHRISTY.ttf", 16, 0); /* load the FONT file */

   boing = al_load_sample("boing.wav"); /* load the sound file */

   background = al_load_sample("taipei.wav"); /* load the background sound file */
   al_play_sample(background, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

   while(1)
    {
        al_get_keyboard_state(&KBstate);
        if (al_key_down(&KBstate, ALLEGRO_KEY_ESCAPE)) break;

		/* draw text onto the buffer */
		al_draw_textf( pongFont, al_map_rgb(255, 255, 255), 75, 0, -1, "Left Player Score: %d", scoreL);
        al_draw_textf( pongFont, al_map_rgb(255, 255, 255), 400, 0, -1, "Right Player Score: %d", scoreR);

        movePaddle(); /* move the paddles */
        al_draw_bitmap( bar, 0, barL_y, 0);
        al_draw_bitmap( bar, 620, barR_y, 0);

        moveBall(); /* move the ball */
        al_draw_bitmap( ball, ball_x, ball_y, 0);/* draw the bitmap */

        al_rest(0.01);

        /* display */
        al_flip_display(); /* Wait for the beginning of a vertical retrace. */
        al_clear_to_color(al_map_rgb(0,0,0));

        /* Clear the complete target bitmap, but confined by the clipping rectangle. */
    }

    al_destroy_bitmap( ball); /* destroy the ball bitmap */
    al_destroy_display( display); /* destroy the display */
    al_destroy_bitmap( bar ); /* destroy the bar bitmap */
    al_destroy_sample( boing ); /* destroy the boing sound file */
    al_destroy_font( pongFont ); /* destroy the font */

    return 0;
} /* end function main */

void moveBall() /* moves the ball */
{
   switch ( direction ) {
      case DOWN_RIGHT:
         ++ball_x; /* move the ball to the right */
         ++ball_y; /* move the ball down */
         break;
      case UP_RIGHT:
         ++ball_x; /* move the ball to the right */
         --ball_y; /* move the ball up */
         break;
      case DOWN_LEFT:
         --ball_x; /* move the ball to the left */
         ++ball_y; /* move the ball down */
         break;
      case UP_LEFT:
         --ball_x; /* move the ball to the left */
         --ball_y; /* move the ball up */
         break;
   } /* end switch */

   /* make sure the ball doesn’t go off the screen */

   /* if the ball is going off the top or bottom... */
   if ( ball_y <= 0 || ball_y >= (SCREEN_H - BALL_SIZE) )
      reverseVerticalDirection(); /* make it go the other way */

   /* if the ball is going off the left or right... */
   if ( ball_x <= 0 || ball_x >= (SCREEN_W - BALL_SIZE) )
      reverseHorizontalDirection(); /* make it go the other way */
} /* end function moveBall */

void reverseVerticalDirection() /* reverse the ball’s up-down direction */
{
   if ( direction == DOWN_RIGHT ) direction = UP_RIGHT;
    else if ( direction == DOWN_LEFT ) direction = UP_LEFT;
        else if ( direction == UP_RIGHT ) direction = DOWN_RIGHT;
            else if ( direction == UP_LEFT ) direction = DOWN_LEFT;

   al_play_sample(boing, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL); /* play "boing" sound once */

} /* end function reverseVerticalDirection */

void reverseHorizontalDirection() /* reverses the horizontal direction */
{
//   int player;
   switch ( direction ){
      case DOWN_RIGHT:
         direction = DOWN_LEFT;
         getScore(PLAYER_R);
         break;
      case UP_RIGHT:
         direction = UP_LEFT;
         getScore(PLAYER_R);
         break;
      case DOWN_LEFT:
         direction = DOWN_RIGHT;
         getScore(PLAYER_L);
         break;
      case UP_LEFT:
         direction = UP_RIGHT;
         getScore(PLAYER_L);
         break;
   } /* end switch */
   al_play_sample(boing, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL); /* play "boing" sound once */

} /* end function reverseHorizontalDirection */

void movePaddle(){

    if (al_key_down(&KBstate, ALLEGRO_KEY_A)) barL_y -= PADDLE_MOVE;
    if (al_key_down(&KBstate, ALLEGRO_KEY_Z)) barL_y += PADDLE_MOVE;
    if (al_key_down(&KBstate, ALLEGRO_KEY_UP)) barR_y -= PADDLE_MOVE;
    if (al_key_down(&KBstate, ALLEGRO_KEY_DOWN)) barR_y += PADDLE_MOVE;

    /* make sure the paddles don go off screen */
    if (barL_y < 30)        barL_y = 30;
    if (barL_y > 380)       barL_y = 380;
    if (barR_y < 30)        barR_y = 30;
    if (barR_y > 380)       barR_y = 380;

}

void getScore(int player){

    if (player == PLAYER_R &&
            (
             (barR_y >= ball_y && barR_y <= ball_y+BALL_SIZE) ||
             (barR_y+PADDLE_SIZE >= ball_y && barR_y+PADDLE_SIZE <= ball_y+BALL_SIZE) ||
             (barR_y <= ball_y && barR_y+PADDLE_SIZE >= ball_y+BALL_SIZE)
             )
            ){
                scoreR = scoreR + 1;
                printf("ball y=%d %d\n", ball_y, ball_y+BALL_SIZE);
                printf("paddle R =%d %d\n", barR_y, barR_y+PADDLE_SIZE);
                printf("paddle L =%d %d\n", barL_y, barL_y+PADDLE_SIZE);
                printf("\n");
                }
    else
        if (player == PLAYER_L &&
            (
             (barL_y >= ball_y && barL_y <= ball_y+BALL_SIZE) ||
             (barL_y+PADDLE_SIZE >= ball_y && barL_y+PADDLE_SIZE <= ball_y+BALL_SIZE) ||
             (barL_y <= ball_y && barL_y+PADDLE_SIZE >= ball_y+BALL_SIZE)
             )
            ){
                scoreL = scoreL + 1;
                printf("ball y=%d %d\n", ball_y, ball_y+BALL_SIZE);
                printf("paddle R =%d %d\n", barR_y, barR_y+PADDLE_SIZE);
                printf("paddle L =%d %d\n", barL_y, barL_y+PADDLE_SIZE);
                printf("\n");
                }

}
