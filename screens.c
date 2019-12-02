#include "myLib.h"
#include "game.h"
#include "spriteSheet.h"

#include "sound.h"
#include "toggle.h"

//variables
SPRITE startButton;
SPRITE instructButton;
SPRITE quitButton;
SPRITE sitting;
SPRITE sleeping;

SPRITE arrow;

OBJ_ATTR shadowOAM[128];


void initializeButtons() {
    //load spritesheet palette and tiles
    DMANow(3, &spriteSheetPal, SPRITEPALETTE, 16);
    DMANow(3, &spriteSheetTiles, &CHARBLOCK[4], spriteSheetTilesLen / 2);


    //hide all of the sprites
    hideSprites();


    //initialize start button
    startButton.width = 32;
    startButton.height = 16;
    startButton.cdel = 0;
    startButton.rdel = 0;
    startButton.rAcc = 0;
    startButton.col = 48;
    startButton.row = 80;

    startButton.aniCounter = 0;
    startButton.curFrame = 24;
    startButton.numFrames = 2;
    startButton.aniState = 0;

    startButton.isSelected = 1;
    startButton.active = 1;

    //initialize instructions button
    if (screen == 0 || screen == 2) {
        instructButton.width = 32;
        instructButton.height = 16;
        instructButton.cdel = 0;
        instructButton.rdel = 0;
        instructButton.rAcc = 0;
        instructButton.col = 48;
        instructButton.row = 100;

        instructButton.aniCounter = 0;
        instructButton.curFrame = 24;
        instructButton.numFrames = 2;
        instructButton.aniState = 4;

        instructButton.isSelected = 0;
        instructButton.active = 1;
    }

    //initialize quit button if on pause screen
    if (screen == 2) {
        quitButton.width = 32;
        quitButton.height = 16;
        quitButton.cdel = 0;
        quitButton.rdel = 0;
        quitButton.rAcc = 0;
        quitButton.col = 48;
        quitButton.row = 120;

        quitButton.aniCounter = 0;
        quitButton.curFrame = 24;
        quitButton.numFrames = 2;
        quitButton.aniState = 8;

        quitButton.isSelected = 0;
        quitButton.active = 1;
    }

    //initialize arrow
    arrow.width = 16;
    arrow.height = 16;
    arrow.cdel = 0;
    arrow.rdel = 0;
    arrow.rAcc = 0;
    arrow.col = 32;
    arrow.row = 80;

    arrow.aniCounter = 0;
    arrow.curFrame = 28;
    arrow.numFrames = 1;
    arrow.aniState = 0;

    arrow.active = 1;

    //initialize sitting Arty
    if (screen == 0) {
        sitting.width = 32;
        sitting.height = 32;
        sitting.cdel = 0;
        sitting.rdel = 0;
        sitting.rAcc = 0;
        sitting.col = 180;
        sitting.row = 100;

        sitting.aniCounter = 0;
        sitting.curFrame = 0;
        sitting.aniState = 16;
        sitting.numFrames = 3;

        sitting.active = 1;
    }

    if (screen == 2) {
        sleeping.width = 32;
        sleeping.height = 32;
        sleeping.cdel = 0;
        sleeping.rdel = 0;
        sleeping.rAcc = 0;
        sleeping.col = 165;
        sleeping.row = 85;

        sleeping.aniCounter = 0;
        sleeping.curFrame = 0;
        sleeping.aniState = 20;
        sleeping.numFrames = 3;

        sleeping.active = 1;
    }

}

