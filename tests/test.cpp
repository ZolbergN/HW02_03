#include <catch.hpp>
#include ".\main.hpp"
TEST_CASE("Check functions", "[explorer]") {
	SECTION("Check INIT")
	{
		fs::path Path = "../tests";
		bool ok = print_info(Path);
		REQUIRE( ok == true);
	}
}
