#include "../../src/network/pollfds.h"


void pollfdsNotCompletelyTestedNotice() {
	exit(0);
}

TEST(PollfdsTests, pollfdsNotCompletelyTestedNotice) {ASSERT_EXIT_ZERO(pollfdsNotCompletelyTestedNotice)}


void pollfdsConstructorDestructorTest() {
	Pollfds pfds1 = Pollfds();
	Pollfds* pfds2 = new Pollfds();

	delete pfds2;

	exit(0);
}

TEST(PollfdsTests, pollfdsConstructorDestructorTest) {ASSERT_EXIT_ZERO(pollfdsConstructorDestructorTest)}