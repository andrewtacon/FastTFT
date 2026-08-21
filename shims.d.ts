// Auto-generated. Do not edit.
declare namespace FastTFT {

    /** Initialise the ST7735 TFT. */
    //% block="initialize TFT"
    //% weight=100 shim=FastTFT::init
    function init(): void;

    /** Set the native SPI clock. Start with 16000000. */
    //% block="set SPI speed to $hz Hz"
    //% hz.min=1000000 hz.max=24000000
    //% weight=98 hz.defl=16000000 shim=FastTFT::setSPISpeed
    function setSPISpeed(hz?: int32): void;

    /** RGB565 colour from 8-bit RGB values. */
    //% block="RGB565 red $red green $green blue $blue"
    //% red.min=0 red.max=255 green.min=0 green.max=255 blue.min=0 blue.max=255
    //% weight=60 shim=FastTFT::rgb
    function rgb(red: int32, green: int32, blue: int32): int32;

    /** Clear framebuffer without sending it to the display. */
    //% block="clear framebuffer with color $color"
    //% weight=74 shim=FastTFT::clearFramebuffer
    function clearFramebuffer(color: int32): void;

    /** Set a framebuffer pixel. This does not communicate with the TFT. */
    //% block="set framebuffer pixel x $x y $y color $color"
    //% x.min=0 x.max=127 y.min=0 y.max=127
    //% weight=73 shim=FastTFT::setPixel
    function setPixel(x: int32, y: int32, color: int32): void;

    /** Fill a framebuffer rectangle without communicating with the TFT. */
    //% block="fill framebuffer rectangle x $x y $y width $width height $height color $color"
    //% x.min=0 x.max=127 y.min=0 y.max=127 width.min=1 width.max=128 height.min=1 height.max=128
    //% weight=72 shim=FastTFT::fillFramebufferRect
    function fillFramebufferRect(x: int32, y: int32, width: int32, height: int32, color: int32): void;

    /** Transfer the complete 128x128 framebuffer using one native SPI transfer. */
    //% block="show framebuffer"
    //% weight=70 shim=FastTFT::show
    function show(): void;
}

// Auto-generated. Do not edit. Really.
