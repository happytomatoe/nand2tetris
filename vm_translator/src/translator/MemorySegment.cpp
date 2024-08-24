#include <map>
#include <cpptrace/cpptrace.hpp>
using namespace std;

//TODO: should this be a class?e

namespace memory {
    enum MemorySegmentPointer {
        StackPointer,
        Local,
        Arg,
        This,
        That,
        Static,
        Const,
        Temp,
        Pointer,
    };

    //both min and max is inclusive
    struct Range {
        int min, max;
    };


    const map<MemorySegmentPointer, Range> memorySegmentMinMaxAdress = {
        {Pointer, {3, 4}},
        {Temp, {5, 12}},

        {Static, {16, 255}},
        {StackPointer, {256, 2047}},
        //Dynamically allocated
        {Local, {2048, 2099}},
        {Arg, {3000, 3999}},
        {This, {4000, 4999}},
        {That, {5000, 5999}},
        // we don't save const in memory. It is only used to get a value and push
        // it onto a stack
    };
    const map<MemorySegmentPointer, int> symbolAdress = {
        {StackPointer, 0},
        {Local, 1},
        {Arg, 2},
        {This, 3},
        {That, 4},
    };


    Range getMemorySegmentMinMaxAdress(MemorySegmentPointer p) {
        if (!memorySegmentMinMaxAdress.contains(p)) {
            throw cpptrace::invalid_argument("Invalid memory segment pointer");
        }
        return memorySegmentMinMaxAdress.at(p);
    }

    int getSymbolAdress(MemorySegmentPointer p) {
        if (!symbolAdress.contains(p)) {
            throw cpptrace::invalid_argument("No symbol for memory segment pointer " + p);
        }
        return symbolAdress.at(p);
    }

    string toString(MemorySegmentPointer p) {
        switch (p) {
            case StackPointer:
                return "SP";
            case Local:
                return "LCL";
            case Arg:
                return "ARG";
            case This:
                return "THIS";
            case That:
                return "THAT";
            case Static:
                return "Static";
            case Const:
                return "Const";
            case Temp:
                return "Temp";
            case Pointer:
                return "Pointer";
            default:
                throw cpptrace::runtime_error("Invalid memory segment pointer");
        }
    }
}
