#ifndef ESSENTIA_LANDR_BAND_MAX_AGGREGATOR_H
#define ESSENTIA_LANDR_BAND_MAX_AGGREGATOR_H

#include "landrcommon.h"
#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class BandMaxAggregator : public Algorithm {
        protected:
            Sink<std::vector<std::vector<Real> > > _bandsInput;
            Source<std::vector<Real> > _bandsOutput;

        public:
            BandMaxAggregator() : Algorithm() {
                declareInput(_bandsInput, 1, "bands", "Input filter bands");
                declareOutput(_bandsOutput, 1, "bands", "Filtered bands vector");
            };
            
            ~BandMaxAggregator() {}
            
            AlgorithmStatus process();

            void declareParameters() {
                declareParameter("numberTimes", "The number of aggregated times to output", "[0,inf)", 512.f);
            }
            
            void configure() {
                // Configure expected length of audio and the number of time segments.
                _nTimesPerBand = parameter("numberTimes").toInt();
                _bandsOutput.setAcquireSize(_nTimesPerBand);
                _bandsOutput.setReleaseSize(_nTimesPerBand);
            }
            
            static const char* name;
            static const char* category;
            static const char* description;
            
        private:
            int _nTimesPerBand;        
    };

} // namespace streaming
} // namespace essentia

#endif // ESSENTIA_LANDR_BAND_MAX_AGGREGATOR_H
