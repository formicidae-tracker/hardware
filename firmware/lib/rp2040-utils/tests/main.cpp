#include <utils/Defer.hpp>

#include <utils/Queue.hpp>

int main() {
	Queue<int, 8, true> foo;
	foo.EmplaceBlocking(0);
	int res;
	foo.RemoveBlocking(res);
	return res;
}
