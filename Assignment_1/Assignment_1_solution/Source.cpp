#include "Image.h"
#include<iostream>
#include<string>
#include<memory>
#include<exception>
#include<algorithm>
using namespace std;



int main() {
	try
	{
		Image img1("./test_images/Mandrill.ppm");  //p6
		Image img2("./test_images/tandon_stacked_color.ppm"); //p3
		Image img3("./test_images/f14_p2.pgm"); //p2 
		Image img6("./test_images/columns_p5.pgm");//p5
		
		//------------------------------------- test basic function---------------------------------

		img1.save("test_overwrite_origin.ppm"); // test save()
		img1.load("./test_images/tandon_stacked_color.ppm");//test overwrite
		img1.save("test_overwrite.ppm");
		
		if (img1.empty())cout << "empty(): empty" << endl; //test empty()
		else cout << "empty(): not empty" << endl;

		img1.clear(); //test clear()
		img1.save("test_save_empty.ppm");
		
		img1.reset(); //test reset()
		//img1.clear();
		//img1.save("test_save_no_data_exception"); //test save no data exception 
		//if (img1.empty())cout << "empty(): empty" << endl;
		//else cout << "empty(): not empty" << endl;

		img3.save("test_save_p5.pgm"); //test save PGM

		img1.load("./test_images/Mandrill.ppm");
		//------------------------------- test operator[] ----------------------------------------------

		cout << endl;cout << "access P5: ";
		for(int i = 0 ; i < img1.image->channel ; i++)cout << (unsigned int)img1[0][i]<<" ";
		cout << endl << endl;

		cout << endl;cout << "access P2: ";
		for (int i = 0; i < img3.image->channel; i++)cout << (unsigned int)img3[0][i] << " ";
		cout << endl << endl;

		//img1.reset(); //empty instance exception
		//cout << img1[0][0] <<endl; 
		//cout << img1[-1][0] << endl;//subscript exception

		//------------------------------- test operator+ ----------------------------------------------

		Image sum_ppm = img1 + img2;
		sum_ppm.save("test_add_ppm.ppm");
		//img1.save("test_sum_origin1.ppm");
		//img2.save("test_sum_origin2.ppm");

		Image sum_pgm = img3 + img6;
		sum_pgm.save("test_add_pgm.pgm");

		//------------------------------- test operator+= ----------------------------------------------

		Image img4("./test_images/Mandrill.ppm");  //p6
		Image img5("./test_images/tandon_stacked_color.ppm"); //p3
		img4 += img5;
		img4.save("test_add_assign.ppm");
		//img5.save("test_sum_equal_origin_img5.ppm");

		//------------------------------- test operator- ----------------------------------------------

		Image res1 = img1 - img2;
		res1.save("test_sub.ppm");
		//img1.save("test_sub_origin_img1.ppm");
		//img2.save("test_sub_origin_img2.ppm");

		//------------------------------- test operator* ----------------------------------------------

		Image res10 = img1 * 0.5;
		res10.save("test_mul_scalar_0.5.ppm");
		//img1.save("test_mul_scalr_origin.ppm");
		//Image res2 = img1 * img2;
		//res2.save("test_mul_img.ppm");
		//img1.save("test_mul_img_origin1.ppm");
		//img2.save("test_mul_img_origin2.ppm");

		//------------------------------- test gamma correction-----------------------------------------------

		Image res3 = img1.gamma_correction(4.0);
		res3.save("gamma_corrected_4.ppm");
		//Image res4 = res3.gamma_correction(0.25);
		//res4.save("gamma_corrected_025 after_4.ppm");
		////Image res5 = res4.gamma_correction(0.25);
		////res5.save("gamma_corrected_2x025.ppm");
		//Image res8 = img3.gamma_correction(0.25);  //test with p5
		//img3.save("P5_gamma_origin.pgm");
		//res8.save("P5_gamma_corrected_025.pgm");

		//------------------------------- test alpha compositing-----------------------------------------------

		Image res6 = img1.alpha_compositing(img2, 0.5);
		res6.save("alpha_compositing_05_man_tandon.ppm");
		Image res7 = img1.alpha_compositing(img2, 0.85);
		res7.save("alpha_compositing_085_man_tandon.ppm");

		Image ac_pgm = img3.alpha_compositing(img6, 0.7);
		ac_pgm.save("alpha_compositing_07_f12_columns.pgm");

		//------------------------------- test erosion--------------------------------------------------------
		//not complete...
		

		return 0;
	}
	catch (string &e) {
		cout << "Error: " << e << endl;
	}
	catch (exception &e)
	{
		cout << "Error: " << e.what() << endl;
	}
	
}