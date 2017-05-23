#define MAXX 9
#define MAXY 7

# define BOARD_X 4
# define BOARD_Y 4

#define PIC_0 0
#define PIC_1 1
#define PIC_2 2
#define PIC_3 3
#define PIC_4 4
#define PIC_5 5
#define PIC_6 6
#define PIC_7 7
#define BOMB  8
#define HBOMB 9
#define VBOMB 10

#define EMPTY 11

#define NUM_PICS 8

// credits marquee
String marqueeTxt = "Instructions: Push A+DPad to match 3 or more bloops. Score as much as you can until you get out of time or blocked with no more matchs. Game by Fernando Jerez 2017. Thanks to @crait for the funny name. Version 1. Enjoy!!!";
int marqueePos = 128;
int marqueeLength= -6*marqueeTxt.length();

byte offy[MAXX][MAXY]; // Offset for animations when falling
byte board[MAXX][MAXY];
bool flag[MAXX][MAXY];
int flagCount = 0;
uint16_t flagFrame = 0;

byte xpos, ypos, prevx,prevy;

// scoring
uint16_t score = 0;
int mult_score = 1;
int tmp_score = 0;
uint16_t scoreFrame = 0;
uint16_t topscore = 0;

String txt[] = {"NICE","GREAT","BIIIG","WOW","OMG!!","WAKA!!!","WHAT??","COME ON"};

int moves=0;
int game_level = 1;

int multi[] = {0,0,0,1,2,4}; 

// timing
int maxTime = 128;
int timeLeft = 128;
int decay = 6; // every 'decay' frames substract 1 time unit (more is slower)

// States
bool title = true;
bool pause = false;
bool gameover = false;
bool falling = false;

