#ifndef NFSEERROR_H
#define NFSEERROR_H

#include <stdexcept>

namespace NFSE
{
    class NfseError : public std::runtime_error
    {
        public:
            NfseError(const std::string& reason);
    };
}

#endif // NFSEERROR_H
