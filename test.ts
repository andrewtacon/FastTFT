// tests go here; this will not be compiled when this package is used as an extension.
FastTFT.init()
FastTFT.clear(0)
FastTFT.fillRect(10, 10, 40, 40, 63488)
FastTFT.fillRect(70, 10, 40, 40, 2016)
FastTFT.createFramebuffer()
FastTFT.clearFramebuffer(0)
FastTFT.fillFramebufferRect(30, 70, 68, 30, 65504)
FastTFT.show()

