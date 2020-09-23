#include "Image.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

PPM* Image::load(const std::string& fileName) {
	std::ifstream imageFile;
	imageFile.open(fileName, ios::in | ios::binary);

	if (!imageFile) {
		std::cout << "Error opening image file " << fileName << std::endl;
		return nullptr;
	}

	PPM* ppmImage = new PPM;
	//directly read ascii for PPM file header info
	imageFile >> ppmImage->mType;
	imageFile >> ppmImage->mColumns;
	imageFile >> ppmImage->mRows;
	ppmImage->msize = ppmImage->mColumns * ppmImage->mRows;
	imageFile >> ppmImage->mMaxValue;

	if (ppmImage->mPixels != nullptr) {
		delete[] ppmImage->mPixels;
	}

	//decide channel by PPM type

	if (ppmImage->mType[1] == '3' || ppmImage->mType[1] == '6')ppmImage->channel = 3;
	if (ppmImage->mType[1] == '2' || ppmImage->mType[1] == '5')ppmImage->channel = 1;

	cout << "Type:" << ppmImage->mType[0] << ppmImage->mType[1] << endl;
	cout << "col:" << ppmImage->mColumns << endl;
	cout << "row:" << ppmImage->mRows << endl;
	cout << "size:" << ppmImage->msize << endl;
	cout << "mMaxValue:" << ppmImage->mMaxValue << endl;
	cout << "Channel:" << ppmImage->channel << endl;

	ppmImage->mPixels = new Pixel[ppmImage->msize];

	//auto i = 0;
	switch (ppmImage->mType[1]) {
	case '3': //read P3: ascii plain character data
		ppmImage->channel = 3;
		for (auto i = 0; i < ppmImage->msize; ++i) {
			Pixel pixel = new unsigned char[ppmImage->channel];
			ppmImage->mPixels[i] = pixel;
			unsigned int r, g, b;//read ascii file can't use unsigned char at first
			imageFile >> r;
			imageFile >> g;
			imageFile >> b;
			ppmImage->mPixels[i][0] = static_cast<unsigned char>(r);
			ppmImage->mPixels[i][1] = static_cast<unsigned char>(g);
			ppmImage->mPixels[i][2] = static_cast<unsigned char>(b);
		}
		break;

	case '6': //read P6: binary data part
		ppmImage->channel = 3;
		for (auto i = 0; i < ppmImage->msize; ++i) {
			Pixel pixel = new unsigned char[ppmImage->channel];
			ppmImage->mPixels[i] = pixel;
			unsigned char r, g, b;
			imageFile.read((char*)&r, sizeof(unsigned char));
			imageFile.read((char*)&g, sizeof(unsigned char));
			imageFile.read((char*)&b, sizeof(unsigned char));
			ppmImage->mPixels[i][0] = r;
			ppmImage->mPixels[i][1] = g;
			ppmImage->mPixels[i][2] = b;
		}
		break;

	case '5': //read P6: binary data part
		ppmImage->channel = 1;
		for (auto i = 0; i < ppmImage->msize; ++i) {
			Pixel pixel = new unsigned char[ppmImage->channel];
			ppmImage->mPixels[i] = pixel;
			unsigned char grey;
			imageFile.read((char*)&grey, sizeof(unsigned char));
			ppmImage->mPixels[i][0] = grey;
		}
		break;

	case '2': //read P3: ascii plain character data
		ppmImage->channel = 1;
		for (auto i = 0; i < ppmImage->msize; ++i) {
			Pixel pixel = new unsigned char[ppmImage->channel];
			ppmImage->mPixels[i] = pixel;
			unsigned int grey;//read ascii file can't use unsigned char at first
			imageFile >> grey;
			ppmImage->mPixels[i][0] = static_cast<unsigned char>(grey);
		}
		break;

	}


	imageFile.close();

	cout << "finish loading " << fileName << endl;
	return ppmImage;


}


