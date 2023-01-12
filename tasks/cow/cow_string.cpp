#include "cow_string.h"
#include <stdexcept>

CowString::ProxyChar::ProxyChar(size_t pos, CowString& owner) : pos_(pos), owner_(owner) {
}

void CowString::ProxyChar::operator=(const char& c) {
    if ((*owner_.data_.GetData())[pos_] != c && *owner_.GetInnerRefsCount() > 1) {
        owner_.data_ = *owner_.data_.GetData();
        owner_.data_.SetAt(pos_, c);
    } else if ((*owner_.data_.GetData())[pos_] != c) {
        owner_.data_.SetAt(pos_, c);
    }
}

CowString::ProxyChar::operator char() const {
    return (*owner_.data_.GetData())[pos_];
}

CowString::InnerString::InnerString(std::string_view data) {
    data_ = new std::string(data);
    try {
        refs_count_ = new size_t(1U);
    } catch (...) {
            delete data_;
            throw std::bad_alloc();
    }
}

CowString::InnerString::InnerString(const InnerString& other) {
    data_ = other.data_;
    refs_count_ = other.refs_count_;
    ++(*refs_count_);
}

CowString::InnerString::~InnerString() {
    if (*refs_count_ > 1U) {
        --(*refs_count_);
    } else {
        delete data_;
        delete refs_count_;
    }
}

void CowString::InnerString::IncreaseRefsCount() {
    ++(*refs_count_);
}

void CowString::InnerString::DecreaseRefsCount() {
    if (*refs_count_ > 1U) {
        --(*refs_count_);
    } else {
        delete refs_count_;
        delete data_;
    }
}

std::string* CowString::InnerString::GetData() const {
    return data_;
}

size_t CowString::InnerString::GetDataSize() const {
    return data_->size();
}

size_t* CowString::InnerString::GetRefsCount() const {
    return refs_count_;
}

void CowString::InnerString::SetRefsCount(size_t* count) {
    refs_count_ = count;
}

void CowString::InnerString::SetData(std::string* data) {
    data_ = data;
}

void CowString::InnerString::SetAt(size_t pos, const char& c) {
    (*data_)[pos] = c;
}

CowString::InnerString& CowString::InnerString::operator=(std::string_view data) {
    DecreaseRefsCount();
    std::string* new_data_ptr = new std::string (data);
    try {
        SetRefsCount(new size_t(1u));
        SetData(new_data_ptr);
    } catch (...) {
        delete new_data_ptr;
        throw  std::bad_alloc();
    }
    return *this;
}

CowString::InnerString& CowString::InnerString::operator=(const CowString::InnerString& other) {
    DecreaseRefsCount();
    SetData(other.GetData());
    SetRefsCount(other.GetRefsCount());
    IncreaseRefsCount();
    return *this;
}

CowString::CowStringIterator::CowStringIterator(size_t pos, CowString& owner) : pos_(pos), owner_(owner) {
}

CowString::ProxyChar CowString::CowStringIterator::operator*() {
    return ProxyChar(pos_, owner_);
}

CowString::ProxyChar CowString::CowStringIterator::operator->() {
    return ProxyChar(pos_, owner_);
}

char CowString::CowStringIterator::operator*() const {
    return owner_[pos_];
}

char CowString::CowStringIterator::operator->() const {
    return owner_[pos_];
}

bool CowString::CowStringIterator::operator!=(const CowString::CowStringIterator& other) {
    return &((*owner_.data_.GetData())[pos_]) != &((*other.owner_.data_.GetData())[other.pos_]);
}

CowString::CowStringIterator& CowString::CowStringIterator::operator++() {
    ++pos_;
    return *this;
}

CowString::ConstCowStringIterator::ConstCowStringIterator(size_t pos, const CowString& owner)
    : pos_(pos), owner_(owner) {
}

char CowString::ConstCowStringIterator::operator*() const {
    return (*owner_.data_.GetData())[pos_];
}

char CowString::ConstCowStringIterator::operator->() const {
    return (*owner_.data_.GetData())[pos_];
}

bool CowString::ConstCowStringIterator::operator!=(const CowString::ConstCowStringIterator& other) {
    return &((*owner_.data_.GetData())[pos_]) != &((*other.owner_.data_.GetData())[other.pos_]);
}

CowString::ConstCowStringIterator& CowString::ConstCowStringIterator::operator++() {
    ++pos_;
    return *this;
}

CowString::CowString(std::string_view data) : data_(data) {
}

CowString::CowString(const CowString& other) : data_(other.data_) {
}

CowString::CowString(CowString&& other) : data_(std::move(other.data_)) {
    other.data_.DecreaseRefsCount();
}

CowString::CowStringIterator CowString::begin() {
    return CowStringIterator(0, *this);
}

CowString::ConstCowStringIterator CowString::begin() const {
    return ConstCowStringIterator(0, *this);
}

CowString::CowStringIterator CowString::end() {
    return CowStringIterator(data_.GetDataSize(), *this);
}

CowString::ConstCowStringIterator CowString::end() const {
    return ConstCowStringIterator(data_.GetDataSize(), *this);
}

CowString::ProxyChar CowString::operator[](size_t pos) {
    return CowString::ProxyChar(pos, *this);
}

char CowString::At(size_t pos) const {
    if (pos < data_.GetDataSize()) {
        return (*data_.GetData())[pos];
    } else {
        throw std::out_of_range("CowString");
    }
}

CowString CowString::operator+(const CowString& other) {
    std::string new_data = *(data_.GetData()) + *(other.data_.GetData());
    return CowString(std::string_view(new_data));
}

CowString CowString::operator+(std::string_view data) {
    return *this + CowString(data);
}

CowString& CowString::operator+=(const CowString& other) {
    if (other.data_.GetData()->empty()) {
        return *this;
    }
    std::string new_data = *(data_.GetData()) + *(other.data_.GetData());
    data_ = new_data;
    return *this;
}

CowString& CowString::operator+=(std::string_view other) {
    *this += CowString(other);
    return *this;
}

CowString& CowString::operator=(const CowString& other) {
    if (this != &other) {
        data_ = other.data_;
    }
    return *this;
}

CowString& CowString::operator=(CowString&& other) {
    if (this != &other) {
        data_ = std::move(other.data_);
        other.data_.DecreaseRefsCount();
    }
    return *this;
}

CowString& CowString::operator=(std::string_view data) {
    data_.DecreaseRefsCount();
    data_ = InnerString(data);
    return *this;
}

bool CowString::operator==(const CowString& other) const {
    return *data_.GetData() == *other.data_.GetData();
}

bool CowString::operator==(std::string_view other) const {
    return *data_.GetData() == std::string(other);
}

bool operator==(std::string_view lhs, const CowString& rhs) {
    return CowString(lhs) == rhs;
}

bool CowString::operator!=(const CowString& other) const {
    return !(*this == other);
}

bool CowString::operator!=(std::string_view other) const {
    return !(*this == other);
}

bool operator!=(std::string_view lhs, const CowString& rhs) {
    return !(lhs == rhs);
}

const char* CowString::GetData() const {
    return data_.GetData()->c_str();
}

size_t* CowString::GetInnerRefsCount() {
    return data_.GetRefsCount();
}

CowString::operator std::string_view() const {
    return std::string_view(*data_.GetData());
}