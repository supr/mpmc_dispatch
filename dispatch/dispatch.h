#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include "mutex/mpmc.h"

namespace io::supr::mtfe {
	class dispatcher {
		private:
			io::supr::mtfe::mutex::mpmc<std::size_t, 64> m_q_;
			std::string m_name_;
			std::vector<std::thread> m_threads_;
			std::atomic_bool m_stop_{false};
		public:
			using item_type = std::size_t;
			using size_type = std::size_t;

			dispatcher() = delete;
			dispatcher(const dispatcher&) = delete;
			dispatcher(dispatcher&&) = delete;
			dispatcher& operator=(const dispatcher&) = delete;
			dispatcher& operator=(dispatcher&&) = delete;
			~dispatcher();
			
			explicit dispatcher(std::string, size_type);

			void dispatch();

			void stop();

			void wait();

			bool empty();

			void enqueue(item_type&& item);
	};
}
