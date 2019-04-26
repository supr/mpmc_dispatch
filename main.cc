#include <memory>
#include <chrono>
#include <thread>
#include <vector>

#include "dispatch/dispatch.h"

io::supr::mtfe::dispatcher* g_dispatcher = nullptr;

void producer(unsigned int idx = 0) {
	for(auto i = (0 + 1*idx); i < (10 + idx*10); ++i) {
	io::supr::mtfe::dispatcher::item_type item(i);
	g_dispatcher->enqueue(std::move(item));
	}
}

int main(int argc, char** argv) {
	std::vector<std::thread> threads;
	g_dispatcher = new io::supr::mtfe::dispatcher("global_dispatcher", 4);
	threads.push_back(std::thread(producer, 0));
	threads.push_back(std::thread(producer, 1));
	threads.push_back(std::thread(producer, 2));
	threads.push_back(std::thread(producer, 3));

	for(auto& thr : threads) {
		thr.join();
	}

	delete g_dispatcher;

	return 0;
}
