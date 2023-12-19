namespace {
    class empty {
    };
}

struct _interior {
    bool b;
    explicit operator bool() const {return b;}
};

template <typename I, typename T>
class row;

template <typename I = u64, typename T = empty>
class entry : public T {
    friend class row<I, T>;
    I begin;
    I end;
    entry(I begin, I end, T base) : T(base), begin(begin), end(end) {}

    friend auto operator<=>(const entry<I,T>& lhs, const entry<I,T>& rhs) = default;

    friend std::ostream& operator<<(std::ostream& os, const entry<I,T>& e)
    {
        return os << e.begin << "-" << e.end << ":" << bool(T(e));
    }
    /*
    {
        auto cmp = lhs->begin <=> rhs->begin;
        if (cmp != 0) return cmp;
        cmp = lhs->end <=> rhs->end;
        if (cmp != 0) return cmp;
        return T(lhs) <=> T(rhs);
    }
    */
};

template <typename I = u64, typename T = empty>
class row {
    using value_t = entry<I, T>;
    std::vector<value_t> entries;

    public:

    void insert (I begin, I end, T base = {}) {
        if (end < begin) {
            // that's just not right
            throw 1;
        }
        for (auto it = entries.begin(); it != entries.end(); ++it) {
            if (it->end < begin) {
                // insert after this one
                continue;
            }
            // it->end >= begin
            // insert before this one
            if (it->begin <= end) {
                // there is some overlap
                throw 2;
            }
            entries.insert(it, value_t{begin, end, base});
            return;
        }
        entries.insert(entries.end(), value_t{begin, end, base});
    }

    u64 count () {
        u64 sum = 0;
        for (const auto& e : entries) {
            sum += u64(e.end - e.begin + 1);
        }
        return sum;
    }

    template<typename B>
    u64 interior () {
        u64 sum = 0;
        I last_end;
        bool interior = false;
        for (const auto& e : entries) {
            if (interior) {
                sum += u64(e.begin - last_end - 1);
            }
            last_end = e.end;
            sum += u64(e.end - e.begin + 1);
            if (B(e)) {
                interior = !interior;
            }
        }
        if (interior) {
            throw 3;
        }
        return sum;
    }


    friend auto operator<=>(const row<I,T>& lhs, const row<I,T>& rhs) {
        auto cmp = lhs.entries.size() <=> rhs.entries.size();
        if (cmp != 0) return cmp;
        for (auto it_lhs = lhs.entries.begin(), it_rhs = rhs.entries.end()
                ; it_lhs != lhs.entries.end(); ++it_lhs, ++it_rhs) {
            cmp = *it_lhs <=> *it_lhs;
            if (cmp != 0) return cmp;
        }
        return cmp;
    }

    friend std::ostream& operator<<(std::ostream& os, const row<I,T>& r)
    {
        for (const auto& e : r.entries) {
            os << e << "\n";
        }
        return os;
    }
};
