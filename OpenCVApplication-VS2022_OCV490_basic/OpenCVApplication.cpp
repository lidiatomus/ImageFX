// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <opencv2/core/utils/logger.hpp>
#include <fstream>

wchar_t* projectPath;

void setupImageWindow(const char* name, int x, int y, int w = 500, int h = 400)
{
	namedWindow(name, WINDOW_NORMAL);
	resizeWindow(name, w, h);
	moveWindow(name, x, y);
}

void grayscaleEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		int height = src.rows;
		int width = src.cols;

		Mat dst(height, width, CV_8UC1);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Vec3b pixel = src.at<Vec3b>(i, j);

				uchar B = pixel[0];
				uchar G = pixel[1];
				uchar R = pixel[2];

				uchar gray = (uchar)((R + G + B) / 3);

				dst.at<uchar>(i, j) = gray;
			}
		}
		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Grayscale", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Grayscale", dst);
		waitKey();
		destroyAllWindows();
	}
}

/*
Blur effect

For each pixel (i, j) in the image:
- consider a neighborhood around it
- iterate through all pixels inside this window
- sum  B, G, R 
- compute the average avg = sum / number_of_pixels
- assign this average color to the current pixel in the destination image.
- at the border check bounds and only use valid pixels

*/

void blurEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		Mat dst;
		//apply the predefined blur function with a vecinity of 5 
		blur(src, dst, Size(5, 5));

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Blur", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Blur", dst);
		waitKey();
	}
}
/* the blur function manually made
void blurEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		int height = src.rows;
		int width = src.cols;

		Mat dst(height, width, CV_8UC3);

		int k = 5;
		int r = k / 2;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int sumB = 0, sumG = 0, sumR = 0;
				int count = 0;

				for (int u = -r; u <= r; u++)
				{
					for (int v = -r; v <= r; v++)
					{
						int ni = i + u;
						int nj = j + v;

						if (ni >= 0 && ni < height && nj >= 0 && nj < width)
						{
							Vec3b pixel = src.at<Vec3b>(ni, nj);

							sumB += pixel[0];
							sumG += pixel[1];
							sumR += pixel[2];
							count++;
						}
					}
				}

				uchar avgB = (uchar)(sumB / count);
				uchar avgG = (uchar)(sumG / count);
				uchar avgR = (uchar)(sumR / count);

				dst.at<Vec3b>(i, j) = Vec3b(avgB, avgG, avgR);
			}
		}

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Blur", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Blur", dst);
		waitKey();
	}
}*/

/*
step 1: Gaussian Blur
- the image is first smoothed using a 5x5 Gaussian filter
- this reduces noise and prevents false edge detection
step 2: Canny Edge Detection
- detects edges based on intensity changes
- uses two thresholds:
	low  -> minimum edge strength
	high -> strong edge threshold
- pixels with strong gradients are marked as edges
*/
void edgeDetectionEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_GRAYSCALE);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		Mat blurred, edges;

		GaussianBlur(src, blurred, Size(5, 5), 1.0);

		int low = 50;
		int high = 150;

		Canny(blurred, edges, low, high);

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Edges", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Edges", edges);
		waitKey();
	}
}

/*
-each pixel is transformed using a combination of its RGB values
-the coefficients increase red and green tones and reduce blue
*/
void sepiaEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		int height = src.rows;
		int width = src.cols;

		Mat dst(height, width, CV_8UC3);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Vec3b pixel = src.at<Vec3b>(i, j);

				int B = pixel[0];
				int G = pixel[1];
				int R = pixel[2];

				int newR = (int)(0.393 * R + 0.769 * G + 0.189 * B);
				int newG = (int)(0.349 * R + 0.686 * G + 0.168 * B);
				int newB = (int)(0.272 * R + 0.534 * G + 0.131 * B);

				if (newR > 255) newR = 255;
				if (newG > 255) newG = 255;
				if (newB > 255) newB = 255;

				dst.at<Vec3b>(i, j)[0] = (uchar)newB;
				dst.at<Vec3b>(i, j)[1] = (uchar)newG;
				dst.at<Vec3b>(i, j)[2] = (uchar)newR;
			}
		}

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Sepia", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Sepia", dst);
		waitKey();
	}
}

void inversionEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		int height = src.rows;
		int width = src.cols;

		Mat dst(height, width, CV_8UC3);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Vec3b pixel = src.at<Vec3b>(i, j);

				dst.at<Vec3b>(i, j)[0] = 255 - pixel[0];
				dst.at<Vec3b>(i, j)[1] = 255 - pixel[1];
				dst.at<Vec3b>(i, j)[2] = 255 - pixel[2];
			}
		}

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Inversion", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Inversion", dst);
		waitKey();
	}
}

/*
- the image is divided into blocks of fixed size
- for each block, the average color is computed and all pixels in that block are replaced with this color
*/
void pixelationEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		int blockSize = 10; 

		Mat dst = src.clone();

		for (int i = 0; i < src.rows; i += blockSize)
		{
			for (int j = 0; j < src.cols; j += blockSize)
			{
				int sumB = 0, sumG = 0, sumR = 0;
				int count = 0;

				for (int bi = i; bi < i + blockSize && bi < src.rows; bi++)
				{
					for (int bj = j; bj < j + blockSize && bj < src.cols; bj++)
					{
						Vec3b pixel = src.at<Vec3b>(bi, bj);
						sumB += pixel[0];
						sumG += pixel[1];
						sumR += pixel[2];
						count++;
					}
				}

				int avgB = sumB / count;
				int avgG = sumG / count;
				int avgR = sumR / count;

				for (int bi = i; bi < i + blockSize && bi < src.rows; bi++)
				{
					for (int bj = j; bj < j + blockSize && bj < src.cols; bj++)
					{
						dst.at<Vec3b>(bi, bj) = Vec3b(avgB, avgG, avgR);
					}
				}
			}
		}

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Pixelated", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Pixelated", dst);
		waitKey();
	}
}

/*
step 1: convert the image to grayscale
step 2: apply convolution using an emboss filter (kernel)
- negative values darken one side, positive values brighten the other
- this highlights intensity differences between neighboring pixels
step 3: 
- edges are enhanced with light/shadow contrast
- creates a 3D relief (embossed) appearance
*/
void embossEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		Mat gray, dst;

		cvtColor(src, gray, COLOR_BGR2GRAY);

		Mat kernel = (Mat_<float>(3, 3) <<
			-2, -1, 0,
			-1, 1, 1,
			0, 1, 2);

		filter2D(gray, dst, CV_8UC1, kernel);

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Emboss", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Emboss", dst);
		waitKey();
	}
}

/*
step 1: apply convolution using a sharpening kernel
step 2:
- the center value (5) keeps the original pixel strong
- the negative values (-1) subtract neighboring pixels
- this enhances differences between edges
step 3:
- edges become more pronounced
- the image appears clearer and sharper
*/
void sharpenEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		Mat dst;

		Mat kernel = (Mat_<float>(3, 3) <<
			0, -1, 0,
			-1, 5, -1,
			0, -1, 0);

		filter2D(src, dst, -1, kernel);

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Sharpen", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Sharpen", dst);
		waitKey();
	}
}

/*

step 1: define number of levels
- the color range is divided into a small number of intervals
step 2: reduce color values
- each pixel value is mapped to the nearest lower level to reduce the number of distinct colors
	val = (val / step) * step
step 3:
- smooth gradients are replaced with flat color regions
*/
void posterizationEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		int levels = 4;
		int step = 256 / levels;

		Mat dst = src.clone();

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				for (int c = 0; c < 3; c++)
				{
					int val = src.at<Vec3b>(i, j)[c];
					val = (val / step) * step;
					dst.at<Vec3b>(i, j)[c] = (uchar)val;
				}
			}
		}

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Posterized", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Posterized", dst);
		waitKey();
	}
}

