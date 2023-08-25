#include <iostream>
#include <cstdlib>
#include <limits>

#include <opencv2/core.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/quality.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;

int main()
{
	try{
		
		if(!cv::utils::fs::exists("/usr/share/doc/opencv-doc"))
		{
			std::clog<<"This test unit required the installation of the package 'opencv-doc' (ubuntu)"<<std::endl;
			return EXIT_FAILURE;
		}
		
//		String pwd = cv::utils::fs::getcwd();

		String pwd = "/media/smile/3FA6E701592EA94E/prog/cudaAtScaleV2/bin";

		std::cout<<"PWD: "<<pwd<<std::endl<<std::flush;

		String execute_bin = format("%s/cudaAtScaleV2.exe --input=/usr/share/doc/opencv-doc/examples/data/left%%02d.jpg --output=%s/../data/test_tgt/unit_left_%%02d.jpg", pwd.c_str(), pwd.c_str());
		
		std::system(execute_bin.c_str());
		
		VideoCapture ref;
		VideoCapture tgt;
		
		if(!ref.open(format("%s/../data/test_ref/unit_left_%%02d.jpg", pwd.c_str()), cv::CAP_IMAGES))
		{
			std::clog<<"Unit test reference image folder not found!"<<std::endl;
			return EXIT_FAILURE;
		}
		
		if(!tgt.open(format("%s/../data/test_tgt/unit_left_%%02d.jpg", pwd.c_str()), cv::CAP_IMAGES))
		{
			std::clog<<"Unit test target image folder not found!"<<std::endl;
			return EXIT_FAILURE;
		}
		
		if(ref.get(cv::CAP_PROP_FRAME_COUNT) != tgt.get(cv::CAP_PROP_FRAME_COUNT))
		{
			std::clog<<"Reference and target sizes does not match!"<<std::endl;
			return EXIT_FAILURE;
		}
		
		int nb_images = static_cast<int>(ref.get(cv::CAP_PROP_FRAME_COUNT));
		
		
		for(int i=0; i<nb_images;i++)
		{
			Mat ref_img, tgt_img;
			
			ref >> ref_img;
			tgt >> tgt_img;
			
			Ptr<quality::QualityMSE> metric = quality::QualityMSE::create(ref_img);

			Scalar delta = metric->compute(tgt_img);
			
			for(int j=0; j<ref_img.channels(); j++)
			{
			   if(delta(j) > std::numeric_limits<float>::epsilon())
			   {
			   	std::clog<<"Difference between source and target is too high"<<std::endl;
			   	return EXIT_FAILURE;
			   }
			}
		}
		
		return EXIT_SUCCESS;	
	}
	catch(std::exception& err)
	{
		std::clog<<"An Unexpected Error Has Occured: "<<err.what()<<std::endl;
	}
	
	return EXIT_FAILURE;
}
