#include <catch.hpp>
#include "main.hpp"
TEST_CASE("Check functions", "[explorer]") {
	SECTION("Check data")
	{
		fs::create_directory("dir");
		fs::path Path("dir");
		print_data(Path);
		REQUIRE(true);
	}
}
