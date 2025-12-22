#include <cmath>
#include <cstddef>
#include <iostream>
#include <type_traits>
#include <variant>

template <typename... Types> struct get_first_type;

template <typename Head, typename... Tail>
struct get_first_type<Head, Tail...> {
  using type = Head;
};

template <typename... Types> class variant {
public:
  using first_type = typename get_first_type<Types...>::type;

  variant()
      : index(0), storage(std::integral_constant<size_t, 0>{}, first_type{}) {}

private:
  size_t index;
  variant_storage<Types...> storage;
};

int main() {

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
}
