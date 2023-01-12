#pragma once

#include <forward_list>
#include <string>

using Value = std::string;
using Iterator = std::forward_list<std::string>::const_iterator;
using ZippedPair = std::pair<const Value&, const Value&>;

class Zipped {
public:
    Zipped(Iterator lhs_begin, Iterator rhs_begin, Iterator lhs_end, Iterator rhs_end);
    class ZipIterator {
    public:
        ZipIterator(Iterator lhs, Iterator rhs, Iterator lhs_end, Iterator rhs_end);
        bool operator!=(const ZipIterator& other);
        ZipIterator& operator++();
        ZippedPair operator*();

    private:
        Iterator lhs_;
        Iterator rhs_;
        Iterator lhs_end_;
        Iterator rhs_end_;
    };

    ZipIterator begin();
    ZipIterator end();

private:
    Iterator lhs_begin_;
    Iterator lhs_end_;
    Iterator rhs_begin_;
    Iterator rhs_end_;
};

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);
