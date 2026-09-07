#include "gtest/gtest.h"
#include "gmock/gmock.h"

class SimpleEnvironment : public ::testing::Environment {
public:
	SimpleEnvironment() {}
	~SimpleEnvironment() override {}
	// Override this to define how to set up the environment.
	void SetUp() override { std::cout << __func__ << std::endl; }
	// Override this to define how to tear down the environment.
	void TearDown() override { std::cout << __func__ << std::endl; }
};
int main(int argc, char* argv[]) {
	testing::InitGoogleTest(&argc, argv);
	testing::Environment* const env =
		testing::AddGlobalTestEnvironment(new SimpleEnvironment);
	return RUN_ALL_TESTS();
}