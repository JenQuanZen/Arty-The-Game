#include <stdlib.h>
#include "myLib.h"
#include "game.h"
#include "spriteSheet.h"

#include "sound.h"
#include "splash.h"

//macros for gravity
#define SHIFTUP(i) ((i) << 8)
#define SHIFTDOWN(i) ((i) >> 8)

//variables
SPRITE player;
SPRITE bones[BONECOUNT];
SPRITE meat[MEATCOUNT];
SPRITE rivers[RIVERCOUNT];
SPRITE splashes[SPLASHCOUNT];
SPRITE lives[LIVESCOUNT];
int livesRemaining;

//cheat boolean
int cheat;

//jump boolean
int jump;

void initializeGame() {
    
    //load spritesheet palette and tiles
    DMANow(3, &spriteSheetPal, SPRITEPALETTE, 16);
    DMANow(3, &spriteSheetTiles, &CHARBLOCK[4], spriteSheetTilesLen / 2);

    cheat = 0;
    jump = 0;

    //hide all of the sprites
    hideSprites();

    initializePlayer();
    initializeBones();
    initializeMeat();

    if (level == 2) {
        initializeRivers();
    }

    if (level == 3) {
        initializeSplashes();
    }
}

void updateGame() {
    updatePlayer();
    for (int i = 0; i < BONECOUNT; i++) {
        updateBones(&bones[i]);

        if (bones[i].aniCounter % 10 == 0) {
            //bones[i].curFrame = 9 + (bones[i].curFrame + 2) % (bones[i].numFrames * 2);
            if (bones[i].curFrame == 12) {
                bones[i].curFrame = 14;
            } else if (bones[i].curFrame == 14) {
            bones[i].curFrame = 16;
            } else {
                bones[i].curFrame = 12;
            }

        }
        bones[i].aniCounter++;
    }

    for (int i = 0; i < MEATCOUNT; i++) {
        updateMeat(&meat[i]);

        if (meat[i].aniCounter % 15 == 0) {
            //bones[i].curFrame = 9 + (bones[i].curFrame + 2) % (bones[i].numFrames * 2);
            if (meat[i].curFrame == 12) {
                meat[i].curFrame = 14;
            } else if (meat[i].curFrame == 14) {
                meat[i].curFrame = 16;
            } else {
                meat[i].curFrame = 12;
            }

        }
        meat[i].aniCounter++;
    }

    if (level == 2) {
        for (int i = 0; i < RIVERCOUNT; i++) {
            updateRivers(&rivers[i]);

            if (rivers[i].aniCounter % 10 == 0) {
            //bones[i].curFrame = 9 + (bones[i].curFrame + 2) % (bones[i].numFrames * 2);
            if (rivers[i].curFrame == 12) {
                rivers[i].curFrame = 16;
            } else if (rivers[i].curFrame == 16) {
            rivers[i].curFrame = 20;
            } else {
            rivers[i].curFrame = 12;
            }

        }
        rivers[i].aniCounter++;
        }

    }
    if (level == 3) {
        for (int i = 0; i < SPLASHCOUNT; i++) {
            updateSplashes(&splashes[i]);
        }
    }

}

void drawGame() {
    drawPlayer();
    drawBones();
    drawMeat();

    if (level == 2) {
        drawRivers();
    }

    if (level == 3) {
        drawSplashes();
    }

    //draw lives
    for (int i = 0; i < LIVESCOUNT; i++) {
        if (lives[i].active) {
            shadowOAM[5 + BONECOUNT + MEATCOUNT + RIVERCOUNT + i].attr0 = ATTR0_4BPP | ATTR0_SQUARE | lives[i].row;
            shadowOAM[5 + BONECOUNT + MEATCOUNT + RIVERCOUNT + i].attr1 = ATTR1_SMALL | lives[i].col;
            shadowOAM[5 + BONECOUNT + MEATCOUNT + RIVERCOUNT + i].attr2 = ATTR2_TILEID(lives[i].aniState, lives[i].curFrame) | ATTR2_PALROW(0);
        } else {
            shadowOAM[5 + BONECOUNT + MEATCOUNT + RIVERCOUNT + i].attr0 = ATTR0_HIDE;
        }
    }

    waitForVBlank();

    //copy the shadowOAM into the OAM
    DMANow(3, shadowOAM, OAM, 128 *4);

}