void Image::save(const std::string& out_put_fileName) {
	if (Image::empty()) {
		cout << "Empty, can't save!" << endl;
		return;
	}
	Image&  ppmFile = *this;
	std::cout << "Saving file type: " << ppmFile.image->mType[0] << ppmFile.image->mType[1] << " ..." << std::endl;
	std::ofstream savePPMFile(out_put_fileName, ios::binary);
	if (!savePPMFile) {
		std::cout << "Error opening image " << out_put_fileName << " to save." << std::endl;
		return;
	}
	else {
		savePPMFile << ppmFile.image->mType << std::endl;
		savePPMFile << ppmFile.image->mColumns << " " << ppmFile.image->mRows << std::endl;
		savePPMFile << ppmFile.image->mMaxValue << std::endl;

		switch (ppmFile.image->mType[1]) {
		case '3':
			for (auto i = 0; i < ppmFile.image->msize; i++) {
				savePPMFile << static_cast<unsigned int>(ppmFile.image->mPixels[i][0]) << std::endl
					<< static_cast<unsigned int>(ppmFile.image->mPixels[i][1]) << std::endl
					<< static_cast<unsigned int>(ppmFile.image->mPixels[i][2]) << std::endl;
			}
			break;
		case '6':
			for (auto i = 0; i < ppmFile.image->msize; i++) {
				savePPMFile.write((char*)&ppmFile.image->mPixels[i][0], sizeof(unsigned char));
				savePPMFile.write((char*)&ppmFile.image->mPixels[i][1], sizeof(unsigned char));
				savePPMFile.write((char*)&ppmFile.image->mPixels[i][2], sizeof(unsigned char));
			}
			break;
		case '2':
			for (auto i = 0; i < ppmFile.image->msize; i++) {
				savePPMFile << static_cast<unsigned int>(ppmFile.image->mPixels[i][0]) << std::endl;
			}
			break;
		case '5':
			for (auto i = 0; i < ppmFile.image->msize; i++) {
				savePPMFile.write((char*)&ppmFile.image->mPixels[i][0], sizeof(unsigned char));
			}
			break;
		}



		savePPMFile.close();
	}
	cout << "Finish saving " << out_put_fileName << endl;
}

void Image::reset() { //reset the class
	if (Image::empty()) {
		cout << "reset(): the class is already empty (dynamic memory freed)." << endl;
		return;
	}

	for (auto i = 0; i < this->image->msize; ++i){
 		delete[] this->image->mPixels[i];
		this->image->mPixels[i] = nullptr;
	}
	delete[] this->image->mPixels;
	this->image->mPixels = nullptr;
	delete this->image;
	cout << "reset(): the class is reset now." << endl;
}

void Image::clear() { //clear image to white
	if (Image::empty()) {
		cout << "clear(): the class is empty. Can't clear image to white." << endl;
		return;
	}
for (auto i = 0; i < this->image->msize; ++i) {
		for (auto j = 0; j < this->image->channel; ++j) {
			this->image->mPixels[i][j] = static_cast<unsigned char>(0);
		}
	}
//cout <<"pixel 2 channel 1: ";
	//cout << (unsigned int)this->mPixels[2][1] << endl;
	//cout << "pixel size-1 channel 0: ";
	//cout << (unsigned int)this->mPixels[this->msize-1][0] << endl;
	//cout << "size: " << this->msize << endl;
	cout << "clear(): the image is white now." << endl;
}


bool Image::empty() {
	if (this->image->mPixels == nullptr) {
		//cout << "empty(): img has no data" << endl;
		return true;
	}
	else {
		//cout << "empty(): img is not empty" << endl;
		return false;
	}
}


string Image::type() {
	if (empty())return "Empty, no data loaded";
	string typ;
	typ += this->image->mType[0];
	typ += this->image->mType[1];
	return typ;
}
unsigned int Image::row() {
	if (empty())return 0;
	return this->image->mRows;
}
unsigned int Image::column() {
	if (empty())return 0;
	return this->image->mColumns;
}

unsigned int Image::size() {
	if (empty())return 0;
	return this->image->msize;
}


Image& Image::gamma_correction(const float gamma) {
	cout << "Gamma Correcting with " << gamma << "..." << endl;
	Image* res = new Image();
	res->image = new PPM;
	res->image->mRows = this->image->mRows;
	res->image->mColumns = this->image->mColumns;
	res->image->msize = this->image->msize;
	for (int i = 0; i < 3; i++)res->image->mType[i] = this->image->mType[i];
	res->image->mMaxValue = this->image->mMaxValue;
	res->image->channel = this->image->channel;

	res->image->mPixels = new Pixel[res->image->msize];
	for (int i = 0; i < res->image->msize; i++) {
		Pixel pixel = new unsigned char[this->image->channel];
		res->image->mPixels[i] = pixel;
		for (int j = 0; j < res->image->channel; j++) {
			res->image->mPixels[i][j] = static_cast<unsigned char>(
				static_cast<float>(this->image->mMaxValue)*
				pow((static_cast<float>(this->image->mPixels[i][j]) / static_cast<float>(this->image->mMaxValue)), gamma));
		}
	}

	return *res;
}

