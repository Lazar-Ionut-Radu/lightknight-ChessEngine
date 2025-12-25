#ifndef LIGHTKNIGHT_EXCEPTIONS_H
#define LIGHTKNIGHT_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace lightknight::exceptions {
    
    class FENException : public std::runtime_error {
    public:
        explicit FENException(const std::string& msg);
    };
} // namespace lightknight::exceptions

#endif // LIGHTKNIGHT_EXCEPTIONS_H
