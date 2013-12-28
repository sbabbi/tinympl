
#include <type_traits>
#include <tinympl/string.hpp>

namespace test_string
{

using namespace tinympl;

typedef basic_string<char,'l','o','w','d'> lowd_t;
typedef lowd_t::insert_c<0,'h','e','l'>::type hellowd_t;
typedef hellowd_t::insert_c<6,'o','r','l'>::type helloworld_t;

static_assert(
	std::is_same<
		helloworld_t,
		basic_string<char,'h','e','l','l','o','w','o','r','l','d'>
	>::value, "basic_string::insert_c ");

static_assert(
	basic_string<char,'h','e','l','l'>::compare<
		basic_string<char,'h','e','l','l','o'>
	>::value == -1,"basic_string::compare");

static_assert(
	basic_string<char,'h','f','l','l','o'>::compare<
		basic_string<char,'h','e','l','l','o'>
	>::value == 1,"basic_string::compare");

static_assert(
	basic_string<char,'h','e','l','l','o'>::compare<
		basic_string<char,'h','e','l','l','o'>
	>::value == 0,"basic_string::compare");

constexpr char Foo[] = "helloworld";

static_assert(
	std::is_same<
		string<Foo>,
		helloworld_t
	>::value,"string<>");

MAKE_TINYMPL_STRING(test,"helloworld");

static_assert(
	std::is_same<
		test,
		helloworld_t
	>::value,"make_mpl_string");

}
