// tests go here; this will not be compiled when this package is used as an extension.
FastTFT.api.init()
FastTFT.api.clear(0)
FastTFT.api.fillRect(10, 10, 40, 40, 63488)
FastTFT.api.fillRect(70, 10, 40, 40, 2016)
FastTFT.api.createFramebuffer()
FastTFT.api.clearFramebuffer(0)
FastTFT.api.fillFramebufferRect(30, 70, 68, 30, 65504)
FastTFT.api.show()
