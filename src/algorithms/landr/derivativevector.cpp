#include "derivativevector.h"

namespace essentia {
namespace streaming {

    const char* DerivativeVector::name = "DerivativeVector";
    const char* DerivativeVector::category = "None";
    const char* DerivativeVector::description = "";

    AlgorithmStatus DerivativeVector::process() {
        auto status = acquireData();
        if (status != OK)
        {
            if (!shouldStop()) return status;

            return NO_INPUT;
        }

        // Grab frames from stream, as requested in constructor
        const auto &inputFrames = _input.tokens();
        auto& outputFrames = _output.tokens();

        // Loop frames in stream
        for (int i = 0; i < inputFrames.size() && i < outputFrames.size(); ++i)
        {
            // Make empty output vector
            outputFrames[i].resize(inputFrames[i].size());

            //Make sure the frames are of the same size
            if (_prevFrame.size() != inputFrames[i].size()) {
                _prevFrame.resize(inputFrames[i].size());
            }

            // Loop samples in frame
            for (int j = 0; j<inputFrames[i].size(); ++j)
            {
                // Calculate derivative and store previous vector
                outputFrames[i][j] = inputFrames[i][j] - _prevFrame[j];
                _prevFrame[j] = inputFrames[i][j];
            }
        }

        releaseData();
        return OK;
    }

} // namespace streaming
} // namespace essentia
