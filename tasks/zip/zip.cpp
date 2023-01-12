#include "zip.h"

Zipped::ZipIterator::ZipIterator(Iterator lhs, Iterator rhs, Iterator lhs_end, Iterator rhs_end)
    : lhs_(lhs), rhs_(rhs), lhs_end_(lhs_end), rhs_end_(rhs_end) {
}

bool Zipped::ZipIterator::operator!=(const ZipIterator& other) {
    return lhs_ != other.lhs_ || rhs_ != other.rhs_;
}

ZippedPair Zipped::ZipIterator::operator*() {
    return ZippedPair(*lhs_, *rhs_);
}
Zipped::ZipIterator& Zipped::ZipIterator::operator++() {
    ++lhs_;
    ++rhs_;
    if (lhs_ == lhs_end_ || rhs_ == rhs_end_) {
        lhs_ = lhs_end_;
        rhs_ = rhs_end_;
    }
    return *this;
}

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end) {
    Zipped result(a_begin, b_begin, a_end, b_end);
    return result;
}
Zipped::ZipIterator Zipped::begin() {
    return ZipIterator(lhs_begin_, rhs_begin_, lhs_end_, rhs_end_);
}

Zipped::ZipIterator Zipped::end() {
    return ZipIterator(lhs_end_, rhs_end_, lhs_end_, rhs_end_);
}
Zipped::Zipped(Iterator lhs_begin, Iterator rhs_begin, Iterator lhs_end, Iterator rhs_end)
    : lhs_begin_(lhs_begin), lhs_end_(lhs_end), rhs_begin_(rhs_begin), rhs_end_(rhs_end) {
}
