export module utility : strutil;

import std;
import : type_restion;

// Generic StringUtility with explicit constraints
export class strutil
{
public:
    using size_t = std::size_t;
    template <rest::character CharType>
    using traits = std::char_traits<CharType>;

private:

    template <rest::character CharType>
    static constexpr size_t length (const CharType* str)
        noexcept
    {
        return traits<CharType>::length(str);
    }

    template <rest::character CharType>
    static constexpr CharType* copy (CharType* dest,
                               const CharType* src,
                                     size_t    size)
        noexcept
    {
        return traits<CharType>::copy(dest, src, size);
    }

    template <rest::character CharType>
    static constexpr CharType* move (CharType* dest,
                               const CharType* src,
                                     size_t    size)
        noexcept
    {
        return traits<CharType>::move(dest, src, size);
    }

    template <rest::character CharType>
    static constexpr CharType* set (CharType* dest,
                                    CharType  value,
                                    size_t    size)
        noexcept
    {
        return traits<CharType>::assign(dest, size, value);
    }

    template <rest::character CharType>
    static constexpr bool compare (const CharType* left,
                                   const CharType* src,
                                         size_t    size)
        noexcept
    {
        return traits<CharType>::compare(left, src, size) == 0;
    }

public:

    template <rest::character CharType>
    static constexpr size_t strlenof (const CharType* str)
        noexcept
    {
        return length<CharType>(str);
    }

    template <rest::character CharType>
    static constexpr size_t strlenof(const std::initializer_list<const CharType*>& list)
        noexcept
    {
        size_t sumlen = 0;
        for (auto* it = list.begin(); it != list.end(); ++it) {
            sumlen += strlenof(*it);
        }
        return sumlen;
    }

    template <rest::character CharType>
    static constexpr CharType* strcopy (CharType* dest,
                                       const CharType* src,
                                             size_t         size)
        noexcept
    {
        return copy<CharType>(dest, src, size);
    }

    template <rest::character CharType>
    static constexpr CharType* strmove (CharType* dest,
                                  const CharType* src,
                                        size_t    size)
        noexcept
    {
        return move<CharType>(dest, src, size);
    }

    template <rest::character CharType>
    static constexpr CharType* strset (CharType* dest,
                                       CharType  value,
                                       size_t    size)
        noexcept
    {
        return set<CharType>(dest, value, size);
    }

    template <rest::character CharType>
    static constexpr bool strcmp (const CharType* left,
                                  const CharType* src,
                                        size_t    size)
        noexcept
    {
        return compare<CharType>(left, src, size);
    }

    template <rest::character CharType>
    static constexpr std::optional<size_t> strmatch (const CharType* str,
                                                           CharType  target,
                                                           size_t    len)
        noexcept
    {
        const CharType* resu = traits<CharType>::find (
            str, len, target
        );
        if (resu == nullptr) {
            return std::nullopt;
        }
        return static_cast<size_t>(resu - str);
    }

    template <rest::character CharType>
    static constexpr std::optional<size_t> strmatch (const CharType* str,
                                                     const CharType* target)
        noexcept
    {
        const size_t strlen = strlenof(str);
        const size_t tarlen = strlenof(target);
        if (tarlen == 0) {
            return 0;
        }
        if (tarlen > strlen) {
            return std::nullopt;
        }
        using string_view = std::basic_string_view<CharType>;
        const string_view hay   {str,    static_cast<std::size_t>(strlen)};
        const string_view needle{target, static_cast<std::size_t>(tarlen)};
        const auto pos = hay.find(needle);
        if (pos == string_view::npos) {
            return std::nullopt;
        }
        return static_cast<size_t>(pos);
    }
};