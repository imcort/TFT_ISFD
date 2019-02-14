#include <MCPanel.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <FS.h>
//#include "ISFD_attitude_ball.h"

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
TFT_eSprite img = TFT_eSprite(&tft);
MCPanel mcp;
int star_color = TFT_WHITE;

void drawX(int x, int y)
{
  tft.drawLine(x - 5, y - 5, x + 5, y + 5, TFT_BLACK);
  tft.drawLine(x - 5, y + 5, x + 5, y - 5, TFT_BLACK);
}

void drawbar(int r) {

  img.fillSprite(TFT_TRANSPARENT);
  for (int i = 0; i < 8; i++) {
    img.fillRect(32, 28 * i, 21, 2, star_color);
    img.fillRect(39, 7 + 28 * i, 7, 2, star_color);
    img.fillRect(25, 14 + 28 * i, 35, 2, star_color);
    img.fillRect(39, 21 + 28 * i, 7, 2, star_color);

    img.setTextColor(star_color);    // Red text in Sprite
    img.setTextDatum(ML_DATUM);   // Middle centre datum
    img.drawNumber(10 * i, 65, 15 + 28 * i, 2);
    img.setTextDatum(MR_DATUM);   // Middle centre datum
    img.drawNumber(10 * i, 20, 15 + 28 * i, 2);
  }
  //img.fillTriangle(42, 0, 35, 14, 49, 14, star_color);
  //img.drawRect(0, 0, 85, 204, star_color);
  tft.setPivot(140, 102);
  img.setPivot(43, 102);

  img.pushRotated(r, TFT_TRANSPARENT);
  //img.pushSprite(100, 0, 0);

}

void setup(void) {
  mcp.begin();
  Serial.begin(115200);
  tft.init();
  tft.setRotation(3);

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nSPIFFS available!");

  // ESP32 will crash if any of the fonts are missing
  bool font_missing = false;
  if (SPIFFS.exists("/NotoSansBold15.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/NotoSansBold36.vlw")    == false) font_missing = true;

  if (font_missing)
  {
    Serial.println("\r\nFont missing in SPIFFS, did you upload it?");
    while (1) yield();
  }
  else Serial.println("\r\nFonts found OK.");

  tft.fillScreen(TFT_BLACK);




  //drawX(160, 120);

  //drawvbar();
  //drawvbar2();

  tft.loadFont("NotoSansBold15");
  tft.setTextColor(ILI9341_GREEN, TFT_BLACK);

  img.setColorDepth(2);
  img.createSprite(85, 204);

}

void loop() {

  for (int i = -180; i < 180; i++) {
    uint32_t t = millis();
    tft.fillRect(50, 0, 180, 102, 0x0438);
    tft.fillRect(50, 102, 180, 102, 0xA1E0);
    drawbar(i);
    tft.fillRect(220, 62, 10, 80, ILI9341_BLACK); //ILS Verticle
    tft.fillRect(0, 0, 50, 87, ILI9341_DARKGREY); //Left Speed Background
    tft.fillRect(0, 117, 50, 87, ILI9341_DARKGREY); //Left Speed Background
    tft.drawRect(0, 87, 50, 30, ILI9341_WHITE);
    tft.fillRect(1, 88, 48, 28, ILI9341_BLACK);
    tft.setTextDatum(MR_DATUM);
    tft.drawNumber(350, 44, 103, 2);
    //tft.drawString((String)350, 44, 103);

    tft.fillRect(291, 0, 29, 204, ILI9341_DARKGREY); //Right VS Background

    tft.fillRect(230 , 0, 60, 87, ILI9341_DARKGREY); //Right Altitude Background
    tft.fillRect(230 , 117, 60, 87, ILI9341_DARKGREY); //Right Altitude Background
    tft.drawRect(230, 87, 60, 30, ILI9341_WHITE);
    tft.fillRect(231, 88, 58, 28, ILI9341_BLACK);
    tft.setTextDatum(ML_DATUM);
    tft.drawNumber(8000, 236, 103, 2);
    //tft.drawString((String)8000, 236, 103);

    tft.fillRect(50, 204, 180, 36, ILI9341_DARKGREY); //Bottom Heading Background

    tft.setTextDatum(BR_DATUM);
    tft.drawString((String)(1000 / (millis() - t)), 320, 240);
    delay(500);

  }


}
