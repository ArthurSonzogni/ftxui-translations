// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_UTIL_REF_HPP
#define FTXUI_UTIL_REF_HPP

#include <ftxui/screen/string.hpp>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "ftxui/util/export.hpp"

namespace ftxui {

/// @brief Un adaptador. Posee o referencia un objeto inmutable.
template <typename T>
class ConstRef {
 public:
  ConstRef() = default;

  // Constructores propietarios:
  ConstRef(T t) : variant_(std::move(t)) {}  // NOLINT

  // Constructores de referencia:
  ConstRef(const T* t) : variant_(t) {}  // NOLINT

  ConstRef& operator=(ConstRef&&) noexcept = default;
  ConstRef(const ConstRef<T>&) = default;
  ConstRef(ConstRef<T>&&) noexcept = default;
  ~ConstRef() = default;

  // Crea una referencia "reajustable"
  ConstRef<T>& operator=(const ConstRef<T>&) = default;

  // Accesores:
  const T& operator()() const { return *Address(); }
  const T& operator*() const { return *Address(); }
  const T* operator->() const { return Address(); }

 private:
  std::variant<T, const T*> variant_ = T{};

  const T* Address() const {
    if (const T* t = std::get_if<T>(&variant_)) {
      return t;
    }
    return std::get<const T*>(variant_);
  }
};

/// @brief Un adaptador. Posee o referencia un objeto mutable.
template <typename T>
class Ref {
 public:
  Ref() = default;

  // Constructores propietarios:
  Ref(T t)
      : variant_(std::move(t)) {}  // NOLINT
                                   //
  // Referencing constructors:
  Ref(T* t)
      : variant_(t) {}  // NOLINT
                        //
  ~Ref() = default;
  Ref& operator=(Ref&&) noexcept = default;
  Ref(const Ref<T>&) = default;
  Ref(Ref<T>&&) noexcept = default;

  // Crea una referencia "reajustable".
  Ref<T>& operator=(const Ref<T>&) = default;

  // Accesores:
  T& operator()() { return *Address(); }
  T& operator*() { return *Address(); }
  T* operator->() { return Address(); }
  const T& operator()() const { return *Address(); }
  const T& operator*() const { return *Address(); }
  const T* operator->() const { return Address(); }

 private:
  std::variant<T, T*> variant_ = T{};

  const T* Address() const {
    if (const T* t = std::get_if<T>(&variant_)) {
      return t;
    }
    return std::get<T*>(variant_);
  }
  T* Address() {
    if (T* t = std::get_if<T>(&variant_)) {
      return t;
    }
    return std::get<T*>(variant_);
  }
};

/// @brief Un adaptador. Posee o referencia una cadena constante. Para mayor comodidad, esta
/// clase convierte múltiples cadenas mutables hacia una representación compartida.
class FTXUI_EXPORT(SCREEN) StringRef : public Ref<std::string> {
 public:
  using Ref<std::string>::Ref;

  // Constructores propietarios:
  StringRef(const wchar_t* ref)  // NOLINT
      : StringRef(to_string(std::wstring(ref))) {}
  StringRef(const char* ref)  // NOLINT
      : StringRef(std::string(ref)) {}
  StringRef(std::string_view ref)  // NOLINT
      : StringRef(std::string(ref)) {}
  StringRef(std::wstring_view ref)  // NOLINT
      : StringRef(to_string(ref)) {}
};

/// @brief Un adaptador. Posee o referencia una cadena constante. Para mayor comodidad, esta
/// clase convierte múltiples cadenas inmutables hacia una representación compartida.
class FTXUI_EXPORT(SCREEN) ConstStringRef : public ConstRef<std::string> {
 public:
  using ConstRef<std::string>::ConstRef;

  // Constructores de referencia:
  ConstStringRef(const std::wstring* ref)  // NOLINT
      : ConstStringRef(to_string(*ref)) {}

