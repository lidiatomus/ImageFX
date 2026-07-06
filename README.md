# ImageFX

ImageFX is a C++ application developed using **OpenCV** that applies various artistic and image processing effects to both **images** and **videos**. The project was developed for the *Digital Image Processing* course and demonstrates the implementation of common filtering and stylization techniques.

---

## Features

### Image Effects
- Grayscale
- Blur
- Edge Detection (Canny)
- Sepia
- Color Inversion
- Pixelation
- Emboss
- Sharpen
- Posterization
- Contrast Adjustment
- Cartoon Effect
- Pencil Sketch
- Oil Painting
- Vignette
- Glow / Soft Light
- Color Quantization

### Video Effects
- Grayscale
- Sepia
- Cartoon
- Inversion
- Blur
- Pixelation
- Pencil Sketch

Video effects can be applied to:
- Webcam stream
- Video files

---

## Technologies

- C++
- OpenCV
- Visual Studio

---

## Project Structure

```
ImageFX
│
├── OpenCVApplication.cpp
├── common.h
├── stdafx.h
├── Images/
└── README.md
```

---

## How It Works

Each effect processes an image (or video frame) using one or more image processing techniques such as:

- Pixel-wise transformations
- Convolution filtering
- Color space manipulation
- Edge detection
- Image blending
- Thresholding

For video processing, every frame is treated as an individual image and processed in real time.

---

## Running the Project

1. Clone the repository

```bash
git clone https://github.com/your_username/ImageFX.git
```

2. Open the solution in Visual Studio.

3. Make sure OpenCV is correctly configured.

4. Build and run the project.

---

## Menu

```
1  - Grayscale conversion
2  - Blur effect
3  - Edge detection
4  - Sepia effect
5  - Color inversion
6  - Pixelation effect
7  - Emboss effect
8  - Sharpen effect
9  - Posterization effect
10 - Contrast adjustment
11 - Cartoon effect
12 - Pencil sketch effect
13 - Oil painting effect
14 - Vignette effect
15 - Glow / Soft light effect
16 - Color quantization effect
17 - Grayscale video
18 - Sepia video
19 - Cartoon video
20 - Inversion video
21 - Blur video
22 - Pixelation video
23 - Pencil sketch video
```

---

## Future Improvements

- More artistic filters
- Parameter adjustment through sliders
- Save processed images and videos
- Real-time filter switching
- Face detection and tracking
- GPU acceleration
