#include "bandmaxaggregator.h"

namespace essentia {
namespace streaming {

    const char* BandMaxAggregator::name = "BandMaxAggregator";
    const char* BandMaxAggregator::category = "None";
    const char* BandMaxAggregator::description = "Implements MaxFilter over each band of the filterbank (typically produced by ERB). The filter operates over a width that adapts to ensure a fixed time result.";

    AlgorithmStatus BandMaxAggregator::process() {
        auto status = acquireData();
        if (status != OK) {
            if (!shouldStop()) return status;
            return NO_INPUT;
        }

        const std::vector<std::vector<Real>> bandsIn = _bandsInput.firstToken();
        std::vector<std::vector<Real>>& bandsOut = _bandsOutput.tokens();
        std::vector<std::vector<Real>>::size_type lengthInFrames = bandsIn.size();
        // compute exact (i.e. fractional) filter width.
        float exactFilterWidth = float(lengthInFrames) / _nTimesPerBand;
        // Fractional length of the filter being output on each iteration.
        // We initialise this to non-zero to avoid duplication of the first element caused by filterWidth == 0.
        float exactOutputLength = exactFilterWidth;
        int emittedLength = 0;
        std::vector<Real>::size_type nBands = bandsIn[0].size();

        COUT << "In process lengthInFrames " << lengthInFrames << " exactFilterWidth " << exactFilterWidth << std::endl;

        int inputIndex = 0; // The index into the bands to draw from next.
        int outputIndex = 0; // The index to the band to write next.
        // Run until the desired number of bands has been generated, or we run out of input data.
        while (exactOutputLength < lengthInFrames + exactFilterWidth && outputIndex < _nTimesPerBand) {
            // Determine the number of frames to check for each max filter. Using int() creates a floor() function.
            int filterWidth = int(exactOutputLength - emittedLength);
            COUT << "In process exactOutputLength " << exactOutputLength << " lengthInFrames " << lengthInFrames << " filterWidth " << filterWidth << std::endl;

            if (exactFilterWidth > 1.0) {  // i.e. we are compressing data using the max filter:
                // run the max filter over the filter width for each band.
                std::vector<Real> bandMaxes(nBands);
                for (int i = 0; i < filterWidth; ++i, ++inputIndex) {
                    // Determine maximum for each band, assign to bandMaxes
                    const Real *bandsToCheck = bandsIn[inputIndex].data();
                    for (int bandIndex = 0; bandIndex < nBands; bandIndex++) {
                        if (bandsToCheck[bandIndex] > bandMaxes[bandIndex]) {
                            bandMaxes[bandIndex] = bandsToCheck[bandIndex];
                        }
                    }
                }
                COUT << "In process writing out bandMaxes for position " << outputIndex << std::endl;
                bandsOut[outputIndex++] = bandMaxes;
            }
            else {      // we are expanding data using duplication:
                COUT << "In process writing out band inputIndex " << inputIndex << " to outputIndex " << outputIndex << std::endl;
                bandsOut[outputIndex++] = bandsIn[inputIndex];
                inputIndex += filterWidth;             // Only begin emitting a new band if we have advanced enough to do so (filterWidth > 0).
            }
            exactOutputLength += exactFilterWidth;
            emittedLength += filterWidth;
        }

        releaseData();
        return FINISHED;
    }

} // namespace streaming
} // namespace essentia
