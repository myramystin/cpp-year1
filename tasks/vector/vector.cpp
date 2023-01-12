#include "vector.h"
#include <algorithm>
#include <new>

Vector::Vector(size_t size) {
    data_ = new ValueType[size];
    size_ = size;
    capacity_ = size;
    for (size_t i = 0U; i < size; ++i) {
        data_[i] = 0;
    }
}

Vector::~Vector() {
    delete[] data_;
}

Vector::Vector(const std::initializer_list<ValueType>& list) {
    size_ = list.size();
    data_ = new ValueType[size_];
    capacity_ = size_;
    for (SizeType i = 0U; i < list.size(); ++i) {
        data_[i] = *(list.begin() + i);
    }
}

Vector::Vector(const Vector& other) {
    *this = other;
}

Vector::SizeType Vector::Size() const {
    return size_;
}

Vector::SizeType Vector::Capacity() const {
    return capacity_;
}

Vector::Vector() {
    data_ = nullptr;
    size_ = 0U;
    capacity_ = 0U;
}

Vector::ValueType& Vector::operator[](size_t position) {
    return data_[position];
}

Vector::ValueType Vector::operator[](size_t position) const {
    return data_[position];
}

void Vector::PopBack() {
    data_[size_ - 1U] = 0;
    --size_;
}

void Vector::Clear() {
    for (size_t i = 0U; i < size_; ++i) {
        data_[i] = 0;
    }
    size_ = 0;
}

void Vector::PushBack(const Vector::ValueType& new_element) {
    if (capacity_ == 0) {
        Reserve(1U);
    } else if (capacity_ <= size_) {
        Reserve(2U * size_);
    }
    if (size_ < capacity_) {
        ++size_;
        data_[size_ - 1U] = new_element;
    }
}

const Vector::ValueType* Vector::Data() const {
    return data_;
}

void Vector::Swap(Vector& other) {
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
}

void Vector::Reserve(Vector::SizeType new_capacity) {
    if (new_capacity > capacity_) {
        ValueType* data_copy = data_;
        data_ = new ValueType[new_capacity];
        for (size_t i = 0U; i < size_; ++i) {
            data_[i] = data_copy[i];
        }
        capacity_ = new_capacity;
        delete[] data_copy;
    }
}

Vector::Iterator Vector::Begin() {
    return Vector::Iterator(&data_[0]);
}

Vector::Iterator Vector::begin() {
    return Begin();
}

Vector::Iterator Vector::End() {
    return Vector::Iterator(&data_[size_]);
}

Vector::Iterator Vector::end() {
    return End();
}

bool Vector::operator==(const Vector& other) const {
    if (size_ != other.size_) {
        return false;
    }
    for (size_t i = 0; i < size_; ++i) {
        if (data_[i] != other.data_[i]) {
            return false;
        }
    }
    return true;
}

bool Vector::operator!=(const Vector& other) const {
    return !(*this == other);
}

std::strong_ordering Vector::operator<=>(const Vector& other) const {
    SizeType min_size = std::min(other.size_, size_);
    for (SizeType i = 0U; i < min_size; ++i) {
        if (data_[i] < other.data_[i]) {
            return std::strong_ordering::less;
        } else if (data_[i] > other.data_[i]) {
            return std::strong_ordering::greater;
        }
    }
    if (min_size == size_) {
        return std::strong_ordering::less;
    } else if (min_size == other.size_) {
        return std::strong_ordering::greater;
    } else {
        return std::strong_ordering::equal;
    }
}
Vector& Vector::operator=(const Vector& other) {
    if (this == &other) {
        return *this;
    }
    ValueType* new_data = new ValueType[other.capacity_];
    for (SizeType i = 0U; i < other.size_; ++i) {
        new_data[i] = other.data_[i];
    }
    std::swap(new_data, data_);
    delete[] new_data;
    size_ = other.Size();
    capacity_ = other.capacity_;
    return *this;
}

Vector::Iterator::Iterator(Vector::ValueType* pointer) : ptr_(pointer) {
}

Vector::Iterator& Vector::Iterator::operator++() {
    ++ptr_;
    return *this;
}

Vector::Iterator Vector::Iterator::operator++(int) {
    auto return_value = Vector::Iterator(ptr_);
    ++ptr_;
    return return_value;
}

bool Vector::Iterator::operator!=(const Vector::Iterator& other) const {
    return ptr_ != other.ptr_;
}

Vector::ValueType& Vector::Iterator::operator*() const {
    return *ptr_;
}

Vector::DifferenceType Vector::Iterator::operator-(Vector::Iterator other) {
    return ptr_ - other.ptr_;
}

bool Vector::Iterator::operator==(const Vector::Iterator& other) const {
    return ptr_ == other.ptr_;
}

std::strong_ordering Vector::Iterator::operator<=>(const Vector::Iterator& other) const {
    return ptr_ <=> other.ptr_;
}

Vector::ValueType* Vector::Iterator::operator->() const {
    return ptr_;
}

Vector::Iterator& Vector::Iterator::operator=(Vector::Iterator other) {
    ptr_ = other.ptr_;
    return *this;
}

Vector::Iterator& Vector::Iterator::operator--() {
    --ptr_;
    return *this;
}

Vector::Iterator Vector::Iterator::operator--(int) {
    auto return_value = Vector::Iterator(ptr_);
    --ptr_;
    return return_value;
}

Vector::Iterator Vector::Iterator::operator+(Vector::DifferenceType shift) {
    auto return_value = Vector::Iterator(ptr_ + shift);
    return return_value;
}

Vector::Iterator& Vector::Iterator::operator+=(Vector::DifferenceType shift) {
    ptr_ += shift;
    return *this;
}

Vector::Iterator& Vector::Iterator::operator-=(Vector::DifferenceType shift) {
    ptr_ -= shift;
    return *this;
}
