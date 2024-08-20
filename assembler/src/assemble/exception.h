//
// Created by babkamen on 8/20/24.
//

#ifndef INVALIDIDENTIFIERSORDEREXCEPTION_H
#define INVALIDIDENTIFIERSORDEREXCEPTION_H

#include <cpptrace/cpptrace.hpp>
using namespace std;
class invalid_identifiers_order_exception final :  public cpptrace::exception_with_message {
public:
     explicit invalid_identifiers_order_exception(
         std::string&& message_arg,
         cpptrace::raw_trace&& trace = cpptrace::detail::get_raw_trace_and_absorb()
     ) noexcept
         : exception_with_message(std::move(message_arg), std::move(trace)) {}
};


#endif //INVALIDIDENTIFIERSORDEREXCEPTION_H
