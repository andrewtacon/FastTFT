#include "pxt.h"
#include "MicroBit.h"
#include "SPI.h"

/*
 * FastTFT128 - native CODAL SPI ST7735 driver for micro:bit MakeCode.
 *
 * Display: ST7735, 128x128, RGB565
 * MOSI: P15
 * MISO: P14
 * SCK : P13
 * DC  : P1
 * CS  : P16
 *
 * Uses the same CODAL SPI mechanism used by the micro:bit PXT runtime,
 * but accesses the SPI object directly so this file does not depend on
 * unqualified PXT pins.cpp functions being visible to the extension.
 */

namespace FastTFT
{
    static const int WIDTH = 128;
    static const int HEIGHT = 128;

    static const int SWRESET = 0x01;
    static const int SLPOUT  = 0x11;
    static const int NORON   = 0x13;
    static const int INVOFF  = 0x20;
    static const int DISPON  = 0x29;
    static const int CASET   = 0x2A;
    static const int RASET   = 0x2B;
    static const int RAMWR   = 0x2C;
    static const int MADCTL  = 0x36;
    static const int COLMOD  = 0x3A;
    static const int FRMCTR1 = 0xB1;
    static const int FRMCTR2 = 0xB2;
    static const int INVCTR  = 0xB4;
    static const int PWCTR1  = 0xC0;
    static const int PWCTR2  = 0xC1;
    static const int PWCTR3  = 0xC2;
    static const int PWCTR4  = 0xC3;
    static const int PWCTR5  = 0xC4;
    static const int VMCTR1  = 0xC5;
    static const int GMCTRP1 = 0xE0;
    static const int GMCTRN1 = 0xE1;

    static SPI *spi = NULL;
    static Buffer framebuffer = mkBuffer(NULL, WIDTH *HEIGHT);
    //NULL;
    static bool initialized = false;

    static void dc(int value)
    {
        uBit.io.P1.setDigitalValue(value);
    }

    static void cs(int value)
    {
        uBit.io.P16.setDigitalValue(value);
    }

    static void configureSPI()
    {
        if (!spi)
        {
            // Same hardware SPI pins used by the PXT micro:bit runtime:
            // MOSI=P15, MISO=P14, SCK=P13.
            spi = new SPI(MOSI, MISO, SCK);
        }

        spi->format(8, 0);
        spi->frequency(16000000);
    }

    static void spiByte(uint8_t value)
    {
        if (!spi)
            configureSPI();

        spi->write(value);
    }

    static void spiBuffer(Buffer b)
    {
        if (!spi)
            configureSPI();

        if (b && b->length)
            for (int i = 0; i < b->length; i++){
                spi->write(b->data[i]);
                // spi->write(b->data[i]);
            }

            // spi->transfer((const char*)b->data, b->length, NULL, 0);
    }

    static void command(uint8_t cmd)
    {
        cs(0);
        dc(0);
        spiByte(cmd);
        cs(1);
    }

    static void commandData(uint8_t cmd, const uint8_t *data, int len)
    {
        cs(0);
        dc(0);
        spiByte(cmd);
        dc(1);

        for (int i = 0; i < len; ++i)
            spiByte(data[i]);

        cs(1);
    }

    static void setWindow(int x0, int y0, int x1, int y1)
    {
        dc(0);
        spiByte(CASET);
        dc(1);
        spiByte((x0 >> 8) & 0xff);
        spiByte(x0 & 0xff);
        spiByte((x1 >> 8) & 0xff);
        spiByte(x1 & 0xff);

        dc(0);
        spiByte(RASET);
        dc(1);
        spiByte((y0 >> 8) & 0xff);
        spiByte(y0 & 0xff);
        spiByte((y1 >> 8) & 0xff);
        spiByte(y1 & 0xff);

        dc(0);
        spiByte(RAMWR);
        dc(1);
    }

