// Copyright 2020 Jonathas Costa and Claudio Silva. All rights reserved.
// Modified from original version to enable process for P6 PPM by Haotian Yi
// Modified from original version to enable process for PGM: P2,P5; PPM: P3,P6 by Haotian Yi
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include<memory>
using namespace std;

// Definition of a pixel type
// typedef struct {
//     unsigned char rgb[3];
// } Pixel;

//typedef shared_ptr<unsigned char> Pixel; //mpixel manages array consisting off shared pointer to rgb array, failed!

typedef unsigned char* Pixel;

// PPM file structure type
struct PPM {
    Pixel *mPixels = nullptr;
    unsigned int mRows;
    unsigned int mColumns;
    unsigned int size;
    char mType[3];
    unsigned int mMaxValue;
    unsigned int channel = 3;
};

// Reads a PGM/PPM file from the disk.
PPM * loadPPM(const std::string & fileName) {
    std::ifstream imageFile;
    imageFile.open(fileName, ios::in | ios::binary);

    if (!imageFile) {
        std::cout << "Error opening image file " << fileName << std::endl;
        return nullptr;
    }

    PPM * ppmImage = new PPM;
    //directily read ascii for PPM file header info
    imageFile >> ppmImage->mType; 
    imageFile >> ppmImage->mColumns;   
    imageFile >> ppmImage->mRows;      
    ppmImage->size = ppmImage->mColumns * ppmImage->mRows;    
    imageFile >> ppmImage->mMaxValue;	

    if (ppmImage->mPixels != nullptr) {
        delete[] ppmImage->mPixels;
    }

    //decide channel by PPM type
    
    if(ppmImage->mType[1]=='3' || ppmImage->mType[1]=='6')ppmImage->channel=3;
    if(ppmImage->mType[1]=='2' || ppmImage->mType[1]=='5')ppmImage->channel=1;

    cout<<"Type:"<<ppmImage->mType[0]<<ppmImage->mType[1]<<endl;
    cout<<"col:"<<ppmImage->mColumns<<endl;
    cout<<"row:"<<ppmImage->mRows<<endl;
    cout<<"size:"<<ppmImage->size<<endl;
    cout<<"mMaxValue:"<<ppmImage->mMaxValue<<endl;
    cout<<"Channel:"<<ppmImage->channel<<endl;

    ppmImage->mPixels = new Pixel[ppmImage->size];

    //auto i = 0;
    switch(ppmImage->mType[1]){
    	case '3': //read P3: ascii plain character data
    		ppmImage->channel = 3;
    		for (auto i = 0; i < ppmImage->size; ++i) {
    			Pixel pixel = new unsigned char[ppmImage->channel];
    			ppmImage->mPixels[i]=pixel;
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
        	for (auto i = 0; i < ppmImage->size; ++i) {
        		Pixel pixel = new unsigned char[ppmImage->channel];
    			ppmImage->mPixels[i]=pixel;
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
        	for (auto i = 0; i < ppmImage->size; ++i) {
        		Pixel pixel = new unsigned char[ppmImage->channel];
    			ppmImage->mPixels[i]=pixel;
		        unsigned char grey;
		        imageFile.read((char*)&grey, sizeof(unsigned char));
		        ppmImage->mPixels[i][0] = grey;
		    }
		    break; 

		case '2': //read P3: ascii plain character data
    		ppmImage->channel = 1;
    		for (auto i = 0; i < ppmImage->size; ++i) {
    			Pixel pixel = new unsigned char[ppmImage->channel];
    			ppmImage->mPixels[i]=pixel;
		        unsigned int grey;//read ascii file can't use unsigned char at first
		        imageFile >> grey;
		        ppmImage->mPixels[i][0] = static_cast<unsigned char>(grey);
		    }
    		break;       

    }

    
    imageFile.close();

    cout<<"finish loading "<<fileName<<endl;
    return ppmImage;


}

// Saves a P6 PPM file.
void savePPM(const std::string & fileName, const PPM & ppmFile) {
    std::cout << "Saving file type: "<<ppmFile.mType[0]<<ppmFile.mType[1]<< " ..." << std::endl;
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
        		for (auto i = 0; i < ppmFile.size; i++) {
		        	savePPMFile << static_cast<unsigned int>(ppmFile.mPixels[i][0]) << std::endl
		                        << static_cast<unsigned int>(ppmFile.mPixels[i][1]) << std::endl
		                        << static_cast<unsigned int>(ppmFile.mPixels[i][2]) << std::endl;
	        	}
	        	break;
        	case '6':
        		for (auto i = 0; i < ppmFile.size; i++) {
		        	savePPMFile.write((char*)&ppmFile.mPixels[i][0], sizeof(unsigned char));
		        	savePPMFile.write((char*)&ppmFile.mPixels[i][1], sizeof(unsigned char));
		        	savePPMFile.write((char*)&ppmFile.mPixels[i][2], sizeof(unsigned char));
        		}
        		break;
        	case '2':
        		for (auto i = 0; i < ppmFile.size; i++) {
		        	savePPMFile << static_cast<unsigned int>(ppmFile.mPixels[i][0]) << std::endl;
	        	}
	        	break;
        	case '5':
        		for (auto i = 0; i < ppmFile.size; i++) {
		        	savePPMFile.write((char*)&ppmFile.mPixels[i][0], sizeof(unsigned char));
        		}
        		break;        		
        }
        
        
  
        savePPMFile.close();
    }
    cout<<"Finish saving "<<fileName<<endl;
}

