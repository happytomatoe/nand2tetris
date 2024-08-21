//
// Created by babkamen on 8/20/24.
//

#ifndef INVALIDIDENTIFIERSORDEREXCEPTION_H
#define INVALIDIDENTIFIERSORDEREXCEPTION_H

#include <cpptrace/cpptrace.hpp>
using namespace std;

class invalid_identifiers_order_before_assignment_exception final : public cpptrace::exception_with_message {
public:
    explicit invalid_identifiers_order_before_assignment_exception(
        std::string &&message_arg,
        cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()
    ) noexcept
        : exception_with_message(std::move(message_arg), std::move(trace)) {
    }
};


class invalid_operand_order_exception final : public cpptrace::exception_with_message {
public:
    explicit invalid_operand_order_exception(
        std::string &&message_arg,
        cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()
    ) noexcept
        : exception_with_message(std::move(message_arg), std::move(trace)) {
    }
};

class syntax_error_exception final : public cpptrace::exception_with_message {
public:
    explicit syntax_error_exception(
        std::string &&message_arg,
        cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()
    ) noexcept
        : exception_with_message(std::move(message_arg), std::move(trace)) {
    }
};

class invalid_symbol_exception final : public cpptrace::exception_with_message {
public:
    explicit invalid_symbol_exception(
        std::string &&message_arg,
        cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()
    ) noexcept
        : exception_with_message(std::move(message_arg), std::move(trace)) {
    }
};

class number_overflow_exception final : public cpptrace::exception_with_message {
public:
    explicit number_overflow_exception(
        std::string &&message_arg,
        cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()
    ) noexcept
        : exception_with_message(std::move(message_arg), std::move(trace)) {
    }
};


#endif //INVALIDIDENTIFIERSORDEREXCEPTION_H
