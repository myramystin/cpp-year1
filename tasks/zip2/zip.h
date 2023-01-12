#pragma once

#include <iterator>

template <typename Iterator1, typename Iterator2>
class Zipped {
public:
    Zipped(Iterator1 lhs_begin, Iterator2 rhs_begin, Iterator1 lhs_end, Iterator2 rhs_end)
        : lhs_begin_(lhs_begin), rhs_begin_(rhs_begin), lhs_end_(lhs_end), rhs_end_(rhs_end){};

    class ZipIterator {
    public:
        ZipIterator(Iterator1 lhs, Iterator2 rhs, Iterator1 lhs_end, Iterator2 rhs_end)
            : lhs_(lhs), rhs_(rhs), lhs_end_(lhs_end), rhs_end_(rhs_end){};

        bool operator!=(const ZipIterator& other) {
            return lhs_ != other.lhs_ || rhs_ != other.rhs_;
        }

        ZipIterator& operator++() {
            ++lhs_;
            ++rhs_;
            if (lhs_ == lhs_end_ || rhs_ == rhs_end_) {
                lhs_ = lhs_end_;
                rhs_ = rhs_end_;
            }
            return *this;
        }

        std::pair<typename std::iterator_traits<Iterator1>::reference,
                  typename std::iterator_traits<Iterator2>::reference>
        operator*() const {
            return {*lhs_, *rhs_};
        }

    private:
        Iterator1 lhs_;
        Iterator2 rhs_;
        Iterator1 lhs_end_;
        Iterator2 rhs_end_;
    };

    ZipIterator begin() {
        return ZipIterator(lhs_begin_, rhs_begin_, lhs_end_, rhs_end_);
    }

    ZipIterator end() {
        return ZipIterator(lhs_end_, rhs_end_, lhs_end_, rhs_end_);
    }

private:
    Iterator1 lhs_begin_;
    Iterator2 rhs_begin_;
    Iterator1 lhs_end_;
    Iterator2 rhs_end_;
};

template <typename Sequence1, typename Sequence2>
Zipped<decltype(std::begin(std::declval<const Sequence1&>())), decltype(std::begin(std::declval<const Sequence2&>()))>
Zip(const Sequence1& sequence1, const Sequence2& sequence2) {
    return Zipped<decltype(std::begin(sequence1)), decltype(std::begin(sequence2))>(
        std::begin(sequence1), std::begin(sequence2), std::end(sequence1), std::end(sequence2));
}
