#include "meanframesalgo.h"

namespace essentia {
namespace streaming {

    const char* MeanFramesAlgo::name = "MeanFramesAlgo";
    const char* MeanFramesAlgo::category = "None";
    const char* MeanFramesAlgo::description = "";

    AlgorithmStatus MeanFramesAlgo::process() {
        auto status = acquireData();
        if (status != OK)
        {
            if (!shouldStop()) return status;

            return NO_INPUT;
        }

        const std::vector<std::vector<std::vector<Real > > >& frames = _input.tokens();
        std::vector<std::vector<Real > >& means = _output.tokens();

        for (int i = 0; i < frames.size() && i < means.size(); ++i)
        {
            auto frame = frames[i];
            means[i] = meanFrames(frame);        
        }

        releaseData();
        return OK;
    }

} // namespace streaming
} // namespace essentia
