/**
So far, the player can jump (with gravity) to collect bones (1 point)
and meat cubes (3 points) for their score. Arty has 5 lives, so once you
touch water 5 times, you lose the game. There is also a cheat where
if you press the A button, Arty will be invincible to the water.

Current Gameplay:
- To start the game, toggle the selected button with the up and down keys,
and once you have selected the button you want (it will be highlighted blue),
press start.
- To play the game, use the right button to move right, and the up
button to jump up to collect things. You can double, triple, quadruple, etc. jump.
- While in the game, press start to pause. Toggle and select the button you want
the same way you did for the start screen on the pause screen.
- While on the instructions screen, same thing, toggle button to select start button.

What still needs to be done:
- show score on screen during game?
- different background music for different states? (pause state music)
- when you lose a life, go back to start of level
*/


#include <stdlib.h>
#include <stdio.h>
#include "myLib.h"
#include "game.h"
#include "font.h"
#include "text.h"

//include state background images
#include "start.h"
#include "instructions.h"
#include "pause.h"
#include "win.h"
#include "lose.h"

//include level background images
#include "level1Back.h"
#include "level1Fore.h"
#include "level2Back.h"
#include "level3Back.h"

//include sounds
#include "sound.h"
#include "totoroBGM.h"
#include "select.h"


//set up state machine
int state;
enum {START, INSTRUCTIONS, LEVEL1, LEVEL2, LEVEL3, PAUSE, WIN, LOSE};

//prototypes
void initialize();

//state prototypes
void start();
void instructions();
void level1();
void level2();
void level3();
void pause();
void win();
void lose();

//go to state prototypes
void goToStart();
void goToInstructions();
void goToLevel1();
void goToLevel2();
void goToLevel3();
void goToPause();
void goToWin();
void goToLose();

//set up button variables
unsigned int buttons;
unsigned int oldButtons;

//random seed
int seed;

//text buffer
char buffer[41];


int main() {

    initialize();

    while(1) {
        oldButtons = buttons;
        buttons = BUTTONS;

        switch(state) {
            case START:
                start();
                break;
            case INSTRUCTIONS:
                instructions();
                break;
            case LEVEL1:
                level1();
                break;
            case LEVEL2:
                level2();
                break;
            case LEVEL3:
                level3();
                break;
            case PAUSE:
                pause();
                break;
            case WIN:
                win();
                break;
            case LOSE:
                lose();
                break;
        }
    }

    return 0;
}

void initialize() {

    

    REG_DISPCTL = MODE0 | BG0_ENABLE | SPRITE_ENABLE;

    buttons = BUTTONS;

    //initialize score values
    score = 0;
    highScore = 0;

    //initialize/setup sounds and interrupts
    setupSounds();
    setupInterrupts();

    //hide all the sprites
    hideSprites();

    goToStart();


}

void goToStart() {

    //set up display control register
    REG_DISPCTL = MODE0 | BG0_ENABLE | SPRITE_ENABLE;

    //load the start tile palette
    DMANow(3, startPal, PALETTE, startPalLen / 2);

    //set up 0 control register
    REG_BG0CNT = BG_SIZE_SMALL | BG_CHARBLOCK(0) | BG_SCREENBLOCK(30);

    //load start tiles into charblock
    DMANow(3, startTiles, &CHARBLOCK[0], startTilesLen / 2);

    //load start map to screenblock
    DMANow(3, startMap, &SCREENBLOCK[30], startMapLen / 2);

    //creating buttons
    initializeButtons();

    //screen is start screen
    screen = 0;

    //begin seed randomization
    seed = 0;

    hOff = 0;

    //initialize level
    level = 0;

    //play background music
    playSoundA(totoroBGM, TOTOROBGMLEN, TOTOROBGMFREQ, 1);

    //initialize lives
    livesRemaining = LIVESCOUNT;

    initializeLives();

    state = START;
}