Image& Image::alpha_compositing(Image& back, const float alpha) {
	cout << "alpha compositing with " << alpha << "..." << endl;
	bool match = true;
	if (this->image->mRows != back.image->mRows ||
		this->image->mColumns != back.image->mColumns ||
		this->image->channel != back.image->channel ||
		this->image->mMaxValue != back.image->mMaxValue)match = false;


	if (!match)cout << "can't do alpha compositing between two img of different size/#channel" << endl;

	Image* res = new Image();
	res->image = new PPM;
	res->image->mRows = this->image->mRows;
	res->image->mColumns = this->image->mColumns;
	res->image->msize = this->image->msize;
	for (int i = 0; i < 3; i++)res->image->mType[i] = this->image->mType[i];
	res->image->mMaxValue = this->image->mMaxValue;
	res->image->channel = this->image->channel;

	res->image->mPixels = new Pixel[res->image->msize];
	for (int i = 0; i < res->image->msize; i++) {
		Pixel pixel = new unsigned char[this->image->channel];
		res->image->mPixels[i] = pixel;
		for (int j = 0; j < res->image->channel; j++) {
			if (match)res->image->mPixels[i][j] = static_cast<unsigned char>(
				alpha*static_cast<float>(this->image->mPixels[i][j]) + 
				(1.0-alpha) * static_cast<float>(back.image->mPixels[i][j]) );
			else res->image->mPixels[i][j] = 0;
		}
	}

	return *res;
}

bool Image::check1(Image* img, unsigned int rows, unsigned int cols) {
	bool res = false;
	int index = img->image->mColumns * rows + cols;
	for (int i = 0; i < img->image->channel; i++)res |= (img->image->mPixels[index][i] != 0);
	return res;
}

void Image::set0 (Image* img, unsigned int rows, unsigned int cols) {
	int index = img->image->mColumns * rows + cols;
	for (int i = 0; i < img->image->channel; i++)img->image->mPixels[index][i] = 0;
}

Image& Image::erosion() {
	cout << "performing erosion..." << endl;
	Image* res = new Image();
	res->image = new PPM;
	res->image->mRows = this->image->mRows;
	res->image->mColumns = this->image->mColumns;
	res->image->msize = this->image->msize;
	for (int i = 0; i < 3; i++)res->image->mType[i] = this->image->mType[i];
	res->image->mMaxValue = this->image->mMaxValue;
	res->image->channel = this->image->channel;

	res->image->mPixels = new Pixel[res->image->msize];
	for (int i = 0; i < res->image->msize; i++) {
		Pixel pixel = new unsigned char[this->image->channel];
		res->image->mPixels[i] = pixel;
		for (int j = 0; j < res->image->channel; j++) {
			res->image->mPixels[i][j] = this->image->mPixels[i][j];
		}
	}

	int rows = this->image->mRows;
	int cols = this->image->mColumns;

	

	for (int i = 0; i < rows-1; i++) {
		for (int j = 0; j < cols-1; j++) {
			if (check1(res, i, j) && check1(res, i + 1, j) && check1(res, i, j + 1) && check1(res, i + 1, j + 1)) {
				continue;
			}
			else {
				set0(res, i, j);
				set0(res, i+1, j);
				set0(res, i, j+1);
				set0(res, i+1, j+1);
			}
		}
	}

	return *res;
}

Image& Image::operator+= (Image& imge) {
	cout << "performing += ..." << endl;
	Image* img = &imge;
	bool match = true;
	if (this->image->mRows != img->image->mRows ||
		this->image->mColumns != img->image->mColumns ||
		this->image->channel != img->image->channel ||
		this->image->mMaxValue != img->image->mMaxValue)match = false;
	
	if (!match)cout << "can't sum two img of different size/#channel" << endl;

	for (int i = 0; i < this->image->msize; i++) {
		for (int j = 0; j < this->image->channel; j++) {
			if (match)this->image->mPixels[i][j] = (this->image->mPixels[i][j] + img->image->mPixels[i][j]) / 2;
			else this->image->mPixels[i][j] = 0;
		}
	}

	return *this;
}

