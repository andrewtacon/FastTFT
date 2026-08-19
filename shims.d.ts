// Auto-generated. Do not edit.
declare namespace api {

    /** Initialise the ST7735 TFT. */
    //% block="initialize TFT"
    //% weight=100 shim=api::init
    function init(): void;

    /** Set the native SPI clock. Start with 16000000. */
    //% block="set SPI speed to $hz Hz"
    //% hz.min=1000000 hz.max=24000000
    //% weight=98 hz.defl=16000000 shim=api::setSPISpeed
    function setSPISpeed(hz?: int32): void;

    /** RGB565 colour from 8-bit RGB values. */
    //% block="RGB565 red $red green $green blue $blue"
    //% red.min=0 red.max=255 green.min=0 green.max=255 blue.min=0 blue.max=255
    //% weight=60 shim=api::rgb
    function rgb(red: int32, green: int32, blue: int32): int32;

    /** Clear the complete 128x128 display. */
    //% block="clear screen with color $color"
    //%
    //% weight=95 color.defl=0 shim=api::clear
    function clear(color?: int32): void;

    /** Fast filled rectangle; one display window and one pixel stream. */
    //% block="fast filled rectangle x $x y $y width $width height $height color $color"
    //% x.min=0 x.max=127 y.min=0 y.max=127 width.min=1 width.max=128 height.min=1 height.max=128
    //% weight=90 shim=api::fillRect
    function fillRect(x: int32, y: int32, width: int32, height: int32, color: int32): void;

    /** Draw one pixel. Use framebuffer functions for many pixels. */
    //% block="fast pixel x $x y $y color $color"
    //% x.min=0 x.max=127 y.min=0 y.max=127
    //% weight=80 shim=api::pixel
    function pixel(x: int32, y: int32, color: int32): void;

    /** Allocate the 32 KB RGB565 framebuffer. */
    //% block="create framebuffer"
    //% weight=75 shim=api::createFramebuffer
    function createFramebuffer(): void;

    /** Clear framebuffer without sending it to the display. */
    //% block="clear framebuffer with color $color"
    //% weight=74 shim=api::clearFramebuffer
    function clearFramebuffer(color: int32): void;

    /** Set a framebuffer pixel. This does not communicate with the TFT. */
    //% block="set framebuffer pixel x $x y $y color $color"
    //% x.min=0 x.max=127 y.min=0 y.max=127
    //% weight=73 shim=api::setPixel
    function setPixel(x: int32, y: int32, color: int32): void;

    /** Fill a framebuffer rectangle without communicating with the TFT. */
    //% block="fill framebuffer rectangle x $x y $y width $width height $height color $color"
    //% x.min=0 x.max=127 y.min=0 y.max=127 width.min=1 width.max=128 height.min=1 height.max=128
    //% weight=72 shim=api::fillFramebufferRect
    function fillFramebufferRect(x: int32, y: int32, width: int32, height: int32, color: int32): void;

    /** Transfer the complete 128x128 framebuffer using one native SPI transfer. */
    //% block="show framebuffer"
    //% weight=70 shim=api::show
    function show(): void;
}

// Auto-generated. Do not edit. Really.
