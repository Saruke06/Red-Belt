#pragma once

#include <array>
#include <stdexcept>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0) : size(a_size) {
    	if (a_size > N)
    		throw invalid_argument("Size > capacity\n");
    }

    T& operator[](size_t index) {
    	return data[index];
    }
    const T& operator[](size_t index) const {
    	data[index];
    }

    typename array<T, N>::iterator begin() {
    	return data.begin();
    }
    typename array<T, N>::iterator end() {
    	return data.begin() + size;
    }
    typename array<T, N>::const_iterator begin() const {
    	return data.begin();
    }
    typename array<T, N>::const_iterator end() const {
    	return data.begin() + size;
    }

    size_t Size() const {
    	return size;
    }
    size_t Capacity() const {
    	return N;
    }

    void PushBack(const T& value) {
    	if (size == N) {
    		throw overflow_error("Size == capacity\n");
    	} else {
    		data[size++] = value;
    	}
    }
    T PopBack() {
    	if (size == 0) {
    		throw underflow_error("Size == 0\n");
    	} else {
    		return data[--size];
    	}
    }

private:
    array<T, N> data;
    size_t size = 0;
};

