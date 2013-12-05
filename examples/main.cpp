#include <iostream>

#include "algorithm_example.hpp"
#include "algorithm_variadic_example.hpp"
#include "lambda_example.hpp"
#include "map_example.hpp"
#include "value_map_example.hpp"
#include "vector_example.hpp"

#include <tinympl/fused_map.hpp>
#include <tinympl/fused_value_map.hpp>

using namespace std;

void test_fused_map()
{
	typedef tinympl::fused_map<
		std::pair<int,std::string>,
		std::pair<int*,std::string>,
		std::pair<long,char>,
		std::pair<char,long> > fmap;
	
	fmap mm ("Hello","intptr",'c',44L);
	
	cout << mm.at<int>() << endl;
	cout << mm.at<char>() << endl;
	cout << mm.at<long>() << endl;
	cout << mm.at<int*>() << endl;
}

void test_fused_value_map()
{
	typedef tinympl::fused_value_map<int,const char*, 6,4,12,18> fmap;
	fmap mm = {"int","char","long","string"};
	
	cout << mm.at<6>() << endl;
	cout << mm.at<4>() << endl;
	cout << mm.at<12>() << endl;
	cout << mm.at<18>() << endl;
}

int main()
{
	test_fused_map();
	test_fused_value_map();
	return 0;
}
