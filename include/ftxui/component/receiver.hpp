// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
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

// 非推奨
//
// 使用法:
//
// 初期化:
// ---------------
//
// auto receiver = MakeReceiver<std:string>();
// auto sender_1= receiver->MakeSender();
// auto sender_2 = receiver->MakeSender();
//
// 次に、送信者を別の場所、場合によっては別のスレッドに移動します。
//
// プロデューサー側:
// ----------------------
// [スレッド 1] sender_1->Send("hello");
// [スレッド 2] sender_2->Send("world");
//
// コンシューマー側:
// ---------------------
// char c;
// while(receiver->Receive(&c)) // プロデューサーがいる限りtrueを返します。
//   print(c)
//
// Receiver::Receive() は、これ以上送信者がいない場合にtrueを返します。

// clang-format off
// 非推奨:
template<class T> class SenderImpl;
// 非推奨:
template<class T> class ReceiverImpl;
// 非推奨:

// 非推奨:
template<class T> using Sender = std::unique_ptr<SenderImpl<T>>;
// 非推奨:
template<class T> using Receiver = std::unique_ptr<ReceiverImpl<T>>;
// 非推奨:
template<class T> Receiver<T> MakeReceiver();
// clang-format on

// ---- 実装部分 ----

template <class T>
// 非推奨:
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