void initializePlayer() {
    player.width = 32;
    player.height = 32;
    player.cdel = 0;
    player.rdel = 0;
    player.rAcc = 0;
    player.col = 20;
    player.row = SCREENHEIGHT - player.height - 25;

    player.aniCounter = 0;
    player.curFrame = 0;
    player.numFrames = 3;
    player.aniState = 0;

}

void updatePlayer() {
    if (BUTTON_PRESSED(BUTTON_UP)) {
        player.rdel = -10;
        player.rAcc = SHIFTDOWN(260);
        player.row--;
        player.curFrame = 0;
        player.aniState = 8 + cheat;
        jump = 1;
    }

    if (player.row == SCREENHEIGHT - player.height - 25) {
        jump = 0;
        player.aniState = cheat;
    }

    if (player.row < SCREENHEIGHT - player.height - 25 && player.row >= 0 - player.height) {
        player.rdel += player.rAcc;
        player.row += player.rdel;
        player.col += player.cdel;
        if (player.row < 0) {
            player.row = 0;
        }
    } else {
        player.row = SCREENHEIGHT - player.height - 25;
    }

    //implement cheat
    if (BUTTON_PRESSED(BUTTON_A)) {
        if (cheat != 4) {
            cheat = 4;
            player.aniState = cheat;
        } else {
            cheat = 0;
            player.aniState = cheat;
        }
    }

}

void drawPlayer() {
    //set up player sprite attributes
    shadowOAM[4].attr0 = ATTR0_REGULAR | ATTR0_4BPP | ATTR0_SQUARE | player.row;
    shadowOAM[4].attr1 = ATTR1_MEDIUM | player.col;
    shadowOAM[4].attr2 = ATTR2_TILEID(player.aniState, player.curFrame) | ATTR2_PALROW(0);

}

void initializeBones() {
    for (int i = 0; i < BONECOUNT; i++) {
        bones[i].width = 16;
        bones[i].height = 16;
        bones[i].cdel = 0;
        bones[i].rdel = 0;
        bones[i].rAcc = 0;
        bones[i].worldCol = rand() % 1000 + hOff + 20;
        bones[i].worldRow = rand() % 130 + 1;

        bones[i].aniCounter = 0;
        bones[i].curFrame = 12;
        bones[i].numFrames = 3;
        bones[i].aniState = 0;
        bones[i].active = 1;
    }
}

void updateBones(SPRITE *a) {

    //if the dog collides with the bone, bone gets eaten
    if (a->active && collision(player.col, player.row, player.width, player.height, a->col, a->row, a->width, a->height)) {
        a->active = 0;
        score++;
    }

    //move the bones as the world moves
    if (BUTTON_HELD(BUTTON_RIGHT)) {
        a->worldCol--;
    }
    // if (a->col + a-> width < 0) {
    //     a->active = 0;
    // }

    a->col = a->worldCol - hOff;
    a->row = a->worldRow - vOff;

}

void drawBones() {
    //set up bone sprite attributes
    for (int i = 0; i < BONECOUNT; i++) {
        if (bones[i].active && bones[i].col > 0 && bones[i].col <= 239) {
            shadowOAM[5 + i].attr0 = ATTR0_4BPP | ATTR0_SQUARE | bones[i].row;
            shadowOAM[5 + i].attr1 = ATTR1_SMALL | bones[i].col;
            shadowOAM[5 + i].attr2 = ATTR2_TILEID(bones[i].aniState, bones[i].curFrame) | ATTR2_PALROW(0);
        } else {
            shadowOAM[5 + i].attr0 = ATTR0_HIDE;
        }
    }
}

