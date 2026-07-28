/*
** Deb PROJECT, 2026
** error class tests
** File description:
** basics
*/

#include "Error.hpp"
#include <criterion/criterion.h>

Test(Error_class_basics, message)
{
    try {
        throw my::Error("hello world");
    } catch (const my::Error& e) {
        cr_assert_str_eq(e.what(), "hello world", "Invalid error message, expected 'hello world' but got '%s'.", e.what());
        cr_assert_eq(e.value(), 84, "Invalid error value, expected 84 but got %d.", e.value());
    } catch (...) {
        cr_assert_fail("Invalid error caught.");
    }
}

Test(Error_class_basics, value)
{
    try {
        throw my::Error("hello world", 0, 1);
    } catch (const my::Error& e) {
        cr_assert_str_eq(e.what(), "hello world", "Invalid error message, expected 'hello world' but got '%s'.", e.what());
        cr_assert_eq(e.value(), 1, "Invalid error value, expected 1 but got %d.", e.value());
        cr_assert_eq(e.erno(), 0, "Invalid errno value, expected 0 but got %d.", e.erno());
    } catch (...) {
        cr_assert_fail("Invalid error caught.");
    }

}

Test(Error_class_basics, errno_value)
{
    try {
        throw my::Error("hello world", 2, 1);
    } catch (const my::Error& e) {
        cr_assert_str_eq(e.what(), "hello world | last recorded errno(2): No such file or directory", "Invalid error message, expected 'hello world | last recorded errno(2): No such file or directory' but got '%s'.", e.what());
        cr_assert_eq(e.value(), 1, "Invalid error value, expected 1 but got %d.", e.value());
        cr_assert_eq(e.erno(), 2, "Invalid errno value, expected 2 but got %d.", e.erno());
    } catch (...) {
        cr_assert_fail("Invalid error caught.");
    }
}

Test(Error_class_oss, message)
{
    try {
        throw my::Error(std::ostringstream() << "hello " << "world");
    } catch (const my::Error& e) {
        cr_assert_str_eq(e.what(), "hello world", "Invalid error message, expected 'hello world' but got '%s'.", e.what());
        cr_assert_eq(e.value(), 84, "Invalid error value, expected 84 but got %d.", e.value());
    } catch (...) {
        cr_assert_fail("Invalid error caught.");
    }
}

Test(Error_class_oss, value)
{
    try {
        throw my::Error(std::ostringstream() << "hello " << "world", 0, 1);
    } catch (const my::Error& e) {
        cr_assert_str_eq(e.what(), "hello world", "Invalid error message, expected 'hello world' but got '%s'.", e.what());
        cr_assert_eq(e.value(), 1, "Invalid error value, expected 1 but got %d.", e.value());
        cr_assert_eq(e.erno(), 0, "Invalid errno value, expected 0 but got %d.", e.erno());
    } catch (...) {
        cr_assert_fail("Invalid error caught.");
    }

}

Test(Error_class_oss, errno_value)
{
    try {
        throw my::Error(std::ostringstream() << "hello " << "world", 2, 1);
    } catch (const my::Error& e) {
        cr_assert_str_eq(e.what(), "hello world | last recorded errno(2): No such file or directory", "Invalid error message, expected 'hello world | last recorded errno(2): No such file or directory' but got '%s'.", e.what());
        cr_assert_eq(e.value(), 1, "Invalid error value, expected 1 but got %d.", e.value());
        cr_assert_eq(e.erno(), 2, "Invalid errno value, expected 2 but got %d.", e.erno());
    } catch (...) {
        cr_assert_fail("Invalid error caught.");
    }
}
