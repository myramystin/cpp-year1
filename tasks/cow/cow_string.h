#pragma once
#include <string_view>
#include <string>

class CowString {
public:
    CowString(std::string_view data);
    CowString(const CowString& other);
    CowString(CowString&& other);

    class ProxyChar {
    public:
        ProxyChar(size_t pos, CowString& owner);
        void operator=(const char& c);
        operator char() const;

    private:
        size_t pos_;
        CowString& owner_;
    };

    class CowStringIterator {
    public:
        CowStringIterator(size_t pos, CowString& owner);

        ProxyChar operator*();
        ProxyChar operator->();
        char operator*() const;
        char operator->() const;
        bool operator!=(const CowStringIterator& other);
        CowStringIterator& operator++();

    private:
        size_t pos_;
        CowString& owner_;
    };

    class ConstCowStringIterator {
    public:
        ConstCowStringIterator(size_t pos, const CowString& owner);

        char operator*() const;
        char operator->() const;

        bool operator!=(const ConstCowStringIterator& other);
        ConstCowStringIterator& operator++();

    private:
        size_t pos_;
        const CowString& owner_;
    };

    CowStringIterator begin();             // NOLINT
    CowStringIterator end();               // NOLINT
    ConstCowStringIterator begin() const;  // NOLINT
    ConstCowStringIterator end() const;    // NOLINT

    ProxyChar operator[](size_t pos);
    char At(size_t pos) const;

    CowString operator+(const CowString& other);
    CowString operator+(std::string_view data);
    CowString& operator+=(const CowString& other);
    CowString& operator+=(std::string_view other);

    CowString& operator=(const CowString& other);
    CowString& operator=(std::string_view data);
    CowString& operator=(CowString&& other);

    bool operator==(const CowString& other) const;
    bool operator==(std::string_view other) const;
    friend bool operator==(std::string_view lhs, const CowString& rhs);
    bool operator!=(const CowString& other) const;
    bool operator!=(std::string_view other) const;
    friend bool operator!=(std::string_view lhs, const CowString& rhs);

    const char* GetData() const;

    operator std::string_view() const;

    size_t* GetInnerRefsCount();

    class InnerString {
    public:
        InnerString(std::string_view data);
        InnerString(const InnerString& other);
        ~InnerString();

        void IncreaseRefsCount();
        void DecreaseRefsCount();

        std::string* GetData() const;
        size_t GetDataSize() const;
        size_t* GetRefsCount() const;

        InnerString& operator=(std::string_view data);
        InnerString& operator=(const InnerString& other);

        void SetAt(size_t pos, const char& c);

    private:
        std::string* data_;
        size_t* refs_count_;
    private:
        void SetRefsCount(size_t* count);
        void SetData(std::string* data);
    };

private:
    InnerString data_;
};
