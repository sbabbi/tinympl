#include <cassert>

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
	
	assert(mm.at<int>() == "Hello");
	assert(mm.at<char>() == 44L);
	assert(mm.at<long>() == 'c');
	assert(mm.at<int*>() == "intptr");
}

void test_fused_value_map()
{
	typedef tinympl::fused_value_map<int,std::string, 6,4,12,18> fmap;
	fmap mm = {"int","char","long","string"};
	
	assert( mm.at<6>() == "int");
	assert( mm.at<4>() == "char");
	assert( mm.at<12>() == "long");
	assert( mm.at<18>() == "string");
}

int main()
{
	test_fused_map();
	test_fused_value_map();
	return 0;
}