void start() {
    updateButtons();
    drawButtons();

    seed++;

    if (BUTTON_PRESSED(BUTTON_START) && instructButton.isSelected) {
        hideSprites();
        playSoundB(select, SELECTLEN, SELECTFREQ, 0);
        goToInstructions();
    }

    if (BUTTON_PRESSED(BUTTON_START) && startButton.isSelected) {
        score = 0;
        hideSprites();
        level = 1;
        srand(seed);
        playSoundB(select, SELECTLEN, SELECTFREQ, 0);
        initializeGame();
        goToLevel1();
    }

    waitForVBlank();

}

void goToInstructions() {

    //set up display control register
    REG_DISPCTL = MODE0 | BG0_ENABLE | SPRITE_ENABLE;

    //load the instructions tile palette
    DMANow(3, instructionsPal, PALETTE, 256);

    //set up 0 control register
    REG_BG0CNT = BG_SIZE_SMALL | BG_CHARBLOCK(0) | BG_SCREENBLOCK(30);

    //load instructions tiles into charblock
    DMANow(3, instructionsTiles, &CHARBLOCK[0], instructionsTilesLen / 2);

    //load instructions map to screenblock
    DMANow(3, instructionsMap, &SCREENBLOCK[30], 1024);

    screen = 1;

    //initialize buttons
    initializeButtons();
    startButton.isSelected = 0;
    startButton.col -= 20;
    startButton.row += 20;
    arrow.row += 40;
    arrow.col -= 20;

    state = INSTRUCTIONS;

    hOff = 0;

}

void instructions() {

    updateButtons();
    drawButtons();

    if (BUTTON_PRESSED(BUTTON_START) && startButton.isSelected) {
        if (level == 0) {
            hideSprites();
            srand(seed);
            level = 1;
            initializeGame();
            goToLevel1();
        } else if (level == 1) {
            hideSprites();
            goToLevel1();
        } else if (level == 2) {
            hideSprites();
            goToLevel2();
        } else {
            hideSprites();
            goToLevel3();
        }
        
    }

    waitForVBlank();

}

void goToLevel1() {

    //set up display control register
    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | SPRITE_ENABLE;

    //load the level 1 tile palette
    DMANow(3, level1BackPal, PALETTE, 256);

    //set up 0 control register
    REG_BG1CNT = BG_SIZE_WIDE | BG_CHARBLOCK(0) | BG_SCREENBLOCK(30);

    //set up 1 control register
    REG_BG0CNT = BG_SIZE_SMALL | BG_CHARBLOCK(3) | BG_SCREENBLOCK(29);

    //load level 1 tiles into charblock
    DMANow(3, level1BackTiles, &CHARBLOCK[0], level1BackTilesLen / 2);
    DMANow(3, level1ForeTiles, &CHARBLOCK[3], level1ForeTilesLen / 2);

    //load level 1 map to screenblock
    DMANow(3, level1BackMap, &SCREENBLOCK[30], level1BackMapLen / 2);
    DMANow(3, level1ForeMap, &SCREENBLOCK[29], 1024);

    state = LEVEL1;

    hOff = 0;

}

void level1() {

    updateGame();
    drawGame();

    if (hOff == 1024) {
        srand(seed);
        level = 2;
        waitForVBlank();
        initializeGame();
        goToLevel2();
    }

    if (BUTTON_PRESSED(BUTTON_START)) {
        hideSprites();
        goToPause();
    }

    // scroll the backgrounds
    if (BUTTON_HELD(BUTTON_RIGHT)) {
        //update player's curframe
        if (player.aniCounter % 5 == 0 && !jump) {
            player.curFrame = (player.curFrame + 4) % (player.numFrames * 4);
        }
        player.aniCounter++;
        hOff++;
    }

    waitForVBlank();

    //implementing parallax
    REG_BG0HOFF = hOff / 2;
    REG_BG1HOFF = hOff / 4;

}

