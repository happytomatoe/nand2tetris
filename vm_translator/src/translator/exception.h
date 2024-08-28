#ifndef INVALIDIDENTIFIERSORDEREXCEPTION_H
#define INVALIDIDENTIFIERSORDEREXCEPTION_H

#include <cpptrace/cpptrace.hpp>
using namespace std;

class BaseException : public cpptrace::exception_with_message {
public:
    int line_number;

    explicit BaseException(
        const int line_number,
        string &&message_arg,
        cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()
    ) noexcept
        : exception_with_message(move(message_arg), move(trace)), line_number(line_number) {
    }
};

class InvalidToken final : public BaseException {
public:
    InvalidToken(int line_number, string &&message_arg,
                 cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()) noexcept
        : BaseException(line_number, move(message_arg), move(trace)) {
    }
};


class InvalidOperation final : public BaseException {
public:
    InvalidOperation(int line_number, string &&message_arg = "Invalid operation",
                     cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()) noexcept
        : BaseException(line_number, move(message_arg), move(trace)) {
    }
};

class NumberOverflowException final : public BaseException {
public:
    NumberOverflowException(int line_number, string &&message_arg,
                            cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()) noexcept
        : BaseException(line_number, move(message_arg), move(trace)) {
    }
};

class UnexpectedToken final : public BaseException {
public:
    UnexpectedToken(int line_number, string &&message_arg,
                    cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()) noexcept
        : BaseException(line_number, move(message_arg), move(trace)) {
    }
};


class PointerOutOfRangeException final : public BaseException {
public:
    PointerOutOfRangeException(int line_number, string &&message_arg,
                               cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()) noexcept
        : BaseException(line_number, move(message_arg), move(trace)) {
    }
};

class StackPointerOutOfRangeException final : public BaseException {
public:
    explicit StackPointerOutOfRangeException(int line_number,
                                             string &&message_arg = "Stack pointer is out of range",
                                             cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb())
        noexcept
        : BaseException(line_number, move(message_arg), move(trace)) {
    }
};

class NumberOutOfRangeException final : public BaseException {
public:
    explicit NumberOutOfRangeException(int line_number,
                                       string &&message_arg = "Stack pointer is out of range",
                                       cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb())
        noexcept
        : BaseException(line_number, move(message_arg), move(trace)) {
    }
};

class AdressOutOfMemorySegmentRange final : public BaseException {
public:
    explicit AdressOutOfMemorySegmentRange(int line_number,
                                           string &&message_arg = "Number is out of range of memory segment",
                                           cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb())
        noexcept
        : BaseException(line_number, move(message_arg), move(trace)) {
    }
};

class DuplicateIdentifierException final : public BaseException {
public:
    explicit DuplicateIdentifierException(int line_number,
                                          const string &identifier,
                                          cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb())
        noexcept
        : BaseException(line_number, move("Duplicate identifier " + identifier), move(trace)) {
    }
};

class GoToUndeclaredLabel final : public BaseException {
public:
    explicit GoToUndeclaredLabel(int line_number,
                                 const string &identifier,
                                 cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb())
        noexcept
        : BaseException(line_number, move("Undeclared goto label: " + identifier), move(trace)) {
    }
};

class IfGoToUndeclaredLabel final : public BaseException {
public:
    explicit IfGoToUndeclaredLabel(int line_number,
                                   const string &identifier,
                                   cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb())
        noexcept
        : BaseException(line_number, move("Undeclared if-goto label: " + identifier), move(trace)) {
    }
};

class EmptyLabelException final : public BaseException {
public:
    explicit EmptyLabelException(int line_number,
                                 cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb())
        noexcept
        : BaseException(line_number, "Cant find label", move(trace)) {
    }
};

class EmptyFunctionNameException final : public BaseException {
public:
    explicit EmptyFunctionNameException(int line_number,
                                        cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb())
        noexcept
        : BaseException(line_number, "Expected function name", move(trace)) {
    }
};

class NoFunctionArgCountException final : public BaseException {
public:
    explicit NoFunctionArgCountException(int line_number,
                                         const string &functionName,
                                         cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb())
        noexcept
        : BaseException(line_number, "Expected function argument count when declaring function " + functionName,
                        move(trace)) {
    }
};


class NotImplementedException final : public cpptrace::exception_with_message {
public:
    explicit NotImplementedException(
        string &&message_arg = "Not implemented yet",
        cpptrace::raw_trace &&trace = cpptrace::detail::get_raw_trace_and_absorb()
    ) noexcept
        : exception_with_message(move(message_arg), move(trace)) {
    }
};


#endif //INVALIDIDENTIFIERSORDEREXCEPTION_H
