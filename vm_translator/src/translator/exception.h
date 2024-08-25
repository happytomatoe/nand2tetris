#ifndef INVALIDIDENTIFIERSORDEREXCEPTION_H
#define INVALIDIDENTIFIERSORDEREXCEPTION_H

#include <cpptrace/cpptrace.hpp>
using namespace std;

class BaseException : public cpptrace::exception_with_message {
public:
    int line_number;

    explicit BaseException(
        const int line_number,
        std::string &&message_arg,
        cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()
    ) noexcept
        : exception_with_message(std::move(message_arg), std::move(trace)), line_number(line_number) {
    }
};

class InvalidOperation final : public BaseException {
public:
    InvalidOperation(int line_number, std::string &&message_arg = "Invalid operation",
                     cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()) noexcept
        : BaseException(line_number, std::move(message_arg), std::move(trace)) {
    }
};

class NumberOverflowException final : public BaseException {
public:
    NumberOverflowException(int line_number, std::string &&message_arg,
                            cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()) noexcept
        : BaseException(line_number, std::move(message_arg), std::move(trace)) {
    }
};

class UnexpectedToken final : public BaseException {
public:
    UnexpectedToken(int line_number, std::string &&message_arg,
                    cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()) noexcept
        : BaseException(line_number, std::move(message_arg), std::move(trace)) {
    }
};


class PointerOutOfRangeException final : public BaseException {
public:
    PointerOutOfRangeException(int line_number, std::string &&message_arg,
                               cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()) noexcept
        : BaseException(line_number, std::move(message_arg), std::move(trace)) {
    }
};

class StackPointerOutOfRangeException final : public BaseException {
public:
    explicit StackPointerOutOfRangeException(int line_number,
                                             std::string &&message_arg = "Stack pointer is out of range",
                                             cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb())
        noexcept
        : BaseException(line_number, std::move(message_arg), std::move(trace)) {
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