  // Constructores propietarios:
  ConstStringRef(const std::wstring ref)  // NOLINT
      : ConstStringRef(to_string(ref)) {}
  ConstStringRef(std::wstring_view ref)  // NOLINT
      : ConstStringRef(to_string(ref)) {}
  ConstStringRef(const wchar_t* ref)  // NOLINT
      : ConstStringRef(to_string(std::wstring(ref))) {}
  ConstStringRef(const char* ref)  // NOLINT
      : ConstStringRef(std::string(ref)) {}
  ConstStringRef(std::string_view ref)  // NOLINT
      : ConstStringRef(std::string(ref)) {}
};

/// @brief Un adaptador. Referencia una lista de cadenas.
///
/// Entradas admitidas:
/// - `std::vector<std::string>`
/// - `std::vector<std::string>*`
/// - `std::vector<std::wstring>*`
/// - `Adapter*`
/// - `std::unique_ptr<Adapter>`
class FTXUI_EXPORT(SCREEN) ConstStringListRef {
 public:
  // Trae tu propio adaptador:
  class Adapter {
   public:
    Adapter() = default;
    Adapter(const Adapter&) = default;
    Adapter& operator=(const Adapter&) = default;
    Adapter(Adapter&&) = default;
    Adapter& operator=(Adapter&&) = default;
    virtual ~Adapter() = default;
    virtual size_t size() const = 0;
    virtual std::string_view operator[](size_t i) const = 0;
  };
  using Variant = std::variant<const std::vector<std::string>,        //
                               const std::vector<std::string>*,       //
                               const std::vector<std::string_view>,   //
                               const std::vector<std::string_view>*,  //
                               const std::vector<std::wstring>*,      //
                               Adapter*,                              //
                               std::unique_ptr<Adapter>               //
                               >;

  ConstStringListRef() = default;
  ~ConstStringListRef() = default;
  ConstStringListRef& operator=(const ConstStringListRef&) = default;
  ConstStringListRef& operator=(ConstStringListRef&&) = default;
  ConstStringListRef(ConstStringListRef&&) = default;
  ConstStringListRef(const ConstStringListRef&) = default;

  ConstStringListRef(std::vector<std::string> value) {  // NOLINT
    variant_ = std::make_shared<Variant>(value);
  }
  ConstStringListRef(const std::vector<std::string>* value) {  // NOLINT
    variant_ = std::make_shared<Variant>(value);
  }
  ConstStringListRef(std::vector<std::string_view> value) {  // NOLINT
    variant_ = std::make_shared<Variant>(value);
  }
  ConstStringListRef(const std::vector<std::string_view>* value) {  // NOLINT
    variant_ = std::make_shared<Variant>(value);
  }
  ConstStringListRef(const std::vector<std::wstring>* value) {  // NOLINT
    variant_ = std::make_shared<Variant>(value);
  }
  ConstStringListRef(Adapter* adapter) {  // NOLINT
    variant_ = std::make_shared<Variant>(adapter);
  }
  template <typename AdapterType>
  ConstStringListRef(std::unique_ptr<AdapterType> adapter) {  // NOLINT
    variant_ = std::make_shared<Variant>(
        static_cast<std::unique_ptr<Adapter>>(std::move(adapter)));
  }

  size_t size() const {
    return variant_ ? std::visit(SizeVisitor(), *variant_) : 0;
  }

  std::string_view operator[](size_t i) const {
    return variant_ ? std::visit(IndexedGetter{i}, *variant_) : "";
  }

 private:
  struct IndexedGetter {
    size_t i;
    std::string_view operator()(const std::vector<std::string>& v) const {
      return v[i];
    }
    std::string_view operator()(const std::vector<std::string>* v) const {
      return (*v)[i];
    }
    std::string_view operator()(const std::vector<std::string_view>& v) const {
      return v[i];
    }
    std::string_view operator()(const std::vector<std::string_view>* v) const {
      return (*v)[i];
    }
    std::string_view operator()(
        [[maybe_unused]] const std::vector<std::wstring>* v) const {
      return "";  // Corrección temporal: No se puede devolver una vista a una conversión
                  // temporal.
    }
    std::string_view operator()(Adapter* v) const { return (*v)[i]; }
    std::string_view operator()(const std::unique_ptr<Adapter>& v) const {
      return (*v)[i];
    }
  };

  struct SizeVisitor {
    size_t operator()(const std::vector<std::string>& v) const {
      return v.size();
    }
    size_t operator()(const std::vector<std::string>* v) const {
      return v->size();
    }
    size_t operator()(const std::vector<std::string_view>& v) const {
      return v.size();
    }
    size_t operator()(const std::vector<std::string_view>* v) const {
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

  std::shared_ptr<Variant> variant_;
};

}  // namespace ftxui

#endif /* fin de la guarda de inclusión: FTXUI_UTIL_REF_HPP */
