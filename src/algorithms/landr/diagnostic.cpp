#include "diagnostic.h"

namespace essentia {
namespace streaming {

    const char* Diagnostic::name = "Diagnostic";
    const char* Diagnostic::category = "None";
    const char* Diagnostic::description = "Dumps out it's input to a log";

    AlgorithmStatus Diagnostic::process() {
        auto status = acquireData();
        if (status != OK)
        {
            if (!shouldStop()) return status;
            
            return NO_INPUT;
        }
        
        // Grab frames from stream, as requested in constructor
        const std::vector<std::vector<Real>>& frameInput = _frameInput.tokens();
        
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
