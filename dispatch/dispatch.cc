#include <atomic>
#include <chrono>

#include <string>
#include <sstream>

#include "spdlog/spdlog.h"

#include "dispatch/dispatch.h"

namespace io::supr::mtfe {
	dispatcher::dispatcher(std::string name, size_type size):
		m_name_{name}, m_threads_(size)
		{
			for(size_type s = 0; s < size; ++s) {
				m_threads_[s] = std::thread(
						&dispatcher::dispatch, this);
			}
		}

	void dispatcher::stop() {
		m_stop_.store(true, std::memory_order::memory_order_seq_cst);
	}

	void dispatcher::dispatch() {
		std::stringstream ss;
		ss << std::this_thread::get_id();
		auto tid = ss.str();
		do {
			item_type item;
			if(m_q_.dequeue_for(item, std::chrono::milliseconds(0))) {
				spdlog::info("{}: Dequeued: {}", tid, item);
			}
		} while(!m_stop_.load() || !m_q_.empty());
	}

	dispatcher::~dispatcher() {
		stop();
		wait();
	}

	void dispatcher::wait() {
		for(size_type s = 0; s < m_threads_.size(); ++s) {
			m_threads_[s].join();
		}
	}

	void dispatcher::enqueue(item_type&& item) {
		m_q_.enqueue(std::forward<item_type>(item));
	}

	bool dispatcher::empty() {
		return m_q_.empty();
	}
}
