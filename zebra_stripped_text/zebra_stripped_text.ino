#include <Arduboy.h>

Arduboy aboy;

int frameRate = 5;int zebraStripeIndex = 0;

void setup() {
  aboy.begin();
  //Lower frame rate saves battery life
  aboy.setFrameRate(frameRate);
}

void loop() {
  if (!aboy.nextFrame()){
    return;
  }

  draw();  
}

void draw(){
  aboy.clear();
  drawZebraStrippedText(0, 10, "zahn", 5);
  aboy.display();
}

void drawZebraStrippedText(int x, int y, char *msg, int charSize){
  int numChars = strlen(msg);
  for (int i = 0; i <numChars;i++){
    int backColor;
    int textColor;
    if (i % 2 == zebraStripeIndex){
      backColor = 0;
      textColor = 255;
    }
    else{
      backColor = 255;
      textColor = 0;
    }
    aboy.drawChar(x + (((charSize * 6) + 2) * i), y, msg[i],textColor, backColor, charSize);
  }
  zebraStripeIndex = (zebraStripeIndex == 0) ? 1 : 0;
}

