#include "Image.h"
#include<iostream>
#include<memory>
using namespace std;



int main() {
	//string filename = "Mandrill-gray.ppm";  //p6
	//string filename = "lena_color.ppm";   //p3
	//Image* img1 = new Image("./images/Mandrill-gray.ppm");  //p6
	//Image* img2 = new Image("./images/tandon_stacked_color.ppm"); //p3
	//Image* img3 = new Image("./images/Mandrill.ppm");
	


	//------------------------------------- test basic function ---------------------------------
	//cout << "1type: " << img1->type() << endl;
	//cout << "1channel: " << img1->image->channel << endl;
	//cout << "1row: " << img1->row() << endl;
	//cout << "1column: " << img1->column() << endl;
	//cout << "1size: " << img1->size() << endl;
	//img1->save("test_save.ppm");
	//if (img1->empty())cout << "empty(): empty" << endl;
	//else cout << "empty(): not empty" << endl;

	//img1->clear();
	//img1->save("test_save_empty.ppm");

	//img1->reset();
	//if (img1->empty())cout << "empty(): empty" << endl;
	//else cout << "empty(): not empty" << endl;


	Image img1("../images/Mandrill.ppm");  //p6
	Image img2("../images/tandon_stacked_color.ppm"); //p3
	Image img3("../images/PGM_P5_sample.pgm"); //p5 different size
	//------------------------------- test operator+ ----------------------------------------------
	//Image sum = img1 + img2;
	//sum.save("test_sum.ppm");
	//img1.save("test_sum_origin1.ppm");
	//img2.save("test_sum_origin2.ppm");
	//------------------------------- test operator+= ----------------------------------------------
	//Image img3("./images/Mandrill.ppm");  //p6
	//Image img4("./images/tandon_stacked_color.ppm"); //p3
	//img3 += img4;
	//img3.save("test_sum_equal.ppm");
	//img4.save("test_sum_equal_origin_img2.ppm");
	//------------------------------- test operator- ----------------------------------------------
	//Image res1 = img1 - img2;
	//res1.save("test_sub.ppm");
	//img1.save("test_sub_origin_img1.ppm");
	//img2.save("test_sub_origin_img2.ppm");
	//------------------------------- test operator* ----------------------------------------------
	//Image res2 = img1 * img2;
	//res2.save("test_muliply.ppm");
	//img1.save("test_muliply_origin_img1.ppm");
	//img2.save("test_muliply_origin_img2.ppm");
	//------------------------------- test gamma correction-----------------------------------------------
	//Image res3 = img1.gamma_correction(4.0);
	//res3.save("gamma_corrected_4.ppm");
	//Image res4 = res1.gamma_correction(0.25);
	//res4.save("gamma_corrected_025.ppm");
	//Image res5 = res2.gamma_correction(0.25);
	//res5.save("gamma_corrected_2x025.ppm");
	//Image res8 = img3.gamma_correction(0.25);  //test with p5
	//img3.save("P5_gamma_origin.pgm");
	//res8.save("P5_gamma_corrected_025.pgm");
	//------------------------------- test alpha compositing-----------------------------------------------
	Image res6 = img1.alpha_compositing(img2, 0.5);
	res6.save("alpha_compositing_0.5_man_tandon.ppm");
	Image res7 = img1.alpha_compositing(img2, 0.85);
	res7.save("alpha_compositing_0.85_man_tandon.ppm");
	//------------------------------- test erosion--------------------------------------------------------
	//Image res9 = img1.gamma_correction(0.25);
	//img1.save("erosion_origin.ppm");
	//res9.save("erosion_result.ppm");

	return 0;
}