//implement the following code to apply a gamma correction procedure
// given the a gamma value and a PPM image file.

void gamma_corrected(const float gamma, PPM & ppmFile) {
	cout<<"Gamma Correcting..."<<endl;
	for (auto i = 0; i < ppmFile.size; ++i) {
		for(int j = 0 ; j < ppmFile.channel ; j++){
			ppmFile.mPixels[i][j] = static_cast<unsigned char>(
            	static_cast<float>(ppmFile.mMaxValue) * 
            	pow(((float)ppmFile.mPixels[i][j]/static_cast<float>(ppmFile.mMaxValue)),gamma) );
          
		}
            // ppmFile.mPixels[i][0] = static_cast<unsigned char>(
            // 	static_cast<float>(ppmFile.mMaxValue) * 
            // 	pow(((float)ppmFile.mPixels[i][0]/static_cast<float>(ppmFile.mMaxValue)),gamma) );
            // ppmFile.mPixels[i][1] = static_cast<unsigned char>(
            // 	static_cast<float>(ppmFile.mMaxValue) * 
            // 	pow(((float)ppmFile.mPixels[i][1]/static_cast<float>(ppmFile.mMaxValue)),gamma) );
            // ppmFile.mPixels[i][2] = static_cast<unsigned char>(
            // 	static_cast<float>(ppmFile.mMaxValue) * 
            // 	pow(((float)ppmFile.mPixels[i][2]/static_cast<float>(ppmFile.mMaxValue)),gamma) );
            
    }
}

int main(int argc, char * argv[]) {

    // Load Original Image
    //P6
    PPM *Origin = loadPPM(std::string("Mandrill-gray.ppm"));
    //P3
    //PPM *Origin = loadPPM(std::string("lena_color.ppm"));
    //P5
    //PPM *Origin = loadPPM(std::string("PGM_P5_sample.pgm"));
    cout<<"1"<<endl;
    // Apply gamma correction
    gamma_corrected(4.f, *Origin);
    cout<<"2"<<endl;
    // Save results to disk    
    savePPM(std::string("gamma_corrected4.ppm"), *Origin);
    //savePPM(std::string("gamma_corrected4.pgm"), *Origin);
    cout<<"3"<<endl;
    // Not linear inverse
    gamma_corrected(0.25f, *Origin);
    cout<<"4"<<endl;
    savePPM(std::string("gamma_corrected25.ppm"), *Origin);
    //savePPM(std::string("gamma_corrected25.pgm"), *Origin);
    cout<<"5"<<endl;
    // Other pass
    gamma_corrected(0.25f, *Origin);
    cout<<"6"<<endl;
    savePPM(std::string("gamma_corrected25x2.ppm"), *Origin);
    //savePPM(std::string("gamma_corrected25x2.pgm"), *Origin);
    cout<<"7"<<endl;
    // for (auto i = 0; i < Origin->size; ++i){
    // 	delete[] Origin->mPixels[i];
    // }
    // delete[] Origin->mPixels;

    return 0;
}
