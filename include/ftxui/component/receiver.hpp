// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
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

// Obsolète
//
// Usage:
//
// Initialisation:
// ---------------
//
// auto receiver = MakeReceiver<std:string>();
// auto sender_1= receiver->MakeSender();
// auto sender_2 = receiver->MakeSender();
//
// Ensuite, déplacez les émetteurs ailleurs, potentiellement dans un thread différent.
//
// Côté producteur:
// ----------------------
// [thread 1] sender_1->Send("hello");
// [thread 2] sender_2->Send("world");
//
// Côté consommateur:
// ---------------------
// char c;
// while(receiver->Receive(&c)) // Retourne vrai tant qu'il y a un producteur.
//   print(c)
//
// Receiver::Receive() retourne vrai lorsqu'il n'y a plus d'émetteurs.

// clang-format off
// Obsolète:
template<class T> class SenderImpl;
// Obsolète:
template<class T> class ReceiverImpl;
// Obsolète:

// Obsolète:
template<class T> using Sender = std::unique_ptr<SenderImpl<T>>;
// Obsolète:
template<class T> using Receiver = std::unique_ptr<ReceiverImpl<T>>;
// Obsolète:
template<class T> Receiver<T> MakeReceiver();
// clang-format on

// ---- Partie implémentation ----

template <class T>
// Obsolète:
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