void goToLevel2() {
    
    //set up display control register
    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | SPRITE_ENABLE;
    
    //load the level 2 tile palette
    DMANow(3, level2BackPal, PALETTE, 256);

    //set up 0 control register
    REG_BG1CNT = BG_SIZE_WIDE | BG_CHARBLOCK(0) | BG_SCREENBLOCK(30);

    //set up 1 control register
    REG_BG0CNT = BG_SIZE_SMALL | BG_CHARBLOCK(3) | BG_SCREENBLOCK(29);

    //load level 2 tiles into charblock
    DMANow(3, level2BackTiles, &CHARBLOCK[0], 21424);
    DMANow(3, level1ForeTiles, &CHARBLOCK[3], level1ForeTilesLen / 2);

    //load level 2 map to screenblock
    DMANow(3, level2BackMap, &SCREENBLOCK[30], 2048);
    DMANow(3, level1ForeMap, &SCREENBLOCK[29], 1024);

    playSoundB(select, SELECTLEN, SELECTFREQ, 0);

    state = LEVEL2;
    hOff = 0;

}

void level2() {

    updateGame();
    drawGame();

    if (hOff == 1024) {
        srand(seed);
        level = 3;
        waitForVBlank();
        initializeGame();
        goToLevel3();
    }
    if (BUTTON_PRESSED(BUTTON_START)) {
        hideSprites();
        goToPause();
    }

    // scroll the backgrounds
    if (BUTTON_HELD(BUTTON_RIGHT)) {
        //update player's curframe
        if (player.aniCounter % 5 == 0 && !jump) {
            player.curFrame = (player.curFrame + 4) % (player.numFrames * 4);
        }
        player.aniCounter++;
        hOff++;
    }

    //lose if you run out of lives
    if (livesRemaining == 0) {
        hideSprites();
        goToLose();
    }

    waitForVBlank();

    //implementing parallax
    REG_BG0HOFF = hOff / 2;
    REG_BG1HOFF = hOff / 4;

}

void goToLevel3() {

    //set up display control register
    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | SPRITE_ENABLE;

    //load the level 3 tile palette
    DMANow(3, level3BackPal, PALETTE, 256);

    //set up 0 control register
    REG_BG1CNT = BG_SIZE_WIDE | BG_CHARBLOCK(0) | BG_SCREENBLOCK(30);

    //set up 1 control register
    REG_BG0CNT = BG_SIZE_SMALL | BG_CHARBLOCK(3) | BG_SCREENBLOCK(29);

    //load level 3 tiles into charblock
    DMANow(3, level3BackTiles, &CHARBLOCK[0], level3BackTilesLen / 2);
    DMANow(3, level1ForeTiles, &CHARBLOCK[3], level1ForeTilesLen / 2);

    //load level 3 map to screenblock
    DMANow(3, level3BackMap, &SCREENBLOCK[30], level3BackMapLen / 2);
    DMANow(3, level1ForeMap, &SCREENBLOCK[29], 1024);

    playSoundB(select, SELECTLEN, SELECTFREQ, 0);

    state = LEVEL3;

    hOff = 0;

}

void level3() {

    updateGame();
    drawGame();

    if (hOff == 1024) {
        hideSprites();
        goToWin();
    }
    if (BUTTON_PRESSED(BUTTON_START)) {
        hideSprites();
        goToPause();
    }
    if (livesRemaining == 0) {
        hideSprites();
        goToLose();
    }

    // scroll the backgrounds
    if (BUTTON_HELD(BUTTON_RIGHT)) {
        //update player's curframe
        if (player.aniCounter % 5 == 0 && !jump) {
            player.curFrame = (player.curFrame + 4) % (player.numFrames * 4);
        }
        player.aniCounter++;
        hOff++;
    }

    waitForVBlank();

    //implementing parallax
    REG_BG0HOFF = hOff / 2;
    REG_BG1HOFF = hOff / 4;

}

