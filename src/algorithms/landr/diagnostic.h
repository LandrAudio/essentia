#ifndef ESSENTIA_LANDR_DIAGNOSTIC_H
#define ESSENTIA_LANDR_DIAGNOSTIC_H

#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class Diagnostic : public Algorithm {
        protected:
            Sink<std::vector<Real> > _frameInput;

        public:
            Diagnostic() : Algorithm() {
                declareInput(_frameInput, 1, "frame", "frame");
                declareParameters();
            }
            
            ~Diagnostic() {}
            
            AlgorithmStatus process();

            void declareParameters() {
                declareParameter("startSample", "the sample to begin printing from", "[0,inf)", 0);
                declareParameter("size", "the size of the frame in samples", "(0,inf)", 1024);
            };
            void configure() {
                _startSample = parameter("startSample").toInt();
                _frameSizeInSamples = parameter("size").toInt();
            }
            
            static const char* name;
            static const char* category;
            static const char* description;

        private:
            int _startSample;
            int _frameSizeInSamples;
        };

} // namespace streaming
} // namespace essentia

#endif ESSENTIA_LANDR_DIAGNOSTIC_H
