#include "nfseerror.h"
namespace NFSE
{
    NfseError::NfseError(const std::string& reason) :
        std::runtime_error(reason)
    {}
}