void goToPause() {

    //set up display control register
    REG_DISPCTL = MODE0 | BG0_ENABLE | SPRITE_ENABLE;

    //load the pause tile palette
    DMANow(3, pausePal, PALETTE, 256);

    //set up 0 control register
    REG_BG0CNT = BG_SIZE_SMALL | BG_CHARBLOCK(0) | BG_SCREENBLOCK(31);

    //load pause tiles into charblock
    DMANow(3, pauseTiles, &CHARBLOCK[0], pauseTilesLen / 2);

    //load pause map to screenblock
    DMANow(3, pauseMap, &SCREENBLOCK[31], 1024);

    hOff = 0;
    vOff = 0;

    //screen is pause screen
    screen = 2;

    //initialize buttons
    initializeButtons();

    //pause sound
    pauseSound();

    state = PAUSE;

}

void pause() {
    updateButtons();
    drawButtons();

    if (BUTTON_PRESSED(BUTTON_START) && startButton.isSelected) {
        unpauseSound();
        playSoundB(select, SELECTLEN, SELECTFREQ, 0);
        if (level == 1) {
            hideSprites();
            goToLevel1();
        } else if (level == 2) {
            hideSprites();
            goToLevel2();
        } else {
            hideSprites();
            goToLevel3();
        }
    }
    
    if (BUTTON_PRESSED(BUTTON_START) && instructButton.isSelected) {
        unpauseSound();
        playSoundB(select, SELECTLEN, SELECTFREQ, 0);
        hideSprites();
        goToInstructions();
    }

    if (BUTTON_PRESSED(BUTTON_START) && quitButton.isSelected) {
        playSoundB(select, SELECTLEN, SELECTFREQ, 0);
        hideSprites();
        goToStart();
    }

    waitForVBlank();

}

void goToWin() {

    //set up display control register
    REG_DISPCTL = MODE3 | BG2_ENABLE | SPRITE_ENABLE;

    // //load the win tile palette
    // DMANow(3, winPal, PALETTE, 256);

    // //set up 0 control register
    // REG_BG0CNT = BG_SIZE_SMALL | BG_CHARBLOCK(0) | BG_SCREENBLOCK(31);

    // //load win tiles into charblock
    // DMANow(3, winTiles, &CHARBLOCK[0], winTilesLen / 2);

    // //load win map to screenblock
    // DMANow(3, winMap, &SCREENBLOCK[31], 1024);

    state = WIN;
    hOff = 0;

    //fillScreen3(GREEN);

    drawFullscreenImage3(winBitmap);

    drawString(80, 30, "You won!", BLACK);

    //update highscore
    if (score >= highScore) {
        highScore = score;

        drawString(60, 50, "Congrats! New High Score!", BLACK);
    }

    sprintf(buffer, "Score: %d", score);
    drawString(100, 70, buffer, BLACK);

    sprintf(buffer, "Highscore: %d", highScore);
    drawString(120, 90, buffer, BLACK);

}

void win() {

    if (BUTTON_PRESSED(BUTTON_START)) {
        hideSprites();
        goToStart();
    }

}

void goToLose() {

    // //set up display control register
    // REG_DISPCTL = MODE0 | BG0_ENABLE | SPRITE_ENABLE;

    // //load the lose tile palette
    // DMANow(3, losePal, PALETTE, 256);

    // //set up 0 control register
    // REG_BG0CNT = BG_SIZE_SMALL | BG_CHARBLOCK(0) | BG_SCREENBLOCK(31);

    // //load lose tiles into charblock
    // DMANow(3, loseTiles, &CHARBLOCK[0], 832);

    // //load lose map to screenblock
    // DMANow(3, loseMap, &SCREENBLOCK[31], 1024);

    //set up display control register
    REG_DISPCTL = MODE3 | BG2_ENABLE | SPRITE_ENABLE;

    drawFullscreenImage3(loseBitmap);

    drawString(65, 5, "Oh no! Arty got drenched!", BLACK);
    drawString(80, 15, "You lost!", BLACK);
    drawString(60, 25, "Press Start to try again.", BLACK);

    state = LOSE;
    hOff = 0;

}

void lose() {

    if (BUTTON_PRESSED(BUTTON_START)) {
        hideSprites();
        goToStart();
    }

}