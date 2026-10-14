export module utility : strutil;

import std;
import : type_restion;

// Generic StringUtility with explicit constraints
export class strutil
{
public:
    using size_t = std::size_t;

private:

    template <rest::character CharacterType>
    static constexpr size_t length (const CharacterType* str)
        noexcept
    {
        return std::char_traits<CharacterType>::length(str);
    }

    template <rest::character CharacterType>
    static constexpr CharacterType* copy (CharacterType* dest,
                                    const CharacterType* src,
                                          size_t         size)
        noexcept
    {
        return std::char_traits<CharacterType>::copy(dest, src, size);
    }

    template <rest::character CharacterType>
    static constexpr CharacterType* move (CharacterType* dest,
                                    const CharacterType* src,
                                          size_t         size)
        noexcept
    {
        return std::char_traits<CharacterType>::move(dest, src, size);
    }

    template <rest::character CharacterType>
    static constexpr CharacterType* set (CharacterType* dest,
                                         CharacterType  value,
                                         size_t         size)
        noexcept
    {
        return std::char_traits<CharacterType>::assign(dest, size, value);
    }

    template <rest::character CharacterType>
    static constexpr bool compare (const CharacterType* left,
                                   const CharacterType* src,
                                         size_t         size)
        noexcept
    {
        return std::char_traits<CharacterType>::compare(left, src, size) == 0;
    }

public:

    template <rest::character CharacterType>
    static constexpr size_t strlenof (const CharacterType* str)
        noexcept
    {
        return length<CharacterType>(str);
    }

    template <rest::character CharacterType>
    static constexpr size_t strlenof(const std::initializer_list<const CharacterType*>& list)
        noexcept
    {
        size_t sumlen = 0;
        for (auto* it = list.begin(); it != list.end(); ++it) {
            sumlen += strlenof(*it);
        }
        return sumlen;
    }

    template <rest::character CharacterType>
    static constexpr CharacterType* strcopy (CharacterType* dest,
                                       const CharacterType* src,
                                             size_t         size)
        noexcept
    {
        return copy<CharacterType>(dest, src, size);
    }

    template <rest::character CharacterType>
    static constexpr CharacterType* strmove (CharacterType* dest,
                                       const CharacterType* src,
                                             size_t         size)
        noexcept
    {
        return move<CharacterType>(dest, src, size);
    }

    template <rest::character CharacterType>
    static constexpr CharacterType* strset (CharacterType* dest,
                                            CharacterType  value,
                                            size_t         size)
        noexcept
    {
        return set<CharacterType>(dest, value, size);
    }

    template <rest::character CharacterType>
    static constexpr bool strcmp (const CharacterType* left,
                                  const CharacterType* src,
                                        size_t         size)
        noexcept
    {
        return compare<CharacterType>(left, src, size);
    }

    template <rest::character CharacterType>
    static constexpr std::optional<size_t> strmatch (const CharacterType* str,
                                                           CharacterType  target,
                                                           size_t         len)
        noexcept
    {
        const CharacterType* p = std::char_traits<CharacterType>::find(str, len, target);
        if (p == nullptr) {
            return std::nullopt;
        }
        return static_cast<size_t>(p - str);
    }

    template <rest::character CharacterType>
    static constexpr std::optional<size_t> strmatch (const CharacterType* str,
                                                     const CharacterType* target)
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
        using string_view = std::basic_string_view<CharacterType>;
        const string_view hay   {str,    static_cast<std::size_t>(strlen)};
        const string_view needle{target, static_cast<std::size_t>(tarlen)};
        const auto pos = hay.find(needle);
        if (pos == string_view::npos) {
            return std::nullopt;
        }
        return static_cast<size_t>(pos);
    }
};