/*
step 1: apply linear transformation to each pixel  val = (val - 128) * alpha + 128
step 2:
- alpha > 1 → increases contrast (brighter brights, darker darks)
- alpha < 1 → decreases contrast
*/
void contrastEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		int height = src.rows;
		int width = src.cols;

		Mat dst(height, width, CV_8UC3);

		double alpha = 1.5;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Vec3b pixel = src.at<Vec3b>(i, j);

				for (int c = 0; c < 3; c++)
				{
					int val = (int)((pixel[c] - 128) * alpha + 128);

					if (val < 0) val = 0;
					if (val > 255) val = 255;

					dst.at<Vec3b>(i, j)[c] = (uchar)val;
				}
			}
		}

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Contrast", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Contrast", dst);
		waitKey();
	}
}

/*
step 1: smooth the color image using bilateral filters
step 2: convert image to grayscale for contour extraction
step 3: apply median blur and Gaussian blur to reduce noise before creating the edge mask
step 4: use adaptive thresholding
- create black contours on a white background
step 5: combine the edge mask with the smoothed color image
- produces flat colors with visible outlines, similar to a cartoon
*/
void cartoonEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		Mat gray, color1, color2, edges, edgeMask, dst;

		bilateralFilter(src, color1, 9, 75, 75);
		bilateralFilter(color1, color2, 9, 75, 75);

		cvtColor(src, gray, COLOR_BGR2GRAY);

		medianBlur(gray, gray, 7);
		GaussianBlur(gray, gray, Size(5, 5), 1.0);

		adaptiveThreshold(
			gray,
			edges,
			255,
			ADAPTIVE_THRESH_MEAN_C,
			THRESH_BINARY,
			9,
			7
		);

		cvtColor(edges, edgeMask, COLOR_GRAY2BGR);

		bitwise_and(color2, edgeMask, dst);

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Cartoon", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Cartoon", dst);
		waitKey();
	}
}

/*
step 1: convert image to grayscale
step 2: invert the grayscale image
- dark areas become bright and bright areas become dark
step 3: apply Gaussian blur to the inverted image
step 4: invert the blurred image again
step 5: divide the grayscale image by the inverted blurred image
- enhances contours and produces a pencil drawing appearance
*/
void pencilSketchEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		Mat gray, invGray, blurImg, invBlur, dst;

		cvtColor(src, gray, COLOR_BGR2GRAY);
		bitwise_not(gray, invGray);
		GaussianBlur(invGray, blurImg, Size(21, 21), 0);
		bitwise_not(blurImg, invBlur);

		divide(gray, invBlur, dst, 256.0);

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Pencil Sketch", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Pencil Sketch", dst);
		waitKey();
	}
}

/*
step 1: for each pixel, analyze a neighborhood around it
- the radius controls the size of the local region
step 2: group neighboring pixels by intensity level
step 3: Find the most frequent intensity level
- this represents the dominant local tone
step 4: replace the current pixel with the average color of that group
- reduces fine details and creates larger color regions
result:
- the image obtains a painted appearance, similar to brush strokes
*/
void oilPaintingEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		int radius = 4;
		int levels = 16;

		Mat dst = src.clone();

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				int count[16] = { 0 };
				int sumB[16] = { 0 };
				int sumG[16] = { 0 };
				int sumR[16] = { 0 };

				for (int y = i - radius; y <= i + radius; y++)
				{
					for (int x = j - radius; x <= j + radius; x++)
					{
						if (y >= 0 && y < src.rows && x >= 0 && x < src.cols)
						{
							Vec3b p = src.at<Vec3b>(y, x);
							int intensity = (p[0] + p[1] + p[2]) / 3;
							int idx = intensity * levels / 256;
							if (idx >= levels) idx = levels - 1;

							count[idx]++;
							sumB[idx] += p[0];
							sumG[idx] += p[1];
							sumR[idx] += p[2];
						}
					}
				}

				int best = 0;
				for (int k = 1; k < levels; k++)
				{
					if (count[k] > count[best])
						best = k;
				}

				if (count[best] > 0)
				{
					dst.at<Vec3b>(i, j)[0] = sumB[best] / count[best];
					dst.at<Vec3b>(i, j)[1] = sumG[best] / count[best];
					dst.at<Vec3b>(i, j)[2] = sumR[best] / count[best];
				}
			}
		}

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Oil Painting", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Oil Painting", dst);
		waitKey();
	}
}

