#include <gtest/gtest.h>
#include "../lib/scheduler.h"

struct Diddy{
	int factor;

	int mul(int x) const {
		return x * factor;
	}

	TTaskScheduler::FutureResult<float> add_and_get_future(TTaskScheduler& scheduler, float val) {
		auto id = scheduler.add([val]() { return val + 10.0f; });
		return scheduler.getFutureResult<float>(id);
	}
};

struct AddNumber{
	float value_to_add;

	float add(float& num){
		num += value_to_add;
		return num;
	}

	float add_two_numbers(float& a, float& b){
		return a + b;
	}
};


TEST(TaskSchedulerTest, SimpleLambdaNoArgs){
	TTaskScheduler scheduler;
	auto id = scheduler.add([]() { return 42; });
	scheduler.executeAll();
	EXPECT_EQ(scheduler.getResult<int>(id), 42);
}

TEST(TaskSchedulerTest, LambdaWithOneArg){
	TTaskScheduler scheduler;
	int x = 5;
	auto id = scheduler.add([](int val) { return val * 2; }, x);
	scheduler.executeAll();
	EXPECT_EQ(scheduler.getResult<int>(id), 10);
}

TEST(TaskSchedulerTest, LambdaWithTwoArgs){
	TTaskScheduler scheduler;
	int a = 3, b = 4;
	auto id = scheduler.add([](int x, int y) { return x + y; }, a, b);
	scheduler.executeAll();
	EXPECT_EQ(scheduler.getResult<int>(id), 7);
}

TEST(TaskSchedulerTest, MethodPointerCall){
	TTaskScheduler scheduler;
	Diddy d{.factor = 4};
	int value = 6;
	auto id = scheduler.add(&Diddy::mul, d, value);
	scheduler.executeAll();
	EXPECT_EQ(scheduler.getResult<int>(id), 24);
}

TEST(TaskSchedulerTest, TaskIsEvaluatedOnlyOnce){
	TTaskScheduler scheduler;
	int call_count = 0;

	auto id = scheduler.add([&call_count](){
		++call_count;
		return 99;
	});

	scheduler.executeAll();
	EXPECT_EQ(scheduler.getResult<int>(id), 99);
	EXPECT_EQ(scheduler.getResult<int>(id), 99);
	EXPECT_EQ(call_count, 1);
}

TEST(TaskSchedulerTest, LambdaWithReferenceArg){
	TTaskScheduler scheduler;
	float num = 10.0f;
	float add_val = 5.0f;

	AddNumber adder{.value_to_add = add_val};

	auto id = scheduler.add([&](float& val_ref){
		val_ref += 2.0f;
		return val_ref;
	}, std::ref(num));

	scheduler.executeAll();
	EXPECT_FLOAT_EQ(scheduler.getResult<float>(id), 12.0f);
	EXPECT_FLOAT_EQ(num, 12.0f);
}

TEST(TaskSchedulerTest, MethodPointerWithReferenceArg){
	TTaskScheduler scheduler;
	float num = 100.0f;
	AddNumber adder{.value_to_add = 25.0f};

	auto id = scheduler.add(&AddNumber::add, adder, std::ref(num));

	scheduler.executeAll();
	EXPECT_FLOAT_EQ(scheduler.getResult<float>(id), 125.0f);
	EXPECT_FLOAT_EQ(num, 125.0f);
}

TEST(TaskSchedulerTest, LambdaWithMultipleReferenceArgs){
	TTaskScheduler scheduler;
	float val1 = 1.0f;
	float val2 = 2.0f;

	auto id = scheduler.add([](float& a, float& b) {
		a += 10.0f;
		b *= 2.0f;
		return a + b;
	}, std::ref(val1), std::ref(val2));

	scheduler.executeAll();
	EXPECT_FLOAT_EQ(scheduler.getResult<float>(id), (1.0f + 10.0f) + (2.0f * 2.0f));
	EXPECT_FLOAT_EQ(val1, 11.0f);
	EXPECT_FLOAT_EQ(val2, 4.0f);
}

TEST(TaskSchedulerTest, LambdaWithMultipleReferenceArgs2){
	TTaskScheduler scheduler;
	float val1 = 267.27f;
	float val2 = 498.749f;

	auto id = scheduler.add([](float& a, float& b) {
		a += 100.239f;
		b *= 239.239f;
		return a + b;
	}, std::ref(val1), std::ref(val2));

	scheduler.executeAll();
	EXPECT_FLOAT_EQ(scheduler.getResult<float>(id), (267.27f + 100.239f) + (498.749f * 239.239f));
	EXPECT_FLOAT_EQ(val1, 367.509f);
	EXPECT_FLOAT_EQ(val2, 119320.21201100001f);
}

TEST(TaskSchedulerTest, EmptySchedulerExecution){
	TTaskScheduler scheduler;

	EXPECT_NO_THROW(scheduler.executeAll());
}