void initializeMeat() {
    for (int i = 0; i < MEATCOUNT; i++) {
        meat[i].width = 16;
        meat[i].height = 16;
        meat[i].cdel = 0;
        meat[i].rdel = 0;
        meat[i].rAcc = 0;
        meat[i].worldCol = rand() % 1000 + hOff + 20;
        meat[i].worldRow = rand() % 130 + 1;

        meat[i].aniCounter = 0;
        meat[i].curFrame = 12;
        meat[i].numFrames = 1;
        meat[i].aniState = 2;
        meat[i].active = 1;
    }
}

void updateMeat(SPRITE *a) {

    //if the dog collides with the meat, meat gets eaten
    if (a->active && collision(player.col, player.row, player.width, player.height, a->col, a->row, a->width, a->height)) {
        a->active = 0;
        score += 3;
    }

    //move the meat as the world moves
    if (BUTTON_HELD(BUTTON_RIGHT)) {
        a->worldCol--;
    }
    // if (a->col + a-> width < 0) {
    //     a->active = 0;
    // }

    a->col = a->worldCol - hOff;
    a->row = a->worldRow - vOff;

}

void drawMeat() {
    //set up meat sprite attributes
    for (int i = 0; i < MEATCOUNT; i++) {
        if (meat[i].active && meat[i].col > 0 && meat[i].col <= 239) {
            shadowOAM[5 + BONECOUNT + i].attr0 = ATTR0_4BPP | ATTR0_SQUARE | meat[i].row;
            shadowOAM[5 + BONECOUNT + i].attr1 = ATTR1_SMALL | meat[i].col;
            shadowOAM[5 + BONECOUNT + i].attr2 = ATTR2_TILEID(meat[i].aniState, meat[i].curFrame) | ATTR2_PALROW(0);
        } else {
            shadowOAM[5 + BONECOUNT + i].attr0 = ATTR0_HIDE;
        }
    }
}

void initializeRivers() {
    for (int i = 0; i < RIVERCOUNT; i++) {
        rivers[i].width = 16;
        rivers[i].height = 32;
        rivers[i].cdel = 0;
        rivers[i].rdel = 0;
        rivers[i].rAcc = 0;
        rivers[i].worldCol = rand() % 1000 + hOff + 20;
        rivers[i].worldRow = SCREENHEIGHT - 32;

        rivers[i].aniCounter = 0;
        rivers[i].curFrame = 12;
        rivers[i].numFrames = 1;
        rivers[i].aniState = 4;
        rivers[i].active = 1;
    }

}

void updateRivers(SPRITE *a) {

    //if the dog collides with the river, dog loses a life
    if (a->active && collision(player.col, player.row, player.width, player.height, a->col, a->row, a->width, a->height)
    && !cheat) {
        a->active = 0;
        livesRemaining--;
        playSoundB(splash, SPLASHLEN, SPLASHFREQ, 0);
        for (int j = 0; j < LIVESCOUNT; j++) {
            if (lives[j].active) {
                lives[j].active = 0;
                break;
            }
        }
    }

    //move the river as the world moves
    if (BUTTON_HELD(BUTTON_RIGHT)) {
        a->worldCol--;
    }
    // if (a->col + a-> width < 0) {
    //     a->active = 0;
    // }

    a->col = a->worldCol - hOff;
    a->row = a->worldRow - vOff;

}

