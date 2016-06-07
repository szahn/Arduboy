#include <Arduboy.h>

Arduboy aboy;

int frameRate = 25;
int tileSize = 4;
int charSize = 3;
int moveTicks = 0;

int exitX;
int exitY;

int playerX;
int playerY;

int coins[30];
int totalCoins = 10;

int playerFace;

// pin values for buttons, probably shouldn't use these
#define PIN_LEFT_BUTTON A2
#define PIN_RIGHT_BUTTON A1
#define PIN_UP_BUTTON A0
#define PIN_DOWN_BUTTON A3
#define PIN_A_BUTTON 7
#define PIN_B_BUTTON 8


void setup() {
  aboy.begin();
  //Lower frame rate saves battery life
  aboy.setFrameRate(frameRate);
  initMap();
}

void loop() {
  if (!aboy.nextFrame()){
    return;
  }

  draw();  
}

void draw(){
  aboy.clear();  
  drawMap();
  drawPlayer();
  aboy.display();
  tick();
}

void tick(){
  if (digitalRead(PIN_UP_BUTTON) == 0){
    playerFace = 0;
  }
  if (digitalRead(PIN_RIGHT_BUTTON) == 0){
    playerFace = 1;
  }
  if (digitalRead(PIN_DOWN_BUTTON) == 0){
    playerFace = 2;
  }
  if (digitalRead(PIN_LEFT_BUTTON) == 0){
    playerFace = 3;
  }

  movePlayer();  
}


void movePlayer(){
  if (moveTicks-- <= 0){
    int mapWidth = WIDTH / tileSize;
    int mapHeight = HEIGHT / tileSize;
    moveTicks = 5;
    int nextX = playerX;
    int nextY = playerY;
    switch (playerFace){
      case 0:{
        nextY -=1;
        break;
      }
      case 1:{
        nextX +=1;
        break;
      }
      case 2:{
        nextY +=1;
        break;
      }
      case 3:{
        nextX -=1;
        break;
      }
    }

    if (totalCoins == 0 && nextX >= exitX && nextY >= exitY && nextX <= exitX + 3 && nextY <= exitY + 3){
      initMap();
      return;
    }

    if (nextX <= 1){
      playerFace = 1;
      return;      
    }

    if (nextY <= 1){
      playerFace = 2;
      return;      
    }

    if (nextY == mapHeight - 1){
      playerFace = 0;
      return;      
    }

    if (nextX == mapWidth - 1){
      playerFace = 3;
      return;      
    }

    int nextLeft = nextX * tileSize;
    int nextRight = nextLeft + charSize;
    int nextTop = nextY * tileSize;
    int nextBottom = nextTop + charSize;
    for (int i = 0; i < 10; i++){
      int index = i * 3;
      if (1 == coins[index]){
        int x= coins[1 + index] * tileSize;
        int y = coins[2 + index]* tileSize;        
        if (nextLeft < x + 1 && nextRight  > x && nextTop < y + 1 && nextBottom > y){
          totalCoins -=1;
          coins[index] = 0;
        }
      }
    }


    playerX = nextX;
    playerY = nextY;

  }
}

void initMap(){
  aboy.initRandomSeed();
  totalCoins = 10;
  spawnLocations();
  spawnCoins();
}

void spawnCoins(){
  int mapWidth = WIDTH / tileSize;
  int mapHeight = HEIGHT / tileSize;
  for (int i = 0; i < 10; i++){
    int index = i * 3;
    int x = random(2, mapWidth - 2);
    int y = random(2, mapHeight - 2);
    coins[index] = 1;
    coins[1 + index] = x;
    coins[2 + index] = y;
  }
}

void spawnLocations(){
  int mapWidth = WIDTH / tileSize;
  int mapHeight = HEIGHT / tileSize;
  int exitSide = random(0, 3);
  switch (exitSide){
    case 0: /* North Exit */{
      exitX = random(1, mapWidth - 2);
      exitY = 0;    
      playerX = random(2, mapWidth - 3);
      playerY = mapHeight - 3; 
      playerFace = 0;
      break;
    }
    case 1: /* South */{
      exitX = random(1, mapWidth - 2);
      exitY = mapHeight - 1;
      playerX = random(2, mapWidth - 3);
      playerY = 3; 
      playerFace = 2;
      break;
    }
    case 2: /* West */{
      exitY = random(1, mapHeight - 2);
      exitX = 0;
      playerX = mapWidth - 3;
      playerY = random(2, mapHeight - 3); 
      playerFace = 3;
      break;
    }
    case 3: /* East */{
      exitY = random(1, mapHeight - 2);
      exitX = mapWidth - 1;
      playerX = 2;
      playerY = random(2, mapHeight - 3); 
      playerFace = 1;
      break;
    }    
  }
}

void drawMap(){
  int mapWidth = WIDTH / tileSize;
  int mapHeight = HEIGHT / tileSize;
  for (int x = 0; x < mapWidth; x++){
    drawWall(x, 0);
    drawWall(x, mapHeight - 1);
  }

  for (int y = 1; y < mapHeight - 1; y++){
    drawWall(0, y);
    drawWall(mapWidth - 1, y);
  }

  drawExit();
  drawCoins();

}

void drawCoins(){
  for (int i = 0; i < 10; i++){
    int index = i * 3;
    if (1 == coins[index]){
      aboy.drawCircle(coins[1 + index] * tileSize, coins[2 + index]  * tileSize, 1, 255);
    }
  }
}

void drawPlayer(){
  aboy.fillCircle(playerX * tileSize, playerY * tileSize, charSize, 255);
  //aboy.drawChar(playerX * tileSize, playerY * tileSize, 'c', 0, 255, 1);
}

void drawExit(){
  if (totalCoins == 0){
     aboy.fillRect(exitX *tileSize, exitY * tileSize, tileSize * 4, tileSize * 4, 0);
  }
}

void drawWall(int x,int y){
   int x1 = x *tileSize;
   int y1 = y * tileSize;
   aboy.fillRect(x1, y1, tileSize, tileSize, 255);
}


