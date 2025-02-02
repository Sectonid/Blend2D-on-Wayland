#include <blend2d.h>

// Window dimensions
#define WIDTH  640
#define HEIGHT 480

void *shm_data;

void Blend2D_renderTest() {
    BLImage img(WIDTH, HEIGHT, BL_FORMAT_PRGB32);
    BLContext ctx(img);

  ctx.setFillStyle(BLRgba32(0x80000000));
  ctx.fillRoundRect(10.0, 30.0, 620.0, 450.0, 16.0);
  ctx.setFillStyle(BLRgba32(0xFFFFFFFF));
  ctx.fillRoundRect(20.0, 20.0, 600.0, 440.0, 16.0);
  
  BLFontFace face;
  BLResult result = face.createFromFile("Dongle-Regular.ttf");
  if (result != BL_SUCCESS) {
    printf("\033[1;31mFailed to load a font \033[0;31m(err=%u)\033[0m\n", result);
  }

  BLFont font;
  font.createFromFace(face, 64.0f);

  ctx.setFillStyle(BLRgba32(0xFF000000));
  ctx.fillUtf8Text(BLPoint(24, 68), font, "Hello Wayland from Blend2D");

    ctx.end();  // Finish rendering

    // Get raw pixel data
    BLImageData imgData;
    img.getData(&imgData);

    // Copy Blend2D buffer into Wayland shared memory
    memcpy(shm_data, imgData.pixelData, imgData.size.h * imgData.stride);
    
    // img.releaseData(&imgData);
}
