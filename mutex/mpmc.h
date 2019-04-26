#pragma once

#include <cstddef>
#include <array>
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace io::supr::mtfe::mutex {
	template<typename T, std::size_t N>
	class circular_queue {
		private:
			std::array<T, N + 1> m_arr_;
			typename std::array<T, N>::size_type  m_read_pos_{0};
			typename std::array<T, N>::size_type m_write_pos_{0};
			std::size_t m_overrun_counter_{0};

		public:
			using item_type = T;
			using size_type = std::size_t;

			size_type size() const {
				return N;
			}

			void push_back(T&& item) {
				m_arr_[m_write_pos_] = std::move(item);
				m_write_pos_ = (m_write_pos_ + 1) % (N + 1);

				if(m_write_pos_ == m_read_pos_) {
					m_read_pos_ = (m_read_pos_ + 1) % (N + 1);
					++m_overrun_counter_;
				}
			}

			void pop_front(T& popped_item) {
				popped_item = std::move(m_arr_[m_read_pos_]);
				m_read_pos_ = (m_read_pos_ + 1) % (N + 1);
			}

			bool empty() {
				return m_read_pos_ == m_write_pos_;
			}

			bool full() {
				return ((m_write_pos_ + 1) % (N + 1)) == m_read_pos_;
			}

			size_type overrun_counter() const {
				return m_overrun_counter_;
			}
	};

	template<typename T, std::size_t N>
	class mpmc {
		private:
			circular_queue<T,N> m_cq_;
			std::mutex m_mtx_;
			std::condition_variable m_cv_push_;
			std::condition_variable m_cv_pop_;
			
		public:
			using item_type = T;
			using size_type = std::size_t;

			size_type overrun_counter() {
				std::unique_lock<std::mutex> lk(m_mtx_);
				return m_cq_.overrun_counter();
			}

			void enqueue(T&& item) {
				{
				std::unique_lock<std::mutex> lk(m_mtx_);
				m_cv_pop_.wait(lk, [this] { return !this->m_cq_.full(); });
				m_cq_.push_back(std::move(item));
				}
				m_cv_push_.notify_one();
			}

			bool dequeue_for(T& item, std::chrono::milliseconds wait_duration) {
				std::unique_lock<std::mutex> lk(m_mtx_);
				if(!m_cv_push_.wait_for(lk, wait_duration, [this] { return !this->m_cq_.empty(); })) {
					return false;
				}
				m_cq_.pop_front(item);
				m_cv_pop_.notify_one();
				return true;
			}

			size_type overrun_counter() const {
				return m_cq_.overrun_counter();
			}

			bool empty() {
				return m_cq_.empty();
			}
	};
}
