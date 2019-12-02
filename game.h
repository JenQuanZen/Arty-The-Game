//sprite struct
typedef struct Sprite {
    //position
    int row;
	int col;
    int worldCol;
    int worldRow;

    //movement
    int rdel;
    int cdel;
    int rAcc;

    //size
	int width;
    int height;

    //animation
    int aniCounter;
    int aniState;
    int prevAniState;
    int curFrame;
    int numFrames;
    int active;

    //buttons
    int isSelected;
} SPRITE;

//constants
#define LIVES 3
#define BONECOUNT 10
#define MEATCOUNT 5
#define RIVERCOUNT 10
#define SPLASHCOUNT 5
#define LIVESCOUNT 5

//variables
extern SPRITE player;
extern int livesRemaining;
int level;
int score;
int highScore;
int jump;

//game prototypes
void initializeGame();
void updateGame();
void drawGame();

//player prototypes
void initializePlayer();
void updatePlayer();
void drawPlayer();

//bone prototypes
void initializeBones();
void updateBones(SPRITE *);
void drawBones();

//meat prototypes
void initializeMeat();
void updateMeat(SPRITE *);
void drawMeat();

//river prototypes
void initializeRivers();
void updateRivers(SPRITE *);
void drawRivers();


//water splash prototypes
void initializeSplashes();
void updateSplashes(SPRITE *);
void drawSplashes();

//lives prototypes
void initializeLives();


/******************************* screens variables and prototypes ***********************************/
//variables
extern SPRITE startButton;
extern SPRITE instructButton;
extern SPRITE quitButton;
extern SPRITE arrow;
extern SPRITE sitting;
extern SPRITE sleeping;
int screen;

//prototypes
void initializeButtons();
void updateButtons();
void drawButtons();