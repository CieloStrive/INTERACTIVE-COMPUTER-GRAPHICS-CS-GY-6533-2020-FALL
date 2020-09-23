#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<memory>
using namespace std;


typedef unsigned char* Pixel;

//PPM file structure type
struct PPM {
	Pixel* mPixels = nullptr;
	unsigned int mRows = 0;
	unsigned int mColumns = 0;
	unsigned int msize = 0;
	char mType[3];
	unsigned int mMaxValue = 0;
	unsigned int channel = 3;
};

class Image{
public:
	Image() {
		cout << "Image class constructing by default..." << endl;
	}
	Image(string fileName) { 
		cout << "Image class constructing by load()..." << endl;
		this->image = load(fileName); 
	}
	~Image() {
		cout << "Image class destructing by reset()..." << endl;
		reset();//free dynamic memory
	}

	PPM* image = nullptr;

	PPM* load(const std::string& fileName);
	void save(const std::string& out_put_fileName);
	void reset();//Usually used in destructor or in load to free dynamic memory of old image inside the class, reset the instance.
	void clear();//clear picture to "0".
	bool empty();//check if there is image data of dynamic memory inside the instance

	string type();
	unsigned int row();
	unsigned int column();
	unsigned int size();

	Image& gamma_correction(const float gamma);
	Image& alpha_compositing(Image& back, const float alpha);
	//Image& erosion();//by simple 2*2 structure full 1;
	bool check1(Image* img, unsigned int rows, unsigned int cols);
	void set0(Image* img, unsigned int rows, unsigned int cols);

	vector<unsigned int> operator[] (int p_index);
	Image& operator+ (Image& img);
	Image& operator+= (Image& img);
	Image& operator- (Image& img);
	Image& operator* (Image& img);
	Image& operator* (const float scalar);
 
};

