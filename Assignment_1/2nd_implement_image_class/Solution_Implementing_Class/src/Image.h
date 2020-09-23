#pragma once
#include<iostream>
#include<string>
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
		image = load(fileName); 
	}
	~Image() {
		cout << "Image class destructing by reset()..." << endl;
		reset();//free dynamic memory
	}

	PPM* image;

	PPM* load(const std::string& fileName);
	void save(const std::string& out_put_fileName);
	void reset();
	void clear();
	bool empty();

	string type();
	unsigned int row();
	unsigned int column();
	unsigned int size();

	Image& gamma_correction(const float gamma);
	Image& alpha_compositing(Image& back, const float alpha);
	Image& erosion();//by simple 2*2 structure full 1;
	bool check1(Image* img, unsigned int rows, unsigned int cols);
	void set0(Image* img, unsigned int rows, unsigned int cols);

	Image& operator+ (Image& img);
	Image& operator+= (Image& img);
	Image& operator- (Image& img);
	Image& operator* (Image& img);
	
	

	//Image(const Image& _copy) :mRows(_copy.mRows), mColumns(_copy.mColumns),msize(_copy.msize),mMaxValue(_copy.mMaxValue),channel(_copy.channel) {
	//	for (int i = 0; i < 3; i++)mType[i] = _copy.mType[i];
	//	mPixels = new Pixel[msize];
	//	cout << "copy: " << msize << endl;
	//	for (int i = 0; i < msize; i++) {
	//		Pixel pixel = new unsigned char[channel];
	//		mPixels[i] = pixel;
	//		for (int j = 0; j < channel; j++) {
	//			mPixels[i][j] = _copy.mPixels[i][j];
	//			
	//		}
	//	}
	//}
 
};

