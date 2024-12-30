#ifndef POINTER_HPP
#define POINTER_HPP

#ifndef __cpp_concepts
    #error "This header requires concepts support"
#endif

#include <array>
#include <cstddef>
#include <type_traits>

namespace ptr {
    inline namespace ptr {
        template <class base_type, std::size_t ptr_lvl>
            requires ("Pointer level must be less than 1024", ptr_lvl < 1024)
        struct ptr_base {
            using type = std::add_pointer_t<typename ptr_base<base_type, ptr_lvl - 1>::type>;
        };

        template <class base_type>
        struct ptr_base<base_type, 0> {
            using type = base_type;
        };

        template <class base_type, std::size_t ptr_lvl>
        using ptr_t = typename ptr_base<base_type, ptr_lvl>::type;

        template <class base_type, std::size_t ptr_lvl>
            requires ("Pointer level must be less than 1024", ptr_lvl < 1024)
        class pointer {
        public:
            using size_type = std::size_t;
            using ptr_type  = ptr_t<base_type, ptr_lvl>;

            constexpr pointer(base_type &base) noexcept : _pointers{} {
                _pointers[0] = std::addressof(base);

                for (size_type index = 1; index < ptr_lvl; ++index)
                    _pointers[index] = std::addressof(_pointers[index - 1]);
            }

            pointer(const pointer &)            = delete;
            pointer &operator=(const pointer &) = delete;

            constexpr pointer(pointer &&other) noexcept            = default;
            constexpr pointer &operator=(pointer &&other) noexcept = default;

            constexpr bool operator==(const pointer &) const noexcept = default;

            constexpr size_type size() const noexcept {
                return ptr_lvl;
            }

            template <size_type level>
                requires (level > 0 && level <= ptr_lvl)
            constexpr auto get() const noexcept {
                return static_cast<ptr_t<base_type, level>>(_pointers[level - 1]);
            }

            constexpr auto operator*() const noexcept {
                return *static_cast<ptr_t<base_type, ptr_lvl>>(_pointers.back());
            }

            constexpr auto operator->() const noexcept {
                return static_cast<ptr_t<base_type, ptr_lvl>>(_pointers.back());
            }

            constexpr operator ptr_t<base_type, ptr_lvl>() const noexcept {
                return static_cast<ptr_t<base_type, ptr_lvl>>(_pointers.back());
            }

        private:
            std::array<void *, ptr_lvl> _pointers;
        };

        template <class base_type>
        class pointer<base_type, 0> {
        public:
            using size_type = std::size_t;
            using ptr_type  = base_type;

            constexpr explicit pointer(base_type &base) noexcept : _value(base) {}

            constexpr size_type size() const noexcept {
                return 0;
            }

            constexpr base_type &get() const noexcept {
                return _value;
            }

            constexpr base_type &operator*() const noexcept {
                return _value;
            }

            constexpr base_type *operator->() const noexcept {
                return std::addressof(_value);
            }

        private:
            base_type &_value;
        };

        template <class type>
        struct is_specialization_of_pointer : std::false_type {};

        template <class base_type, std::size_t ptr_lvl>
        struct is_specialization_of_pointer<pointer<base_type, ptr_lvl>> : std::true_type {};

        template <class type>
        constexpr bool is_specialization_of_pointer_v = is_specialization_of_pointer<type>::value;
    } // namespace ptr

    inline namespace drf {
        template <class ptr_type, std::size_t deref_lvl>
            requires ("Dereference level must be less than 1024", deref_lvl < 1024)
        struct deref_base {
            using type =
                std::remove_pointer_t<typename deref_base<std::remove_reference_t<ptr_type>, deref_lvl - 1>::type>;
        };

        template <class no_ptr_type>
        struct deref_base<no_ptr_type, 0> {
            using type = no_ptr_type;
        };

        template <class ptr_type, std::size_t deref_lvl>
        using deref_t = typename deref_base<ptr_type, deref_lvl>::type;

        template <std::size_t deref_lvl, class ptr_type>
        constexpr auto deref(const ptr_type &ptr) {
            if constexpr (deref_lvl == 0)
                return ptr;
            else if constexpr (std::is_pointer_v<std::remove_reference_t<ptr_type>>)
                return deref<deref_lvl - 1>(*ptr);
            else if constexpr (is_specialization_of_pointer_v<std::remove_reference_t<ptr_type>>)
                return deref<deref_lvl - 1>(*ptr);
        }
    } // namespace drf
} // namespace ptr

#endif // POINTER_HPP
