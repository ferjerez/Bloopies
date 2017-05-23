/*
    ARDUJEWEL?????? (not the final name)

    Fernando Jerez 2017

    Licensed: CC-NC

    Feel free to distribute, modify or do what you want with the code.
    NON COMMERCIAL LICENSE.
    Keep this little piece of code free for the joy of human beings.

    Have fun!!!
*/

#include <Arduboy2.h>
#include <ArduboyTones.h>
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

#include "globals.h"
#include "bitmaps.h"
#include "functions.h"

void setup() {

  arduboy.begin();
  arduboy.setFrameRate(30);
  arduboy.initRandomSeed();
  arduboy.setTextWrap(false);

  // cursor initial position
  xpos = MAXX / 2;
  ypos = MAXY / 2;

  // Initialize board,flag,offy
  initBoard();
  // Check initial matches
  checkBoard();

}

void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame())) return;

  arduboy.pollButtons();
  arduboy.clear();
  if (title) {
    
    // TITLE SCREEN
    
    arduboy.drawBitmap(0, 0,  titleIMG, 128, 64, 1);
    if (arduboy.justPressed(B_BUTTON) || arduboy.justPressed(A_BUTTON)) title = false;

    // credits marquee
    arduboy.setCursor(marqueePos,56);
    arduboy.print(marqueeTxt);
    marqueePos--;
    if(marqueePos < marqueeLength ){
      marqueePos = 128;
    }
    

    
  } else {

    // GAME
    
    if (pause) {
      arduboy.setCursor(50, 28);
      arduboy.print("PAUSE");
      if (arduboy.justPressed(B_BUTTON) && !arduboy.pressed(A_BUTTON)) pause = false;
    } else {

      // Time
      arduboy.fillRect(0, 62, max(0, timeLeft), 2);


      // Text

      arduboy.setCursor(86, 4);
      arduboy.print("LVL:");
      arduboy.print(game_level);
      arduboy.setCursor(86, 12);
      arduboy.print("Score");
      arduboy.setCursor(86, 20);
      arduboy.print(score);

      if (tmp_score > 0) {
        arduboy.setCursor(86, 28);
        arduboy.print("x");
        arduboy.print(mult_score);
        arduboy.setCursor(86, 36);
        arduboy.print("+");
        arduboy.print(tmp_score);

        arduboy.setCursor(86, 52);
        arduboy.print(txt[constrain(mult_score - game_level, 0, 7)]);
      }else{
        if(score>topscore) topscore = score;
        arduboy.setCursor(86,44);
        arduboy.print("Top");
        arduboy.setCursor(86, 52);
        arduboy.print(topscore);
      }


      // Move cursor
      prevx = xpos;
      prevy = ypos;


      if (arduboy.justPressed(UP_BUTTON))    ypos = max(0, ypos - 1);
      if (arduboy.justPressed(DOWN_BUTTON))  ypos = min(MAXY - 1, ypos + 1);
      if (arduboy.justPressed(LEFT_BUTTON))  xpos = max(0, xpos - 1);
      if (arduboy.justPressed(RIGHT_BUTTON)) xpos = min(MAXX - 1, xpos + 1);
      if (arduboy.justPressed(B_BUTTON) && !arduboy.pressed(A_BUTTON)) pause = true;

      // FIRST: If there's flagged tiles keep blinking for a while before cleaning and continue playing
      if (flagCount > 0) {
        timeLeft = maxTime;
        if (arduboy.frameCount - flagFrame > 15) {
          // refill
          refillBoard();
          if (!falling) {
            mult_score++;
            checkBoard();

            if (flagCount == 0) {
              score += tmp_score;
              mult_score = game_level;
              tmp_score = 0;
            }
          }
        }
      } else {
        if (!gameover) {

          // Moving...

          // Time decay
          if (arduboy.frameCount % decay == 0) {
            timeLeft--;
          }
          // detect GAME OVER
          if (timeLeft <= 0) {
            gameover = true;
          }



          if (arduboy.pressed(A_BUTTON) && (xpos != prevx || ypos != prevy)) {
            // SWAP
            byte tmp = board[xpos][ypos];
            board[xpos][ypos] = board[prevx][prevy];
            board[prevx][prevy] = tmp;

            // check matchs
            if (!falling) {
              checkBoard();
            }

            // undo the swapping if anything flagged
            if (flagCount == 0) {
              byte tmp = board[xpos][ypos];
              board[xpos][ypos] = board[prevx][prevy];
              board[prevx][prevy] = tmp;
              xpos = prevx;
              ypos = prevy;

            } else {
              // add a new move
              moves++;
              // increases level every 5 moves
              if (moves % 5 == 0) game_level++;
              // fill time
              timeLeft = maxTime;
            }
          }


        } else {
          // GAME OVER state. Push A or B for a new game
          if (arduboy.justPressed(B_BUTTON) || arduboy.justPressed(A_BUTTON)) {
            title = true;
            gameover = false;
            pause = false;
            score = 0;
            moves = 0;
            game_level = 1;
            mult_score = 1;
            timeLeft = 128;
            // init board
            initBoard();
            // Check initial matches
            checkBoard();
          }

        }
      }
      // Drawing...

      // draw BOARD
      falling = false;
      for (byte x = 0; x < MAXX; x++) {
        for (byte y = 0; y < MAXY; y++) {
          if ((int)pics[board[x][y]] != EMPTY) {
            if (!falling && offy[x][y] > 0) falling = true;
            if (!flag[x][y] || arduboy.frameCount % 2) // blink when flagged
              if (!flag[x][y]) {

                arduboy.drawBitmap(BOARD_X + x * 8, BOARD_Y + (y * 8) - offy[x][y],  pics[board[x][y]], 8, 8, 1);
                offy[x][y] = max(offy[x][y] - 1, 0);
              } else {
                arduboy.drawBitmap(BOARD_X + x * 8, BOARD_Y + (y * 8),  pics[board[x][y]], 8, 8, 1);
              }

          } else {
            // arduboy.fillRect(BOARD_X + x * 8, BOARD_Y + y * 8, 8, 8, BLACK);
          }
        }
      }

      if (!gameover) {
        // Cursor
        if (arduboy.frameCount % 10) {
          arduboy.drawRect(BOARD_X + xpos * 8 - 1, BOARD_Y + ypos * 8 - 1, 9, 9);
        }
      } else {
        // GAME OVER
        arduboy.fillRect(16, 16, 48, 32, BLACK);
        arduboy.drawRect(16, 16, 48, 32, WHITE);
        arduboy.setCursor(28, 22);
        arduboy.print("GAME");
        arduboy.setCursor(28, 34);
        arduboy.print("OVER");

      }

    }
  }

  arduboy.display();

}