/*
step 1: compute the image center, it remains brighter
step 2: for each pixel, compute its distance from the center
- pixels farther from the center receive a smaller brightness factor
step 3: multiply each color channel by this factor
- borders become darker while the center remains visible
*/
void vignetteEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		Mat dst = src.clone();

		double cx = src.cols / 2.0;
		double cy = src.rows / 2.0;

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				double dx = (j - cx) / cx;
				double dy = (i - cy) / cy;
				double r2 = dx * dx + dy * dy;

				double factor = exp(-1.8 * r2);

				for (int c = 0; c < 3; c++)
				{
					int val = (int)(src.at<Vec3b>(i, j)[c] * factor);
					if (val < 0) val = 0;
					if (val > 255) val = 255;
					dst.at<Vec3b>(i, j)[c] = (uchar)val;
				}
			}
		}

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Vignette", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Vignette", dst);
		waitKey();
	}
}

/*
step 1: apply Gaussian blur to the image
step 2: blend the original image with the blurred image
- addWeighted combines both images using selected weights
*/
void glowEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		Mat blurImg, dst;
		GaussianBlur(src, blurImg, Size(0, 0), 10);

		addWeighted(src, 0.7, blurImg, 0.5, 0, dst);

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Glow", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Glow", dst);
		waitKey();
	}
}

/*
step 1: define a step value
- the color range  is divided into intervals
step 2: For each pixel and each color channel, map the value to its interval
- similar colors are grouped together
step 3: replace the original value with the representative interval value
- reduces the number of colors in the image
*/
void colorQuantizationEffect()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, IMREAD_COLOR);
		if (src.empty())
		{
			printf("Could not open image\n");
			continue;
		}

		Mat dst = src.clone();
		int step = 64;

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				for (int c = 0; c < 3; c++)
				{
					int val = src.at<Vec3b>(i, j)[c];
					val = (val / step) * step + step / 2;
					if (val > 255) val = 255;
					dst.at<Vec3b>(i, j)[c] = (uchar)val;
				}
			}
		}

		setupImageWindow("Original", 50, 50, 500, 400);
		setupImageWindow("Color Quantization", 600, 50, 500, 400);

		imshow("Original", src);
		imshow("Color Quantization", dst);
		waitKey();
	}
}

bool isWindowClosed(const char* winName)
{
	try
	{
		return getWindowProperty(winName, WND_PROP_VISIBLE) < 1;
	}
	catch (...)
	{
		return true;
	}
}

bool openVideoSource(VideoCapture& cap)
{
	int choice;
	printf("Video source:\n");
	printf("1 - Webcam\n");
	printf("2 - Video file\n");
	printf("Option: ");
	scanf("%d", &choice);

	if (choice == 1)
	{
		cap.open(0);
	}
	else if (choice == 2)
	{
		char fname[MAX_PATH];
		if (!openFileDlg(fname))
			return false;

		cap.open(fname);
	}
	else
	{
		return false;
	}

	if (!cap.isOpened())
	{
		printf("Cannot open video source.\n");
		return false;
	}

	return true;
}

/*
Video processing with image effects
step 1: open video source (webcam or file)
step 2: read frames in a loop
- each frame is treated as an image (Mat)
step 3: apply image processing effect
- the same algorithms used for images are applied to each frame
step 4: display processed frames
- imshow() shows frames continuously, creating video playback
step 5: control playback speed
- waitKey(30) ensures smooth visualization (~30 FPS)
*/

void grayscaleVideoEffect()
{
	VideoCapture cap;
	if (!openVideoSource(cap))
		return;

	Mat frame, gray;
	char c;

	namedWindow("Original Video", WINDOW_NORMAL);
	namedWindow("Grayscale Video", WINDOW_NORMAL);

	resizeWindow("Original Video", 640, 480);
	resizeWindow("Grayscale Video", 640, 480);

	moveWindow("Original Video", 50, 50);
	moveWindow("Grayscale Video", 720, 50);

	while (true)
	{
		if (!cap.read(frame) || frame.empty())
			break;

		cvtColor(frame, gray, COLOR_BGR2GRAY);

		if (isWindowClosed("Original Video") || isWindowClosed("Grayscale Video"))
			break;

		imshow("Original Video", frame);
		imshow("Grayscale Video", gray);

		c = waitKey(30);
		if (c == 27)
			break;

		if (isWindowClosed("Original Video") || isWindowClosed("Grayscale Video"))
			break;
	}
	destroyAllWindows();
}

