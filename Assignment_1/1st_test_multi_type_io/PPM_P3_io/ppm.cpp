// Copyright 2020 Jonathas Costa and Claudio Silva. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

// Definition of a pixel type
typedef struct {
    unsigned char rgb[3];
} Pixel;

// PPM file structure type
struct PPM {
    Pixel * mPixels = nullptr;
    unsigned int mRows;
    unsigned int mColumns;
    unsigned int size;
    char mType[3];
    unsigned int mMaxValue;
};

// Reads a PPM file from the disk.
// Works only for P3 Type 
PPM * loadPPM(const std::string & fileName) {
    std::ifstream imageFile;
    imageFile.open(fileName, std::fstream::in);

    if (!imageFile) {
        std::cout << "Error opening image file " << fileName << std::endl;
        return nullptr;
    }

    PPM * ppmImage = new PPM;

    imageFile >> ppmImage->mType;
    imageFile >> ppmImage->mColumns;
    imageFile >> ppmImage->mRows;
    ppmImage->size = ppmImage->mColumns * ppmImage->mRows;
    imageFile >> ppmImage->mMaxValue;

    if (ppmImage->mPixels != nullptr) {
        delete ppmImage->mPixels;
    }
    
    ppmImage->mPixels = new Pixel[ppmImage->size];

    auto i = 0;
    while (i < (ppmImage->size - 1)) {
        unsigned int r, g, b;
        imageFile >> r;
        imageFile >> g;
        imageFile >> b;
        ppmImage->mPixels[i].rgb[0] = static_cast<unsigned char>(r);
        ppmImage->mPixels[i].rgb[1] = static_cast<unsigned char>(g);
        ppmImage->mPixels[i].rgb[2] = static_cast<unsigned char>(b);
        ++i;
    }
    
    imageFile.close();

    return ppmImage;
}

// Saves a PPM file.
void savePPM(const std::string & fileName, const PPM & ppmFile) {
    std::cout << "Saving file type P3..." << std::endl;
    std::ofstream savePPMFile(fileName, std::ofstream::trunc);
    if (!savePPMFile) {
        std::cout << "Error opening image " << fileName << " to save." << std::endl;
        return;
    } else {
        savePPMFile << ppmFile.mType <<  std::endl;
        savePPMFile << ppmFile.mColumns << " " << ppmFile.mRows << std::endl;
        savePPMFile << ppmFile.mMaxValue << std::endl;
        for (auto i = 0; i < ppmFile.size; ++i) {
        	savePPMFile << static_cast<unsigned int>(ppmFile.mPixels[i].rgb[0]) << std::endl
                        << static_cast<unsigned int>(ppmFile.mPixels[i].rgb[1]) << std::endl
                        << static_cast<unsigned int>(ppmFile.mPixels[i].rgb[2]) << std::endl;
        }
        savePPMFile.close();
    }
}

// Student:
// You must implement the following code to apply a gamma correction procedure
// given the a gamma value and a PPM image file.

void gamma_corrected(const float gamma, PPM & ppmFile) {
	for (auto i = 0; i < ppmFile.size; ++i) {
            ppmFile.mPixels[i].rgb[0] = static_cast<unsigned char>(
            	static_cast<float>(ppmFile.mMaxValue) * 
            	pow(((float)ppmFile.mPixels[i].rgb[0]/static_cast<float>(ppmFile.mMaxValue)),gamma) );
            ppmFile.mPixels[i].rgb[1] = static_cast<unsigned char>(
            	static_cast<float>(ppmFile.mMaxValue) * 
            	pow(((float)ppmFile.mPixels[i].rgb[1]/static_cast<float>(ppmFile.mMaxValue)),gamma) );
            ppmFile.mPixels[i].rgb[2] = static_cast<unsigned char>(
            	static_cast<float>(ppmFile.mMaxValue) * 
            	pow(((float)ppmFile.mPixels[i].rgb[2]/static_cast<float>(ppmFile.mMaxValue)),gamma) );
            
        }
}

int main(int argc, char * argv[]) {

    // Load Original Image
    PPM *lenaImage = loadPPM(std::string("lena_gray.ppm"));

    // Apply gamma correction
    gamma_corrected(4.f, *lenaImage);
    // Save results to disk    
    savePPM(std::string("gamma_corrected4.ppm"), *lenaImage);

    // Not linear inverse
    gamma_corrected(0.25f, *lenaImage);
    savePPM(std::string("gamma_corrected25.ppm"), *lenaImage);

    // Other pass
    gamma_corrected(0.25f, *lenaImage);
    savePPM(std::string("gamma_corrected25x2.ppm"), *lenaImage);

    return 0;
}
