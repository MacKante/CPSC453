# Lecture 2

#### September 10, 2024

---

## Graphics Systems Continued...

### GPU (Graphics Processing Unit)

- A specialized processor typically found on graphics cards.
- **Designed to accelarate graphics rendering and perform parallel processing tasks**

### GPU Pipeline

![GPU Pipeline](../z_ImageDump/GPU%20Pipeline.jpg)

1. **Vertex Processing:** This stage handles the transformation and creation of geometric primitives.

2. **Rasterization:** Converts the geometric primitives into fragments, which are essentially potential pixels. This process determines which pixels on the screen will be affected by each primitive.

3. **Fragment Shader:** Processes each fragment to determine its final color and other attributes, such as texture and lighting effects. This stage allows for detailed and complex visual effects to be applied to each pixel.

4. **Blending:** Combines the processed fragments to form the final pixel colors.

### Frame Buffer
- A region of memory usually on a graphics card, that is large enough to hold a value (color or intensity) for every pixel in the display.
- Holds all pixel values:
    - (*x resolution* pixels) * (*y resolution* pixels)
- Depth of the frame buffer:
    - determines how many colors the frame buffer can represent
    - number of bits available to each pixel
    - often referred to as the ***color depth*** or the number of ***planes***

![Frame Buffer](../z_ImageDump/Frame%20Buffer.jpg)

#### Example...

![A small screen example](../z_ImageDump/A%20small%20screen%20example.jpg)

**How many unique colors can an RGB frame buffer with 6 planes represent?**
- Each RGB channel is allotted 6/3 = 2 bits
- These 2 bits can produce 2^2 = 4 colors in each channel
- Total number of colors that can be represented is 4*4*4 = 64

**Also, what would be the frame buffer size for a display screen of eight by eight pixels with a depth of six bits?**
- Screen size times the color depth
- 8 * 8 * 6 = 384 bits = 48 bytes

**24 bit planes?**
- Each RGB channel is given 8 bits (24/3 = 8) and can represent 2^8 shades
- All channels combined can represent (2^8)^3 = 2^24 = 16,777,216, or 16.7 million, colors


## Assignment 1: Serpinsky Triangle

- sub_trianlge(A, B, C, d)
- sub_triangle(A, D, F, d-1)
- sub_triangle(D, B, E, d-1)
- sub_triangle(F, E, C, d-1)
- ~~sub_triangle(D, E, F, -d-1)~~