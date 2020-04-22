#include "../../src/util/lock.h"


void increment(int* count, Lock* lock) {
	lock->lock();
	for (int i = 0; i < 1000000; ++i) {
		(*count) += 1;
	}
	lock->unlock();
}

void increment2(int* count, Lock* lock, int* shared) {
	while (shared == 0) {
		lock->wait();
	}
	increment(count, lock);
}


void lockMutexTest() {
	Lock* l = new Lock();

	int count = 0;

	std::thread one (increment, &count, l);
	std::thread two (increment, &count, l);

	one.join();
	two.join();

	ASSERT_EQUALS(count, 2000000);

	delete l;

	exit(0);
}

TEST(LockTests, lockMutexTest) {ASSERT_EXIT_ZERO(lockMutexTest)}


void lockConditionalVariableTest() {
	Lock* l = new Lock();

	int count = 0;
	int shared = 0;

	std::thread one (increment2, &count, l, &shared);
	std::thread two (increment2, &count, l, &shared);

	shared = 1;
	l->notify_all();

	one.join();
	two.join();

	ASSERT_EQUALS(count, 2000000);

	delete l;

	exit(0);
}

TEST(LockTests, lockConditionalVariableTest) {ASSERT_EXIT_ZERO(lockConditionalVariableTest)}