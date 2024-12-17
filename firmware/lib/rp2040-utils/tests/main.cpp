#include "Defer.hpp"

#include "Queue.hpp"

int main() {
	Queue<int, 8, true> foo;
	foo.EmplaceBlocking(0);
	int res;
	foo.RemoveBlocking(res);
	return res;
}