void sepiaVideoEffect()
{
	VideoCapture cap;
	if (!openVideoSource(cap))
		return;

	Mat frame, dst;
	char c;

	namedWindow("Original Video", WINDOW_NORMAL);
	namedWindow("Sepia Video", WINDOW_NORMAL);

	resizeWindow("Original Video", 640, 480);
	resizeWindow("Sepia Video", 640, 480);

	moveWindow("Original Video", 50, 50);
	moveWindow("Sepia Video", 720, 50);

	while (true)
	{
		if (!cap.read(frame) || frame.empty())
			break;

		dst = frame.clone();

		for (int i = 0; i < frame.rows; i++)
		{
			for (int j = 0; j < frame.cols; j++)
			{
				Vec3b pixel = frame.at<Vec3b>(i, j);

				int B = pixel[0];
				int G = pixel[1];
				int R = pixel[2];

				int newR = (int)(0.393 * R + 0.769 * G + 0.189 * B);
				int newG = (int)(0.349 * R + 0.686 * G + 0.168 * B);
				int newB = (int)(0.272 * R + 0.534 * G + 0.131 * B);

				if (newR > 255) newR = 255;
				if (newG > 255) newG = 255;
				if (newB > 255) newB = 255;

				dst.at<Vec3b>(i, j)[0] = (uchar)newB;
				dst.at<Vec3b>(i, j)[1] = (uchar)newG;
				dst.at<Vec3b>(i, j)[2] = (uchar)newR;
			}
		}

		if (isWindowClosed("Original Video") || isWindowClosed("Sepia Video"))
			break;

		imshow("Original Video", frame);
		imshow("Sepia Video", dst);

		c = waitKey(30);
		if (c == 27)
			break;

		if (isWindowClosed("Original Video") || isWindowClosed("Sepia Video"))
			break;
	}
	destroyAllWindows();
}

void cartoonVideoEffect()
{
	VideoCapture cap;
	if (!openVideoSource(cap))
		return;

	Mat frame, gray, color1, color2, edges, edgeMask, dst;
	char c;

	namedWindow("Original Video", WINDOW_NORMAL);
	namedWindow("Cartoon Video", WINDOW_NORMAL);

	resizeWindow("Original Video", 640, 480);
	resizeWindow("Cartoon Video", 640, 480);

	moveWindow("Original Video", 50, 50);
	moveWindow("Cartoon Video", 720, 50);

	while (true)
	{
		if (!cap.read(frame) || frame.empty())
			break;

		bilateralFilter(frame, color1, 9, 75, 75);
		bilateralFilter(color1, color2, 9, 75, 75);

		cvtColor(frame, gray, COLOR_BGR2GRAY);
		medianBlur(gray, gray, 7);
		GaussianBlur(gray, gray, Size(5, 5), 1.0);

		adaptiveThreshold(
			gray,
			edges,
			255,
			ADAPTIVE_THRESH_MEAN_C,
			THRESH_BINARY,
			9,
			7
		);

		cvtColor(edges, edgeMask, COLOR_GRAY2BGR);
		bitwise_and(color2, edgeMask, dst);

		if (isWindowClosed("Original Video") || isWindowClosed("Cartoon Video"))
			break;

		imshow("Original Video", frame);
		imshow("Cartoon Video", dst);

		c = waitKey(30);
		if (c == 27)
			break;

		if (isWindowClosed("Original Video") || isWindowClosed("Cartoon Video"))
			break;
	}
	destroyAllWindows();
}

