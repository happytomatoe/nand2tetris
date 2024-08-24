#ifndef INVALIDIDENTIFIERSORDEREXCEPTION_H
#define INVALIDIDENTIFIERSORDEREXCEPTION_H

#include <cpptrace/cpptrace.hpp>
using namespace std;

class InvalidOperation final : public cpptrace::exception_with_message {
public:
    explicit InvalidOperation(
        std::string &&message_arg,
        cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()
    ) noexcept
        : exception_with_message(std::move(message_arg), std::move(trace)) {
    }
};

class UnexpectedToken final : public cpptrace::exception_with_message {
public:
    explicit UnexpectedToken(
        std::string &&message_arg,
        cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()
    ) noexcept
        : exception_with_message(std::move(message_arg), std::move(trace)) {
    }
};

class NumberOverflowException final : public cpptrace::exception_with_message {
public:
    explicit NumberOverflowException(
        std::string &&message_arg,
        cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()
    ) noexcept
        : exception_with_message(std::move(message_arg), std::move(trace)) {
    }
};

class PointerOutOfRangeException final : public cpptrace::exception_with_message {
public:
    explicit PointerOutOfRangeException(
        std::string &&message_arg,
        cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()
    ) noexcept
        : exception_with_message(std::move(message_arg), std::move(trace)) {
    }
};

class NotImplementedException final : public cpptrace::exception_with_message {
public:
    explicit NotImplementedException(
        std::string &&message_arg = "Not implemented yet",
        cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()
    ) noexcept
        : exception_with_message(std::move(message_arg), std::move(trace)) {
    }
};


#endif //INVALIDIDENTIFIERSORDEREXCEPTION_H