Image& Image::operator+ (Image& imge) {
	cout << "performing addition..." << endl;
	Image* img = &imge;
	bool match = true;
	if (this->image->mRows != img->image->mRows ||
		this->image->mColumns != img->image->mColumns ||
		this->image->channel != img->image->channel ||
		this->image->mMaxValue != img->image->mMaxValue)match = false;


	if (!match)cout << "can't sum two img of different size/#channel" << endl;

	Image* sum = new Image();
	sum->image = new PPM;
	sum->image->mRows = this->image->mRows;
	sum->image->mColumns = this->image->mColumns;
	sum->image->msize = this->image->msize;
	for (int i = 0; i < 3; i++)sum->image->mType[i] = this->image->mType[i];
	sum->image->mMaxValue = this->image->mMaxValue;
	sum->image->channel = this->image->channel;

	sum->image->mPixels = new Pixel[sum->image->msize];
	for (int i = 0; i < sum->image->msize; i++) {
		Pixel pixel = new unsigned char[this->image->channel];
		sum->image->mPixels[i] = pixel;
		for (int j = 0; j < sum->image->channel; j++) {
			if (match)sum->image->mPixels[i][j] = this->image->mPixels[i][j] + img->image->mPixels[i][j];
			else sum->image->mPixels[i][j] = 0;
		}
	}

	return *sum;
}


Image& Image::operator- (Image& imge) {
	cout << "performing subtraction..." << endl;
	Image* img = &imge;
	bool match = true;
	if (this->image->mRows != img->image->mRows ||
		this->image->mColumns != img->image->mColumns ||
		this->image->channel != img->image->channel ||
		this->image->mMaxValue != img->image->mMaxValue)match = false;


	if (!match)cout << "can't subtract between two img of different size/#channel" << endl;

	Image* res = new Image();
	res->image = new PPM;
	res->image->mRows = this->image->mRows;
	res->image->mColumns = this->image->mColumns;
	res->image->msize = this->image->msize;
	for (int i = 0; i < 3; i++)res->image->mType[i] = this->image->mType[i];
	res->image->mMaxValue = this->image->mMaxValue;
	res->image->channel = this->image->channel;

	res->image->mPixels = new Pixel[res->image->msize];
	for (int i = 0; i < res->image->msize; i++) {
		Pixel pixel = new unsigned char[this->image->channel];
		res->image->mPixels[i] = pixel;
		for (int j = 0; j < res->image->channel; j++) {
			if (match)res->image->mPixels[i][j] = this->image->mPixels[i][j] <= img->image->mPixels[i][j] ? (unsigned char)0 : this->image->mPixels[i][j] - img->image->mPixels[i][j];
			else res->image->mPixels[i][j] = 0;
		}
	}

	return *res;
}


Image& Image::operator* (Image& imge) {
	cout << "performing subtraction..." << endl;
	Image* img = &imge;
	bool match = true;
	if (this->image->mRows != img->image->mRows ||
		this->image->mColumns != img->image->mColumns ||
		this->image->channel != img->image->channel ||
		this->image->mMaxValue != img->image->mMaxValue)match = false;


	if (!match)cout << "can't do multiplication between two img of different size/#channel" << endl;

	Image* res = new Image();
	res->image = new PPM;
	res->image->mRows = this->image->mRows;
	res->image->mColumns = this->image->mColumns;
	res->image->msize = this->image->msize;
	for (int i = 0; i < 3; i++)res->image->mType[i] = this->image->mType[i];
	res->image->mMaxValue = this->image->mMaxValue;
	res->image->channel = this->image->channel;

	res->image->mPixels = new Pixel[res->image->msize];
	for (int i = 0; i < res->image->msize; i++) {
		Pixel pixel = new unsigned char[this->image->channel];
		res->image->mPixels[i] = pixel;
		for (int j = 0; j < res->image->channel; j++) {
			if (match)res->image->mPixels[i][j] = this->image->mPixels[i][j] * img->image->mPixels[i][j];
			else res->image->mPixels[i][j] = 0;
		}
	}

	return *res;
}


//Image& Image::operator= (const Image& _copy) {
//	image = new PPM;
//
//	image->mRows = _copy.image->mRows;
//	image->mColumns = _copy.image->mColumns;
//	image->msize = _copy.image->msize;
//	image->mMaxValue = _copy.image->mMaxValue;
//	image->channel = _copy.image->channel;
//	cout << "operator=: " << image->msize << endl;
//	for (int i = 0; i < 3; i++)image->mType[i] = _copy.image->mType[i];
//
//	image->mPixels = new Pixel[image->msize];
//	cout << "copy: " << image->msize << endl;
//	for (int i = 0; i < image->msize; i++) {
//		Pixel pixel = new unsigned char[image->channel];
//		image->mPixels[i] = pixel;
//		for (int j = 0; j < image->channel; j++) {
//			image->mPixels[i][j] = _copy.image->mPixels[i][j];
//
//		}
//	}
//	return *this;
//}