void inversionVideoEffect()
{
	VideoCapture cap;
	if (!openVideoSource(cap))
		return;

	Mat frame, dst;
	char c;

	namedWindow("Original Video", WINDOW_NORMAL);
	namedWindow("Inversion Video", WINDOW_NORMAL);

	resizeWindow("Original Video", 640, 480);
	resizeWindow("Inversion Video", 640, 480);

	moveWindow("Original Video", 50, 50);
	moveWindow("Inversion Video", 720, 50);

	while (true)
	{
		if (!cap.read(frame) || frame.empty())
			break;

		dst = frame.clone();

		for (int i = 0; i < frame.rows; i++)
		{
			for (int j = 0; j < frame.cols; j++)
			{
				dst.at<Vec3b>(i, j)[0] = 255 - frame.at<Vec3b>(i, j)[0];
				dst.at<Vec3b>(i, j)[1] = 255 - frame.at<Vec3b>(i, j)[1];
				dst.at<Vec3b>(i, j)[2] = 255 - frame.at<Vec3b>(i, j)[2];
			}
		}

		if (isWindowClosed("Original Video") || isWindowClosed("Inversion Video"))
			break;

		imshow("Original Video", frame);
		imshow("Inversion Video", dst);

		c = waitKey(30);
		if (c == 27)
			break;

		if (isWindowClosed("Original Video") || isWindowClosed("Inversion Video"))
			break;
	}

	destroyAllWindows();
}

void blurVideoEffect()
{
	VideoCapture cap;
	if (!openVideoSource(cap))
		return;

	Mat frame, dst;
	char c;

	namedWindow("Original Video", WINDOW_NORMAL);
	namedWindow("Blur Video", WINDOW_NORMAL);

	resizeWindow("Original Video", 640, 480);
	resizeWindow("Blur Video", 640, 480);

	moveWindow("Original Video", 50, 50);
	moveWindow("Blur Video", 720, 50);

	while (true)
	{
		if (!cap.read(frame) || frame.empty())
			break;

		blur(frame, dst, Size(7, 7));

		if (isWindowClosed("Original Video") || isWindowClosed("Blur Video"))
			break;

		imshow("Original Video", frame);
		imshow("Blur Video", dst);

		c = waitKey(30);
		if (c == 27)
			break;

		if (isWindowClosed("Original Video") || isWindowClosed("Blur Video"))
			break;
	}

	destroyAllWindows();
}

void pixelationVideoEffect()
{
	VideoCapture cap;
	if (!openVideoSource(cap))
		return;

	Mat frame, dst;
	char c;
	int blockSize = 10;

	namedWindow("Original Video", WINDOW_NORMAL);
	namedWindow("Pixelation Video", WINDOW_NORMAL);

	resizeWindow("Original Video", 640, 480);
	resizeWindow("Pixelation Video", 640, 480);

	moveWindow("Original Video", 50, 50);
	moveWindow("Pixelation Video", 720, 50);

	while (true)
	{
		if (!cap.read(frame) || frame.empty())
			break;

		dst = frame.clone();

		for (int i = 0; i < frame.rows; i += blockSize)
		{
			for (int j = 0; j < frame.cols; j += blockSize)
			{
				int sumB = 0, sumG = 0, sumR = 0, count = 0;

				for (int y = i; y < i + blockSize && y < frame.rows; y++)
				{
					for (int x = j; x < j + blockSize && x < frame.cols; x++)
					{
						Vec3b p = frame.at<Vec3b>(y, x);
						sumB += p[0];
						sumG += p[1];
						sumR += p[2];
						count++;
					}
				}

				int avgB = sumB / count;
				int avgG = sumG / count;
				int avgR = sumR / count;

				for (int y = i; y < i + blockSize && y < frame.rows; y++)
				{
					for (int x = j; x < j + blockSize && x < frame.cols; x++)
					{
						dst.at<Vec3b>(y, x) = Vec3b(avgB, avgG, avgR);
					}
				}
			}
		}

		if (isWindowClosed("Original Video") || isWindowClosed("Pixelation Video"))
			break;

		imshow("Original Video", frame);
		imshow("Pixelation Video", dst);

		c = waitKey(30);
		if (c == 27)
			break;

		if (isWindowClosed("Original Video") || isWindowClosed("Pixelation Video"))
			break;
	}

	destroyAllWindows();
}