void updateButtons() {



    //update start button
    if (startButton.isSelected) {
        startButton.curFrame = 26;
    } else {
        startButton.curFrame = 24;
    }

    //update instructions button
    if (instructButton.isSelected) {
        instructButton.curFrame = 26;
    } else {
        instructButton.curFrame = 24;
    }

    //update quit button
    if (quitButton.isSelected) {
        quitButton.curFrame = 26;
    } else {
        quitButton.curFrame = 24;
    }

    //update sitting Arty
    if (screen == 0) {
        if (sitting.aniCounter % 20 == 0) {
            sitting.curFrame = (sitting.curFrame + 4) % (sitting.numFrames * 4);

        }
        sitting.aniCounter++;
    }

    //update sleeping Arty
    if (screen == 2) {
        if (sleeping.aniCounter % 20 == 0) {
            sleeping.curFrame = (sleeping.curFrame + 4) % (sleeping.numFrames * 4);

        }
        sleeping.aniCounter++;
    }

    if (BUTTON_PRESSED(BUTTON_DOWN)) {
        if (startButton.isSelected) {
            startButton.isSelected = 0;
            instructButton.isSelected = 1;
            arrow.row +=20;
        } else if (instructButton.isSelected) {
            if (screen == 2) {
                instructButton.isSelected = 0;
                quitButton.isSelected = 1;
                arrow.row += 20;
            }
        }
        playSoundB(toggle, TOGGLELEN, TOGGLEFREQ, 0);
    }

    if (BUTTON_PRESSED(BUTTON_UP)) {
        if (instructButton.isSelected) {
            instructButton.isSelected = 0;
            startButton.isSelected = 1;
            arrow.row -= 20;
        }
        if (quitButton.isSelected) {
            quitButton.isSelected = 0;
            instructButton.isSelected = 1;
            arrow.row -= 20;
        }
        playSoundB(toggle, TOGGLELEN, TOGGLEFREQ, 0);
    }


}

void drawButtons() {
    //start button
    if (startButton.active) {
        shadowOAM[0].attr0 = ATTR0_4BPP | ATTR0_WIDE | startButton.row;
        shadowOAM[0].attr1 = ATTR1_MEDIUM | startButton.col;
        shadowOAM[0].attr2 = ATTR2_TILEID(startButton.aniState, startButton.curFrame) | ATTR2_PALROW(0);
    } else {
        shadowOAM[0].attr0 = ATTR0_HIDE;
    }
    //instructions button
    if (instructButton.active && screen != 1) {
        shadowOAM[1].attr0 = ATTR0_4BPP | ATTR0_WIDE | instructButton.row;
        shadowOAM[1].attr1 = ATTR1_MEDIUM | instructButton.col;
        shadowOAM[1].attr2 = ATTR2_TILEID(instructButton.aniState, instructButton.curFrame) | ATTR2_PALROW(0);
    } else {
        shadowOAM[1].attr0 = ATTR0_HIDE;
    }

    
    if (instructButton.active && screen == 2) {
        shadowOAM[2].attr0 = ATTR0_4BPP | ATTR0_WIDE | quitButton.row;
        shadowOAM[2].attr1 = ATTR1_MEDIUM | quitButton.col;
        shadowOAM[2].attr2 = ATTR2_TILEID(quitButton.aniState, quitButton.curFrame) | ATTR2_PALROW(0);
    } else {
        shadowOAM[2].attr0 = ATTR0_HIDE;
    }

    if (arrow.active) {
        shadowOAM[3].attr0 = ATTR0_4BPP | ATTR0_SQUARE | arrow.row;
        shadowOAM[3].attr1 = ATTR1_SMALL | arrow.col;
        shadowOAM[3].attr2 = ATTR2_TILEID(arrow.aniState, arrow.curFrame) | ATTR2_PALROW(0);
    } else {
        shadowOAM[3].attr0 = ATTR0_HIDE;
    }

    if (sitting.active && screen == 0) {
        shadowOAM[4].attr0 = ATTR0_4BPP | ATTR0_SQUARE | sitting.row;
        shadowOAM[4].attr1 = ATTR1_MEDIUM | sitting.col;
        shadowOAM[4].attr2 = ATTR2_TILEID(sitting.aniState, sitting.curFrame) | ATTR2_PALROW(0);
    } else {
        shadowOAM[4].attr0 = ATTR0_HIDE;
    }

    if (sitting.active && screen == 2) {
        shadowOAM[5 + BONECOUNT + MEATCOUNT + SPLASHCOUNT].attr0 = ATTR0_4BPP | ATTR0_SQUARE | sleeping.row;
        shadowOAM[5 + BONECOUNT + MEATCOUNT + SPLASHCOUNT].attr1 = ATTR1_MEDIUM | sleeping.col;
        shadowOAM[5 + BONECOUNT + MEATCOUNT + SPLASHCOUNT].attr2 = ATTR2_TILEID(sleeping.aniState, sleeping.curFrame) | ATTR2_PALROW(0);
    } else {
        shadowOAM[5 + BONECOUNT + MEATCOUNT + SPLASHCOUNT].attr0 = ATTR0_HIDE;
    }



    waitForVBlank();

    //copy the shadowOAM into the OAM
    DMANow(3, shadowOAM, OAM, 128 *4);
}