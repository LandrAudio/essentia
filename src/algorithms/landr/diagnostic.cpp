#include "diagnostic.h"

namespace essentia {
namespace streaming {

const char* LANDR::Diagnostic::name = "Diagnostic";
const char* LANDR::Diagnostic::category = "None";
const char* LANDR::Diagnostic::description = "Dumps out it's input to a log";

LANDR::Diagnostic::Diagnostic()
: Algorithm()
{
    declareInput(_frameInput, 1, "frame", "frame");
    declareParameters();
}

LANDR::Diagnostic::~Diagnostic()
{
}

void
LANDR::Diagnostic::Register()
{
    AlgorithmFactory::Registrar<Diagnostic> diagnostic;
}

void
LANDR::Diagnostic::configure()
{
    _startSample = parameter("startSample").toInt();
    _frameSizeInSamples = parameter("size").toInt();
}

void
LANDR::Diagnostic::declareParameters()
{
    declareParameter("startSample", "the sample to begin printing from", "[0,inf)", 0);
    declareParameter("size", "the size of the frame in samples", "(0,inf)", 1024);
}

AlgorithmStatus
LANDR::Diagnostic::process()
{
    auto status = acquireData();
    if (status != OK)
    {
        if (!shouldStop()) return status;
        
        return NO_INPUT;
    }
    
    // Grab frames from stream, as requested in constructor
    const std::vector<std::vector<essentia::Real>>& frameInput = _frameInput.tokens();
    
    // Loop frames in stream
    for (int i = 0; i < frameInput.size(); ++i)
    {
        for (int sampleIndex = _startSample; sampleIndex < _startSample + _frameSizeInSamples; ++sampleIndex)
        {
            // We specifically do not declare this as COUT because to do so will then render diagnostics useless
            // unless the entire library is recompiled with COUT enabled. We want to be able to add diagnostics
            // to a running system which may not have been compiled with COUT enabled.
            std::cout << "Diagnostic frame[" << i << "][" << sampleIndex << "] = " << frameInput[i][sampleIndex] << std::endl;
        }
    }

    releaseData();
    return OK;
}

} // namespace streaming
} // namespace essentia
