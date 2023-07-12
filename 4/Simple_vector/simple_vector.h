#pragma once

#include <cstdlib>

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector() {
	  begin_ = nullptr;
	  end_ = nullptr;
	  capacity_ = 0;
	  size_ = 0;
  }
  explicit SimpleVector(size_t size) {
	  begin_ = new T[size];
	  end_ = begin_ + size;
	  capacity_ = size;
	  size_ = size;
  }
  ~SimpleVector() {
	  delete[] begin_;
  }

  T& operator[](size_t index) {
	  return begin_[index];
  }

  T* begin() {
	  return begin_;
  }
  T* end() {
	  return end_;
  }

  size_t Size() const {
	  return size_;
  }
  size_t Capacity() const {
	  return capacity_;
  }
  void PushBack(const T& value) {
	  if(Capacity() == 0) {
		  capacity_ = 1;
		  size_ = 1;
		  delete[] begin_;
		  begin_ = new T[capacity_];
		  begin_[0] = value;
		  end_ = begin_ + size_;
	  } else if (Size() == Capacity()) {
		  capacity_ = 2*capacity_;
		  T* new_begin = new T[capacity_];
		  for (size_t i = 0; i < Size(); ++i) {
			  new_begin[i] = begin_[i];
		  }
		  new_begin[size_] = value;
		  size_++;
		  delete[] begin_;
		  begin_ = new_begin;
		  end_ = begin_ + Size();
	  } else {
		  begin_[size_] = value;
		  size_++;
		  ++end_;
	  }
  }

private:
  // Добавьте поля для хранения данных вектора
  T* begin_;
  T* end_;
  size_t capacity_;
  size_t size_;
};
