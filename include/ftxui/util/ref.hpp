// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_UTIL_REF_HPP
#define FTXUI_UTIL_REF_HPP

#include <ftxui/screen/string.hpp>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace ftxui {

/// @brief 一個適配器。擁有或引用一個不可變的物件。
template <typename T>
class ConstRef {
 public:
  ConstRef() = default;
  ConstRef(T t) : variant_(std::move(t)) {}  // NOLINT
  ConstRef(const T* t) : variant_(t) {}      // NOLINT
  ConstRef& operator=(ConstRef&&) noexcept = default;
  ConstRef(const ConstRef<T>&) = default;
  ConstRef(ConstRef<T>&&) noexcept = default;
  ~ConstRef() = default;

  // 建立一個「可重新設定」的引用
  ConstRef<T>& operator=(const ConstRef<T>&) = default;

  // 存取器：
  const T& operator()() const { return *Address(); }
  const T& operator*() const { return *Address(); }
  const T* operator->() const { return Address(); }

 private:
  std::variant<T, const T*> variant_ = T{};

  const T* Address() const {
    return std::holds_alternative<T>(variant_) ? &std::get<T>(variant_)
                                               : std::get<const T*>(variant_);
  }
};

/// @brief 一個適配器。擁有或引用一個可變的物件。
template <typename T>
class Ref {
 public:
  Ref() = default;
  Ref(T t) : variant_(std::move(t)) {}  // NOLINT
  Ref(T* t) : variant_(t) {}            // NOLINT
  ~Ref() = default;
  Ref& operator=(Ref&&) noexcept = default;
  Ref(const Ref<T>&) = default;
  Ref(Ref<T>&&) noexcept = default;

  // 建立一個「可重新設定」的引用。
  Ref<T>& operator=(const Ref<T>&) = default;

  // 存取器：
  T& operator()() { return *Address(); }
  T& operator*() { return *Address(); }
  T* operator->() { return Address(); }
  const T& operator()() const { return *Address(); }
  const T& operator*() const { return *Address(); }
  const T* operator->() const { return Address(); }

 private:
  std::variant<T, T*> variant_ = T{};

  const T* Address() const {
    return std::holds_alternative<T>(variant_) ? &std::get<T>(variant_)
                                               : std::get<T*>(variant_);
  }
  T* Address() {
    return std::holds_alternative<T>(variant_) ? &std::get<T>(variant_)
                                               : std::get<T*>(variant_);
  }
};

/// @brief 一個適配器。擁有或引用一個常數字串。為方便起見，此類別將多個可變字串轉換為共享表示。
class StringRef : public Ref<std::string> {
 public:
  using Ref<std::string>::Ref;

  StringRef(const wchar_t* ref)  // NOLINT
      : StringRef(to_string(std::wstring(ref))) {}
  StringRef(const char* ref)  // NOLINT
      : StringRef(std::string(ref)) {}
};

/// @brief 一個適配器。擁有或引用一個常數字串。為方便起見，此類別將多個不可變字串轉換為共享表示。
class ConstStringRef : public ConstRef<std::string> {
 public:
  using ConstRef<std::string>::ConstRef;

  ConstStringRef(const std::wstring* ref)  // NOLINT
      : ConstStringRef(to_string(*ref)) {}
  ConstStringRef(const std::wstring ref)  // NOLINT
      : ConstStringRef(to_string(ref)) {}
  ConstStringRef(const wchar_t* ref)  // NOLINT
      : ConstStringRef(to_string(std::wstring(ref))) {}
  ConstStringRef(const char* ref)  // NOLINT
      : ConstStringRef(std::string(ref)) {}
};

/// @brief 一個適配器。引用一個字串列表。
///
/// 支援的輸入：
/// - `std::vector<std::string>`
/// - `std::vector<std::string>*`
/// - `std::vector<std::wstring>*`
/// - `Adapter*`
/// - `std::unique_ptr<Adapter>`
class ConstStringListRef {
 public:
  // 使用您自己的適配器：
  class Adapter {
   public:
    Adapter() = default;
    Adapter(const Adapter&) = default;
    Adapter& operator=(const Adapter&) = default;
    Adapter(Adapter&&) = default;
    Adapter& operator=(Adapter&&) = default;
    virtual ~Adapter() = default;
    virtual size_t size() const = 0;
    virtual std::string operator[](size_t i) const = 0;
  };
  using Variant = std::variant<const std::vector<std::string>,    //
                               const std::vector<std::string>*,   //
                               const std::vector<std::wstring>*,  //
                               Adapter*,                          //
                               std::unique_ptr<Adapter>           //
                               >;

  ConstStringListRef() = default;
  ~ConstStringListRef() = default;
  ConstStringListRef& operator=(const ConstStringListRef&) = default;
  ConstStringListRef& operator=(ConstStringListRef&&) = default;
  ConstStringListRef(ConstStringListRef&&) = default;
  ConstStringListRef(const ConstStringListRef&) = default;

  ConstStringListRef(std::vector<std::string> value)  // NOLINT
  {
    variant_ = std::make_shared<Variant>(value);
  }
  ConstStringListRef(const std::vector<std::string>* value)  // NOLINT
  {
    variant_ = std::make_shared<Variant>(value);
  }
  ConstStringListRef(const std::vector<std::wstring>* value)  // NOLINT
  {
    variant_ = std::make_shared<Variant>(value);
  }
  ConstStringListRef(Adapter* adapter)  // NOLINT
  {
    variant_ = std::make_shared<Variant>(adapter);
  }
  template <typename AdapterType>
  ConstStringListRef(std::unique_ptr<AdapterType> adapter)  // NOLINT
  {
    variant_ = std::make_shared<Variant>(
        static_cast<std::unique_ptr<Adapter>>(std::move(adapter)));
  }

  size_t size() const {
    return variant_ ? std::visit(SizeVisitor(), *variant_) : 0;
  }

  std::string operator[](size_t i) const {
    return variant_ ? std::visit(IndexedGetter(i), *variant_) : "";
  }

 private:
  struct SizeVisitor {
    size_t operator()(const std::vector<std::string>& v) const {
      return v.size();
    }
    size_t operator()(const std::vector<std::string>* v) const {
      return v->size();
    }
    size_t operator()(const std::vector<std::wstring>* v) const {
      return v->size();
    }
    size_t operator()(const Adapter* v) const { return v->size(); }
    size_t operator()(const std::unique_ptr<Adapter>& v) const {
      return v->size();
    }
  };

  struct IndexedGetter {
    IndexedGetter(size_t index)  // NOLINT
        : index_(index) {}
    size_t index_;
    std::string operator()(const std::vector<std::string>& v) const {
      return v[index_];
    }
    std::string operator()(const std::vector<std::string>* v) const {
      return (*v)[index_];
    }
    std::string operator()(const std::vector<std::wstring>* v) const {
      return to_string((*v)[index_]);
    }
    std::string operator()(const Adapter* v) const { return (*v)[index_]; }
    std::string operator()(const std::unique_ptr<Adapter>& v) const {
      return (*v)[index_];
    }
  };

  std::shared_ptr<Variant> variant_;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_UTIL_REF_HPP */