

#include "gtest/gtest.h"

#include <queue>
//#include "CppUnitTest.h"
//using namespace Microsoft::VisualStudio::CppUnitTestFramework;

class QueueTest : public testing::Test {
protected:
	QueueTest() {
		// q0_ remains empty
		q1_.push(1);
		q2_.push(2);
		q2_.push(3);
	}

	// ~QueueTest() override = default;

	std::queue<int> q0_;
	std::queue<int> q1_;
	std::queue<int> q2_;
};

TEST(HelloTest, BasicAssertions)
{
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
}

TEST_F(QueueTest, IsEmptyInitially) {
	EXPECT_EQ(q0_.size(), 0);
}

TEST_F(QueueTest, DequeueWorks) {
	int n = q0_.size();
	EXPECT_EQ(n, 0);

	n = q1_.back();
	ASSERT_NE(n, 0);
	EXPECT_EQ(n, 1);
	q1_.pop();
	EXPECT_EQ(q1_.size(), 0);

	n = q2_.back();
	ASSERT_NE(n, 0);
	q2_.pop();
	EXPECT_EQ(n, 3);
	EXPECT_EQ(q2_.size(), 1);
}