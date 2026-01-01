#include <cstddef>
#include <iostream>
#include <type_traits>
#include <utility>
#include <variant>

template <typename... Types> struct get_first_type;

template <typename Head, typename... Tail>
struct get_first_type<Head, Tail...> {
  using type = Head;
};

// find index of active type in parametr set
template <typename T, typename... Types> struct get_index_type;

// base case
template <typename T, typename... Types> struct get_index_type<T, T, Types...> {
  static constexpr size_t value = 0;
};
template <typename T, typename Head, typename... Tail>
struct get_index_type<T, Head, Tail...> {
  static constexpr size_t value = 1 + get_index_type<T, Tail...>::value;
};

template <typename... Types> union variant_storage;

// base case for variant_storage
template <typename Type> union variant_storage<Type> {
  template <typename Other>
  variant_storage(std::integral_constant<size_t, 0>, Other &&value)
      : value(std::forward<Other>(value)) {}

  template <size_t INDEX> auto &get() { return value; }

  void destroy(size_t /*index*/) { value.~Type(); }
  Type value;
};

// general case for variant_storage
template <typename Type, typename... Types>
union variant_storage<Type, Types...> {

  template <typename Other>
  variant_storage(std::integral_constant<size_t, 0>, Other &&val)
      : value(std::forward<Other>(val)) {}

  template <std::size_t INDEX, typename Other>
  variant_storage(std::integral_constant<size_t, INDEX>, Other &&val)
      : rest(std::integral_constant<size_t, INDEX - 1>{},
             std::forward<Other>(val)) {}

  template <size_t INDEX> auto &get() {
    if constexpr (INDEX == 0)
      return value;
    else
      return rest.template get<INDEX - 1>();
  }

  void destroy(size_t index) {
    if (index == 0)
      value.~Type();
    else
      rest.destroy(index - 1);
  }

  ~variant_storage() {}

  Type value;
  variant_storage<Types...> rest;
};
template <typename... Types> class variant {
public:
  using first_type = typename get_first_type<Types...>::type;

  variant()
      : index(0), storage(std::integral_constant<size_t, 0>{}, first_type{}) {}

  template <typename Type>
  variant(Type &&value)
      : index(get_index_type<Type, Types...>::value),
        storage(std::integral_constant<size_t,
                                       get_index_type<Type, Types...>::value>{},
                std::forward<Type>(value)) {}

  ~variant() { storage.destroy(index); }

  template <size_t INDEX> auto &get() { return storage.template get<INDEX>(); }

  // assignment operator with template acceptance
  template <typename Type> auto &operator=(Type &&value) {
    constexpr size_t ind = get_index_type<Type, Types...>::value;
    storage.destroy(index);
    index = ind;
    get<ind>() = std::move(value);
    return *this;
    int end = 0;
  }

private:
  size_t index;
  variant_storage<Types...> storage;
};

int main() {
  variant<int, std::string, size_t> var;

#if 0
  /*
   * std::variant is a type-safe union, at any given moment,
   * it stores only one of the listed types.
   */
  std::variant<int, std::string, size_t> var;

  var = int(1000);
  /*
   * safe as active alternative is int (index 0)
   * std::get<I> req that I matches current active index
   * */
  std::cout << "int: " << std::get<0>(var) << std::endl;

  var = std::string("ann");
  std::cout << "string: " << std::get<1>(var) << std::endl;

  var = size_t(1234);
  std::cout << "size_t: " << std::get<2>(var) << std::endl;
#endif
}
