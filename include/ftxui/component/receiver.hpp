// Copyright 2020 Arthur Sonzogni. All rights reserved.
// 本源代码的使用受 MIT 许可协议的约束，该协议可在 LICENSE 文件中找到。
#ifndef FTXUI_COMPONENT_RECEIVER_HPP_
#define FTXUI_COMPONENT_RECEIVER_HPP_

#include <algorithm>           // for copy, max
#include <atomic>              // for atomic, __atomic_base
#include <condition_variable>  // for condition_variable
#include <ftxui/util/warn_windows_macro.hpp>
#include <memory>   // for unique_ptr, make_unique
#include <mutex>    // for mutex, unique_lock
#include <queue>    // for queue
#include <utility>  // for move

namespace ftxui {

// 已弃用
//
// 用法:
//
// 初始化:
// ---------------
//
// auto receiver = MakeReceiver<std:string>();
// auto sender_1= receiver->MakeSender();
// auto sender_2 = receiver->MakeSender();
//
// 然后将发送器移到其他地方，可能在不同的线程中。
//
// 在生产者侧:
// ----------------------
// [thread 1] sender_1->Send("hello");
// [thread 2] sender_2->Send("world");
//
// 在消费者侧:
// ---------------------
// char c;
// while(receiver->Receive(&c)) // 只要有生产者存在，就返回 true。
//   print(c)
//
// 当没有更多发送器时，Receiver::Receive() 返回 true。

// clang-format off
// 已弃用:
template<class T> class SenderImpl;
// 已弃用:
template<class T> class ReceiverImpl;
// 已弃用:

// 已弃用:
template<class T> using Sender = std::unique_ptr<SenderImpl<T>>;
// 已弃用:
template<class T> using Receiver = std::unique_ptr<ReceiverImpl<T>>;
// 已弃用:
template<class T> Receiver<T> MakeReceiver();
// clang-format on

// ---- Implementation part ----

template <class T>
// 已弃用:
class SenderImpl {
 public:
  SenderImpl(const SenderImpl&) = delete;
  SenderImpl(SenderImpl&&) = delete;
  SenderImpl& operator=(const SenderImpl&) = delete;
  SenderImpl& operator=(SenderImpl&&) = delete;
  void Send(T t) { receiver_->Receive(std::move(t)); }
  ~SenderImpl() { receiver_->ReleaseSender(); }

  Sender<T> Clone() { return receiver_->MakeSender(); }

 private:
  friend class ReceiverImpl<T>;
  explicit SenderImpl(ReceiverImpl<T>* consumer) : receiver_(consumer) {}
  ReceiverImpl<T>* receiver_;
};

template <class T>
class ReceiverImpl {
 public:
  Sender<T> MakeSender() {
    std::unique_lock<std::mutex> lock(mutex_);
    senders_++;
    return std::unique_ptr<SenderImpl<T>>(new SenderImpl<T>(this));
  }
  ReceiverImpl() = default;

  bool Receive(T* t) {
    while (senders_ || !queue_.empty()) {
      std::unique_lock<std::mutex> lock(mutex_);
      if (queue_.empty()) {
        notifier_.wait(lock);
      }
      if (queue_.empty()) {
        continue;
      }
      *t = std::move(queue_.front());
      queue_.pop();
      return true;
    }
    return false;
  }

  bool ReceiveNonBlocking(T* t) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (queue_.empty()) {
      return false;
    }
    *t = queue_.front();
    queue_.pop();
    return true;
  }

  bool HasPending() {
    std::unique_lock<std::mutex> lock(mutex_);
    return !queue_.empty();
  }

  bool HasQuitted() {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.empty() && !senders_;
  }

 private:
  friend class SenderImpl<T>;

  void Receive(T t) {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      queue_.push(std::move(t));
    }
    notifier_.notify_one();
  }

  void ReleaseSender() {
    senders_--;
    notifier_.notify_one();
  }

  std::mutex mutex_;
  std::queue<T> queue_;
  std::condition_variable notifier_;
  std::atomic<int> senders_{0};
};

template <class T>
Receiver<T> MakeReceiver() {
  return std::make_unique<ReceiverImpl<T>>();
}

}  // namespace ftxui

#endif  // FTXUI_COMPONENT_RECEIVER_HPP_