    static void begin()
    {
        configureSPI();

        cs(1);
        dc(1);

        command(SWRESET);
        fiber_sleep(120);

        command(SLPOUT);
        fiber_sleep(120);

        const uint8_t a[] = {0x01, 0x2C, 0x2D};
        commandData(FRMCTR1, a, 3);

        const uint8_t b[] = {0x01, 0x2C, 0x2D};
        commandData(FRMCTR2, b, 3);

        const uint8_t c[] = {0x07};
        commandData(INVCTR, c, 1);

        const uint8_t d[] = {0xA2, 0x02, 0x84};
        commandData(PWCTR1, d, 3);

        const uint8_t e[] = {0xC5};
        commandData(PWCTR2, e, 1);

        const uint8_t f[] = {0x0A, 0x00};
        commandData(PWCTR3, f, 2);

        const uint8_t g[] = {0x8A, 0x2A};
        commandData(PWCTR4, g, 2);

        const uint8_t h[] = {0x8A, 0xEE};
        commandData(PWCTR5, h, 2);

        const uint8_t i[] = {0x0E};
        commandData(VMCTR1, i, 1);

        command(INVOFF);

        const uint8_t j[] = {0xC8};
        commandData(MADCTL, j, 1);

        // 16-bit RGB565
        const uint8_t k[] = {0x05};
        commandData(COLMOD, k, 1);

        fiber_sleep(10);

        const uint8_t cols[] = {0x00, 0x00, 0x00, 0x7F};
        commandData(CASET, cols, 4);

        const uint8_t rows[] = {0x00, 0x00, 0x00, 0x7F};
        commandData(RASET, rows, 4);

        const uint8_t gp[] = {
            0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D,
            0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10
        };
        commandData(GMCTRP1, gp, 16);

        const uint8_t gn[] = {
            0x03, 0x1D, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D,
            0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10
        };
        commandData(GMCTRN1, gn, 16);

        command(NORON);
        fiber_sleep(10);

        command(DISPON);
        fiber_sleep(100);

        initialized = true;
    }

    static void ensure()
    {
        if (!initialized)
            begin();
    }

    //% block="initialize TFT"
    //% weight=100
    void init()
    {
        begin();
    }

    //% block="set SPI speed to $hz Hz"
    //% hz.min=1000000 hz.max=24000000 hz.defl=16000000
    //% weight=98
    void setSPISpeed(int hz)
    {
        ensure();

        if (hz < 1000000) hz = 1000000;
        if (hz > 24000000) hz = 24000000;

        spi->format(8, 0);
        spi->frequency(hz);
    }

    //% block="RGB565 red $red green $green blue $blue"
    //% red.min=0 red.max=255 green.min=0 green.max=255 blue.min=0 blue.max=255
    //% weight=60
    int rgb(int red, int green, int blue)
    {
        if (red < 0) red = 0;
        if (red > 255) red = 255;
        if (green < 0) green = 0;
        if (green > 255) green = 255;
        if (blue < 0) blue = 0;
        if (blue > 255) blue = 255;

        return ((red & 0xF8) << 8) |
               ((green & 0xFC) << 3) |
               (blue >> 3);
    }

    
    
    
    // //% block="create framebuffer"
    // //% weight=75
    // void createFramebuffer()
    // {
    //     ensure();

    //     if (!framebuffer)
    //         framebuffer = mkBuffer(NULL, WIDTH * HEIGHT);
    // }

    //% block="clear framebuffer with color $color"
    //% weight=74
    void clearFramebuffer(int color)
    {
        // createFramebuffer();

        for (int i = 0; i < WIDTH * HEIGHT; i++)
        {
            framebuffer->data[i] = color;
        }
    }

    //% block="set framebuffer pixel x $x y $y color $color"
    //% x.min=0 x.max=127 y.min=0 y.max=127
    //% weight=73
    void setPixel(int x, int y, int color)
    {
        // createFramebuffer();

        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
            return;

        int p = (y * WIDTH + x) ;
        framebuffer->data[p] = color;
    }

    //% block="fill framebuffer rectangle x $x y $y width $width height $height color $color"
    //% x.min=0 x.max=127 y.min=0 y.max=127 width.min=1 width.max=128 height.min=1 height.max=128
    //% weight=72
    void fillFramebufferRect(int x, int y, int width, int height, int color)
    {
        // createFramebuffer();

        if (width <= 0 || height <= 0)
            return;

        int x0 = x < 0 ? 0 : x;
        int y0 = y < 0 ? 0 : y;
        int x1 = x + width - 1;
        int y1 = y + height - 1;

        if (x1 >= WIDTH) x1 = WIDTH - 1;
        if (y1 >= HEIGHT) y1 = HEIGHT - 1;

        if (x0 > x1 || y0 > y1)
            return;


        for (int yy = y0; yy <= y1; ++yy)
        {
            int p = (yy * WIDTH + x0) ;

            for (int xx = x0; xx <= x1; ++xx)
            {
                framebuffer->data[p] = color;
                p = p + 1;
            }
        }
    }

    //% block="show framebuffer"
    //% weight=70
    void show()
    {
        // createFramebuffer();

        cs(0);
        setWindow(0, 0, 127, 127);

        // 32,768 bytes in one native CODAL SPI transfer.
        spiBuffer(framebuffer);

        cs(1);
    }
}