void drawRivers() {
    //set up river sprite attributes
    for (int i = 0; i < RIVERCOUNT; i++) {
        if (rivers[i].active && rivers[i].col > 0 && rivers[i].col <= 239) {
            shadowOAM[5 + BONECOUNT + MEATCOUNT + i].attr0 = ATTR0_4BPP | ATTR0_TALL | rivers[i].row;
            shadowOAM[5 + BONECOUNT + MEATCOUNT + i].attr1 = ATTR1_MEDIUM | rivers[i].col;
            shadowOAM[5 + BONECOUNT + MEATCOUNT + i].attr2 = ATTR2_TILEID(rivers[i].aniState, rivers[i].curFrame) | ATTR2_PALROW(0);
        } else {
            shadowOAM[5 + BONECOUNT + MEATCOUNT + i].attr0 = ATTR0_HIDE;
        }
    }
}

void initializeSplashes() {
    for (int i = 0; i < SPLASHCOUNT; i++) {
        splashes[i].width = 16;
        splashes[i].height = 16;
        splashes[i].cdel = 0;
        splashes[i].rdel = 0;
        splashes[i].rAcc = 0;
        splashes[i].worldCol = rand() % 1000 + hOff + 20;
        splashes[i].worldRow = 0;

        splashes[i].aniCounter = 0;
        splashes[i].curFrame = 12;
        splashes[i].numFrames = 1;
        splashes[i].aniState = 6;
        splashes[i].active = 1;
    }
}

void updateSplashes(SPRITE *a) {
    //if the dog collides with the splash, dog loses a life
    if (a->active && collision(player.col, player.row, player.width, player.height, a->col, a->row, a->width, a->height)
    && !cheat) {
        a->active = 0;
        livesRemaining--;
        playSoundB(splash, SPLASHLEN, SPLASHFREQ, 0);
        for (int j = 0; j < LIVESCOUNT; j++) {
            if (lives[j].active) {
                lives[j].active = 0;
                break;
            }
        }
    }

    //move the river as the world moves
    if (BUTTON_HELD(BUTTON_RIGHT)) {
        a->worldCol--;
    }
    // if (a->col + a-> width < 0) {
    //     a->active = 0;
    // }

    a->rAcc = SHIFTDOWN(260);
    a->col = a->worldCol - hOff;
    a->row = a->worldRow - vOff;

    if (a->row < SCREENHEIGHT && a->col > 0 && a->col <= 239) {
        a->rdel += a->rAcc;
        a->row += a->rdel;

        if (a->aniCounter % 29 == 0) {
            //bones[i].curFrame = 9 + (bones[i].curFrame + 2) % (bones[i].numFrames * 2);
            a->curFrame += 2;

        }
            a->aniCounter++;
    }
    if (a->row >= SCREENHEIGHT - 35) {
        a->active = 0;
    }

}

void drawSplashes() {
    //set up splash sprite attributes
    for (int i = 0; i < SPLASHCOUNT; i++) {
        if (splashes[i].active && splashes[i].col > 0 && splashes[i].col <= 239) {
            shadowOAM[5 + BONECOUNT + MEATCOUNT + i].attr0 = ATTR0_4BPP | ATTR0_SQUARE | splashes[i].row;
            shadowOAM[5 + BONECOUNT + MEATCOUNT + i].attr1 = ATTR1_SMALL | splashes[i].col;
            shadowOAM[5 + BONECOUNT + MEATCOUNT + i].attr2 = ATTR2_TILEID(splashes[i].aniState, splashes[i].curFrame) | ATTR2_PALROW(0);
        } else {
            shadowOAM[5 + BONECOUNT + MEATCOUNT + i].attr0 = ATTR0_HIDE;
        }
    }
}

void initializeLives() {
    for (int i = 0; i < LIVESCOUNT; i++) {
        lives[i].width = 16;
        lives[i].height = 16;
        lives[i].cdel = 0;
        lives[i].rdel = 0;
        lives[i].col = 64 - 16*i;
        lives[i].row = 140;

        lives[i].aniCounter = 0;
        lives[i].aniState = 8;
        lives[i].curFrame = 12;
        lives[i].numFrames = 1;
        lives[i].active = 1;
    }
}
