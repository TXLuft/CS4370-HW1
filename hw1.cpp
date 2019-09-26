/*******************************************************
 * Homework 1: Rasterization                           *
 * CS 148 (Summer 2016), Stanford University           *
 *-----------------------------------------------------*
 * Here you will implement the circle rasterization    *
 * method you derived in the written portion of the    *
 * homework.                                           *
 * To compile this in linux:                           *
 *        g++ hw1.cpp                                  *
 * Then, run the program as follows:                   *
 *        ./a.out 200                                  *
 * to generate a 200x200 image containing a circular   *
 * arc.  Note that the coordinate system we're using   *
 * places pixel centers at integer coordinates and     *
 * has the origin at the lower left.                   *
 * Your code will generate a .ppm file containing the  *
 * final image. These images can be viewed using       *
 * "display" in Linux or Irfanview in Mac/Windows.     *
 *******************************************************/

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>
using namespace std;

// We'll store image info as globals; not great programming practice
// but ok for this short program.
int size;
bool **image;

void renderPixel(int x, int y) {
	assert(x >= 0 && y >= 0 && x <= size && y <= size);
	image[x][y] = 1;
	
	// Plot symmetrical point if y != x
	if(x != y)
    image[y][x] = 1;
}


// Midpoint Algorithm
int midPoint(int x, int y, int r)
{
  // d = x² + y² - R²
  // +: outside
  // -: inside
  return (x * x) + (y * y) - (r * r);
}


void rasterizeArc(int radius) {
	// TODO:  rasterize the arc using renderPixel to light up pixels
  
  // Sqrt(2)
  float root2 = 0.707106781;
  
  // P0
  int x0 = 0;
  int y0 = radius;
  
  // P45
  int xf = int(radius * root2 + 0.5f);
  
  // Iteratively Draw 1/8th Circle
  for(int xi = x0, yi = y0; xi <= xf; xi++)
  {
    // Calculate d
    int d = midPoint(xi, yi - 1, radius);
    
    // If sloping down
    if(d > 0)
      yi--;
    
    // Render Point
    renderPixel(xi, yi);
  }
}

// You shouldn't need to change anything below this point.

int main(int argc, char *argv[]) {
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " circleSize\n";
		return 0;
	}
	
#ifdef _WIN32
	sscanf_s(argv[1], "%d", &size);
#else
	sscanf(argv[1], "%d", &size);
#endif
	if (size <= 0) {
		cout << "Image must be of positive size.\n";
		return 0;
	}
	
	// reserve image as 2d array
	image = new bool*[size+1];
	for (int i = 0; i <= size; i++) image[i] = new bool[size+1];
	
	rasterizeArc(size);
	
	char filename[50];
#ifdef _WIN32
	sprintf_s(filename, 50, "circle%d.ppm", size);
#else
	sprintf(filename, "circle%d.ppm", size);
#endif
	
	ofstream outfile(filename);
	outfile << "P3\n# " << filename << "\n";
	outfile << size+1 << ' ' << size+1 << ' ' << 1 << endl;

	for (int i = 0; i <= size; i++)
	for (int j = 0; j <= size; j++)	
		outfile << image[size-i][j] << " 0 0\n";
	
	// delete image data
	for (int i = 0; i <= size; i++) delete [] image[i];
	delete [] image;
	
	return 0;
}
