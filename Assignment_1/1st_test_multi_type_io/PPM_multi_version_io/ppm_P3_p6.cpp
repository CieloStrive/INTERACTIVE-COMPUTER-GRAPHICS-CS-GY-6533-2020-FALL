// Copyright 2020 Jonathas Costa and Claudio Silva. All rights reserved.
// Modified from original version to enable process for P6 PPM by Haotian Yi
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
// Try P6 file 
PPM * loadPPM(const std::string & fileName) {
    std::ifstream imageFile;
    imageFile.open(fileName, ios::in | ios::binary);

    if (!imageFile) {
        std::cout << "Error opening image file " << fileName << std::endl;
        return nullptr;
    }

    PPM * ppmImage = new PPM;
    //directily read ascii for PPM file header info
    imageFile >> ppmImage->mType; cout<<ppmImage->mType[0]<<ppmImage->mType[1]<<endl;
    imageFile >> ppmImage->mColumns;   cout<<"col:"<<ppmImage->mColumns<<endl;
    imageFile >> ppmImage->mRows;      cout<<"row:"<<ppmImage->mRows<<endl;
    ppmImage->size = ppmImage->mColumns * ppmImage->mRows;    cout<<"size:"<<ppmImage->size<<endl;
    imageFile >> ppmImage->mMaxValue;	cout<<"mMaxValue:"<<ppmImage->mMaxValue<<endl;

    if (ppmImage->mPixels != nullptr) {
        delete ppmImage->mPixels;
    }
    
    ppmImage->mPixels = new Pixel[ppmImage->size];
    auto i = 0;
    switch(ppmImage->mType[1]){
    	case '3': //read P3: ascii plain character data
    		while (i < (ppmImage->size - 1)) {
		        unsigned int r, g, b;//read ascii file can't use unsigned char at first
		        imageFile >> r;
		        imageFile >> g;
		        imageFile >> b;
		        ppmImage->mPixels[i].rgb[0] = static_cast<unsigned char>(r);
		        ppmImage->mPixels[i].rgb[1] = static_cast<unsigned char>(g);
		        ppmImage->mPixels[i].rgb[2] = static_cast<unsigned char>(b);
		        ++i;
		    }
    		break;
        case '6': //read P6: binary data part
        	while (i < (ppmImage->size - 1)) {
		        unsigned char r, g, b;
		        imageFile.read((char*)&r, sizeof(unsigned char));
		        imageFile.read((char*)&g, sizeof(unsigned char));
		        imageFile.read((char*)&b, sizeof(unsigned char));
		        ppmImage->mPixels[i].rgb[0] = r;
		        ppmImage->mPixels[i].rgb[1] = g;
		        ppmImage->mPixels[i].rgb[2] = b;
		        ++i;
		    }
		    break;

    }

    
    imageFile.close();

    return ppmImage;
}

// Saves a P6 PPM file.
void savePPM(const std::string & fileName, const PPM & ppmFile) {
    std::cout << "Saving file type "<<ppmFile.mType[0]<<ppmFile.mType[1]<< " ..." << std::endl;
    std::ofstream savePPMFile(fileName, ios::binary);
    if (!savePPMFile) {
        std::cout << "Error opening image " << fileName << " to save." << std::endl;
        return;
    } else {
        savePPMFile << ppmFile.mType <<  std::endl;
        savePPMFile << ppmFile.mColumns << " " << ppmFile.mRows << std::endl;
        savePPMFile << ppmFile.mMaxValue << std::endl;

        switch(ppmFile.mType[1]){
        	case '3':
        		for (auto i = 0; i < ppmFile.size; ++i) {
		        	savePPMFile << static_cast<unsigned int>(ppmFile.mPixels[i].rgb[0]) << std::endl
		                        << static_cast<unsigned int>(ppmFile.mPixels[i].rgb[1]) << std::endl
		                        << static_cast<unsigned int>(ppmFile.mPixels[i].rgb[2]) << std::endl;
	        	}
	        	break;
        	case '6':
        		for (auto i = 0; i < ppmFile.size; ++i) {
		        	savePPMFile.write((char*)&ppmFile.mPixels[i].rgb[0], sizeof(unsigned char));
		        	savePPMFile.write((char*)&ppmFile.mPixels[i].rgb[1], sizeof(unsigned char));
		        	savePPMFile.write((char*)&ppmFile.mPixels[i].rgb[2], sizeof(unsigned char));
        		}
        		break;
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
    //P6
    //PPM *Origin = loadPPM(std::string("Mandrill-gray.ppm"));
    //P3
    PPM *Origin = loadPPM(std::string("lena_color.ppm"));
    
    // Apply gamma correction
    gamma_corrected(4.f, *Origin);
    // Save results to disk    
    savePPM(std::string("gamma_corrected4.ppm"), *Origin);

    // Not linear inverse
    gamma_corrected(0.25f, *Origin);
    savePPM(std::string("gamma_corrected25.ppm"), *Origin);

    // Other pass
    gamma_corrected(0.25f, *Origin);
    savePPM(std::string("gamma_corrected25x2.ppm"), *Origin);

    return 0;
}