void pencilSketchVideoEffect()
{
	VideoCapture cap;
	if (!openVideoSource(cap))
		return;

	Mat frame, gray, invGray, blurImg, invBlur, dst;
	char c;

	namedWindow("Original Video", WINDOW_NORMAL);
	namedWindow("Pencil Sketch Video", WINDOW_NORMAL);

	resizeWindow("Original Video", 640, 480);
	resizeWindow("Pencil Sketch Video", 640, 480);

	moveWindow("Original Video", 50, 50);
	moveWindow("Pencil Sketch Video", 720, 50);

	while (true)
	{
		if (!cap.read(frame) || frame.empty())
			break;

		cvtColor(frame, gray, COLOR_BGR2GRAY);
		bitwise_not(gray, invGray);
		GaussianBlur(invGray, blurImg, Size(21, 21), 0);
		bitwise_not(blurImg, invBlur);
		divide(gray, invBlur, dst, 256.0);

		if (isWindowClosed("Original Video") || isWindowClosed("Pencil Sketch Video"))
			break;

		imshow("Original Video", frame);
		imshow("Pencil Sketch Video", dst);

		c = waitKey(30);
		if (c == 27)
			break;

		if (isWindowClosed("Original Video") || isWindowClosed("Pencil Sketch Video"))
			break;
	}

	destroyAllWindows();
}

int main()
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_FATAL);
	projectPath = _wgetcwd(0, 0);

	int op;
	do
	{
		system("cls");
		destroyAllWindows();

		printf("Menu:\n");
		printf(" 1 - Grayscale conversion\n");
		printf(" 2 - Blur effect\n");
		printf(" 3 - Edge detection\n");
		printf(" 4 - Sepia effect\n");
		printf(" 5 - Color inversion\n");
		printf(" 6 - Pixelation effect\n");
		printf(" 7 - Emboss effect\n");
		printf(" 8 - Sharpen effect\n");
		printf(" 9 - Posterization effect\n");
		printf("10 - Contrast adjustment\n");
		printf("11 - Cartoon effect\n");
		printf("12 - Pencil sketch effect\n");
		printf("13 - Oil painting effect\n");
		printf("14 - Vignette effect\n");
		printf("15 - Glow / soft light effect\n");
		printf("16 - Color quantization effect\n");
		printf("17 - Grayscale video\n");
		printf("18 - Sepia video\n");
		printf("19 - Cartoon video\n");
		printf("20 - Inversion video\n");
		printf("21 - Blur video\n");
		printf("22 - Pixelation video\n");
		printf("23 - Pencil sketch video\n");
		printf(" 0 - Exit\n\n");

		printf("Option: ");
		scanf("%d", &op);

		switch (op)
		{
		case 1:
			grayscaleEffect();
			break;
		case 2:
			blurEffect();
			break;
		case 3:
			edgeDetectionEffect();
			break;
		case 4:
			sepiaEffect();
			break;
		case 5:
			inversionEffect();
			break;
		case 6:
			pixelationEffect();
			break;
		case 7:
			embossEffect();
			break;
		case 8:
			sharpenEffect();
			break;
		case 9:
			posterizationEffect();
			break;
		case 10:
			contrastEffect();
			break;
		case 11:
			cartoonEffect();
			break;
		case 12:
			pencilSketchEffect();
			break;
		case 13:
			oilPaintingEffect();
			break;
		case 14:
			vignetteEffect();
			break;
		case 15:
			glowEffect();
			break;
		case 16:
			colorQuantizationEffect();
			break;
		case 17:
			grayscaleVideoEffect();
			break;
		case 18:
			sepiaVideoEffect();
			break;
		case 19:
			cartoonVideoEffect();
			break;
		case 20:
			inversionVideoEffect();
			break;
		case 21:
			blurVideoEffect();
			break;
		case 22:
			pixelationVideoEffect();
			break;
		case 23:
			pencilSketchVideoEffect();
			break;
		}
	} while (op != 0);

	return 0;
}
