# CS-GY 6533 A Interactive Computer Graphics

# Assignment 1: Images 

## Index
- [CS-GY 6533 A Interactive Computer Graphics](#cs-gy-6533-a-interactive-computer-graphics)
- [Assignment 1: Images](#assignment-1-images)
	- [Index](#index)
	- [General](#general)
	- [File structure](#file-structure)
	- [Reminder](#reminder)
	- [Implement Image Class](#implement-image-class)
		- [Image class frame :](#image-class-frame-)
		- [Image data structure](#image-data-structure)
		- [Test basic fumction of load, save etc.](#test-basic-fumction-of-load-save-etc)
		- [Output:](#output)
	- [Exception](#exception)
		- [Examples:](#examples)
		- [Handle trivial problem](#handle-trivial-problem)
	- [Overload operator[]](#overload-operator)
		- [Test:](#test)
		- [Output:](#output-1)
	- [Overload Operator+](#overload-operator-1)
		- [Test:](#test-1)
		- [Output:](#output-2)
	- [Overload Operator+=](#overload-operator-2)
		- [Test:](#test-2)
		- [Output:](#output-3)
	- [Overload Operator-](#overload-operator-)
		- [Test:](#test-3)
		- [Output:](#output-4)
	- [Overload Operator*](#overload-operator-3)
		- [Test:](#test-4)
		- [Output:](#output-5)
	- [Gamma Correction](#gamma-correction)
		- [Test:](#test-5)
		- [Output:](#output-6)
	- [Alpha Compositing](#alpha-compositing)
		- [Test:](#test-6)
		- [Output:](#output-7)


---

## General

implementing a fine image class that enabel reading PPM and PGM image files

overload the operators and implement functions for gamma correction and alpha compositing.

---

## File structure

1st_test_multi_type_io -----------files of my first step: implement and test `load()` and `save()` on Linux

2nd_implement_image_class --------files of my second step: implement basic `Image` class, see next dir for refined version 

Assignment_1_solution-------------VS2019 project that complete all tasks

images----------------------------basic test images provided by professor

requirement-----------------------detailed requiement of the assignment

sub_final-------------------------submmision of Linux & CMAKE version 

---

## Reminder

Execution: 

1. create build/ 
2. `cmake ../` in build/ 
3. `make` in build/ 
4. `./Assignment1` 
5. all outcome images is in build/

All task results in the report are obtained by executing on Windows by VS2019, but the program has been tested on Linux by CMAKE as well, and submission is a Linux & CMAKE version.

All test images are in *test_images* directory, and *images* directory saves images for *README.md*.

The "file path" for loading image in *source.cpp* in submission is slight different from that in this report because project structure for CMAKE is slightly different from that for VS2019. 

---

## Implement Image Class

### Image class frame :

```cpp
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
	void reset(); //usually used in destructor or in load 
	//to free dynamic memory of old image inside the class, reset the instance.
	void clear(); //clear picture to "0".
	bool empty(); //check if there is image data of dynamic memory inside the instance

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
 
};
```

### Image data structure

To save image of different #channel, the structure of image data is a pixel pointer array pointed by a pointer `mPixel`, and inside this array are pointers pointing to array of size of `channel` that save actual data of each pixel of different channel.

Below is a code snippet showing this structure in `load()` :

```cpp
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
```

### Test basic fumction of load, save etc.

```cpp
//------------------------------------- test basic function---------------------------------
	Image img1("./test_images/Mandrill.ppm");  //p6
	//Image img2("./test_images/tandon_stacked_color.ppm"); //p3
	Image img3("./test_images/PGM_P5_sample.pgm"); //p5 different size

	//------------------------------------- test basic function---------------------------------
	
	img1.save("test_save.ppm");
	if (img1.empty())cout << "empty(): empty" << endl;
	else cout << "empty(): not empty" << endl;

	img1.clear();
	img1.save("test_save_empty.ppm");
	img1.empty();

	img1.reset();
	if (img1.empty())cout << "empty(): empty" << endl;
	else cout << "empty(): not empty" << endl;

	img3.save("test_save_P5.pgm"); // test on P5 PGM
```

### Output: 

![](/images/2020-09-21-20-00-20.png)

**test_save.ppm, test_save_empty.ppm and test_save_P5.pgm:**

![](/images/2020-09-21-20-01-46.png)

---

## Exception

To ensure program / function run correctly and safely, when there is problem that can cause illegal result or potentially affect later execution, exception will be threw and program will terminated.

### Examples:

**load():** Can't open file, affect later task, throw exception.

![](/images/2020-09-22-15-10-33.png)

**save():** Can't save file, task failed , throw exception.

![](/images/2020-09-22-15-11-24.png)

**type():** instances with no data have no information on type, if we want to use type as condition later, it brings potential risk, so exception will be threw.

![](/images/2020-09-22-15-12-38.png)

**alpha_compositing():** if instance(s) have no data or different size/channel, we can't process them, throw exception. 

![](/images/2020-09-22-15-13-48.png)

### Handle trivial problem


There will be some trivial problems if we handle it well , we won't need to throw exception.

**example1**:

`reset()` is designed to erase(free) dynamic image data, usually used in destructor. 
When we mistakely reset any instance twice, it cause "double release" problem, we should stop it. So condition is set to prevent it, and no need to throw exception.

![](/images/2020-09-22-15-21-16.png)

**example2**:

There will be chance that we might want to load a new image to a instance that have old image inside it. So we need to free old dynamic data and attach new data to the instance. See below:

![](/images/2020-09-22-15-48-24.png)

---

## Overload operator[]

Overload operator[] to read pixel value, `imgclass[index]` will return a pointer pointing array that containing pixel value(s).

But we should be careful with pixel, not recommend to modify them through [].

"subscript out of range" and "access empty instance" will be check before return result.

If we want to print pixel value at output by returned pixel array, we should transform `unsigned char` to `unsigned int` and **follow the restriction of #channel**. For example:

### Test:

```cpp
	//------------------------------- test operator[] ----------------------------------------------

	cout << endl;cout << "access P5: ";
	for(int i = 0 ; i < img1.image->channel ; i++)cout << (unsigned int)img1[0][i]<<" ";
	cout << endl << endl;

	cout << endl;cout << "access P2: ";
	for (int i = 0; i < img3.image->channel; i++)cout << (unsigned int)img3[0][i] << " ";
	cout << endl << endl;

	//img1.reset(); //empty instance exception
	//cout << img1[0][0] <<endl; 
	cout << img1[-1][0] << endl;//test subscript exception
```

### Output:

![](/images/2020-09-23-13-53-19.png)

---

## Overload Operator+

Overload operator+ to allow two pictures to perform addition and **return a new picture** that each pixel's value is sum of two original pictures. It will do type(#channel)/size checking to see if two images can do addition.

For example, it will allows same-size PPM of P3 and P6 to do addition.

### Test:
 
```cpp
	//------------------------------- test operator+ ----------------------------------------------
    Image img1("./test_images/Mandrill.ppm");  //p6
	Image img2("./test_images/tandon_stacked_color.ppm"); //p3
	
    Image sum_ppm = img1 + img2;
	sum_ppm.save("test_add_ppm.ppm");
	//img1.save("test_sum_origin1.ppm");
	//img2.save("test_sum_origin2.ppm");

	Image sum_pgm = img3 + img6;
	sum_pgm.save("test_add_pgm.pgm");
```    
### Output:

![](/images/2020-09-22-21-48-54.png)

**test_add_ppm.ppm:**

![](/images/2020-09-22-21-49-45.png)

**test_add_pgm.pgm:**

![](/images/2020-09-22-21-50-14.png)

---

## Overload Operator+=

When overloading the addition assignment operator (operator+=), it will average the result of adding the two pixels values as required.

### Test:

```cpp
	//------------------------------- test operator+= ----------------------------------------------

	Image img4("./test_images/Mandrill.ppm");  //p6
	Image img5("./test_images/tandon_stacked_color.ppm"); //p3
	img4 += img5;
	img4.save("test_add_assign.ppm");
	//img5.save("test_sum_equal_origin_img5.ppm");
```

### Output:

![](/images/2020-09-21-20-24-18.png)

**test_add_assign.ppm:**

![](/images/2020-09-21-20-25-39.png)

---

## Overload Operator-

Overload operator- to performing subtraction between pictures. It will check if subtraction cause result smaller than 0 and set value to 0 when underflow.

*It will return a new image instance after subtraction.*

### Test:

```cpp
	//------------------------------- test operator- ----------------------------------------------

	Image res1 = img1 - img2;
	res1.save("test_sub.ppm");
```

### Output:

![](/images/2020-09-21-20-32-27.png)

**test_sub.ppm:**

![](/images/2020-09-21-20-33-37.png)

---

## Overload Operator*

Overload multiplication of pixels by a scalar. *It will return a new image instance after multiplication.*

**I think if we multilpy a img with scalar, its brightness is adjusted.** 

*PS: Mistakely overload * to perform multiplication between images at first. Also recorded here. But I think this kind of multiplication is meaningless because its effect is random.*

### Test:

```cpp
	//------------------------------- test operator* ----------------------------------------------

	Image res10 = img1 * 0.8;
	res10.save("test_mul_scalar_0.8.ppm");
	//img1.save("test_mul_scalr_origin.ppm");
	Image res2 = img1 * img2;
	res2.save("test_mul_img.ppm");
	//img1.save("test_mul_img_origin1.ppm");
	//img2.save("test_mul_img_origin2.ppm");
```

### Output:

![](/images/2020-09-22-00-45-30.png)

**By scalar: (ADJUST BRIGHTNESS)**

![](/images/2020-09-22-00-47-31.png)

**By Image: (EFFECT IS RANDOM)**

![](/images/2020-09-22-00-46-35.png)

---

## Gamma Correction

Gamma correction given float type gamma as parameter. *It will return a new image instance after gamma correction.*

Also tested with P5 PGM picture.

### Test:

```cpp
	//------------------------------- test gamma correction-----------------------------------------------

	Image res3 = img1.gamma_correction(4.0);
	res3.save("gamma_corrected_4.ppm");
	Image res4 = res3.gamma_correction(0.25);
	res4.save("gamma_corrected_025 after_4.ppm");
	//Image res5 = res4.gamma_correction(0.25);
	//res5.save("gamma_corrected_2x025.ppm");
	Image res8 = img3.gamma_correction(0.25);  //test with p5
	img3.save("P5_gamma_origin.pgm");
	res8.save("P5_gamma_corrected_025.pgm");
```

### Output:

![](/images/2020-09-21-20-45-43.png)

**results:**

![](/images/2020-09-21-20-48-45.png)

---

## Alpha Compositing

Performing alpha compositing with background image an alpha as parameters. 

*It will return a new image as result.*

Follow the formula: **Cmew = Cfront * alpha + Cback * (1-alpha)** .

### Test:

```cpp
	//------------------------------- test alpha compositing-----------------------------------------------

	Image res6 = img1.alpha_compositing(img2, 0.5);
	res6.save("alpha_compositing_05_man_tandon.ppm");
	Image res7 = img1.alpha_compositing(img2, 0.85);
	res7.save("alpha_compositing_085_man_tandon.ppm");

	Image ac_pgm = img3.alpha_compositing(img6, 0.7);
	ac_pgm.save("alpha_compositing_07_f12_columns.pgm");
```

### Output:

![](/images/2020-09-22-22-00-37.png)

**PPM & alpha = 0.5 :**

![](/images/2020-09-22-22-01-07.png)

**PPM & alpha = 0.85 :**

![](/images/2020-09-22-22-01-23.png)

**PGM & alpha = 0.7 :**

![](/images/2020-09-22-22-02-10.png)

---
