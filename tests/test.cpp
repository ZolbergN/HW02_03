#include <catch.hpp>
#include <explorer.hpp>
TEST_CASE("Check functions", "[explorer]") {
	SECTION("Check INIT")
	{
		fs::path Path = "./tests";
		bool ok = print_info(Path);
		REQUIRE( ok == true);
	}
}
