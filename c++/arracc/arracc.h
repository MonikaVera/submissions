#ifndef ARRACC
#define ARRACC
#include <vector>

template <typename T, class BinPred=std::plus<T>>
class array_accumulator {
private:
  std::vector<T> arrOld;
  std::vector<T*> arrays;
  std::vector<int> sizes;
public:
  array_accumulator(T* arr, int size) {
      arrOld.push_back(arr[0]);
      for(int i=1; i<size; i++) {
        arrOld.push_back(arr[i]);
        arr[i]= BinPred{}(arr[i-1], arr[i]);
      }
      arrays.push_back(arr);
      sizes.push_back(size);
    }
  ~array_accumulator() {
    while(!arrOld.empty()) {
      T* curr=arrays.back();
      for(int j=sizes.back()-1; j>=0; j--) {
        curr[j]=arrOld.back();
        arrOld.pop_back();
      }
      arrays.pop_back();
      sizes.pop_back();
    }
  }
  void add(T* arrAdd, int size_2 ) {
    arrOld.push_back(arrAdd[0]);
    arrAdd[0]+=(arrays.back())[sizes.back()-1];
    for(int i=1; i<size_2; i++) {
      arrOld.push_back(arrAdd[i]);
      arrAdd[i]=BinPred{}(arrAdd[i-1], arrAdd[i]);
    }
    arrays.push_back(arrAdd);
    sizes.push_back(size_2);
  }
  unsigned int size() const{
    return arrOld.size();
  }
};
#endif

//g++ -fsanitize=address,leak,undefined -O3 -Wall -Wextra -Werror main.cpp
//clang-tidy-12 -header-filter='.*' main.cpp --
//g++ -O3 -Wall -Wextra -Werror main.cpp
//valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./a.out