#ifndef ESSENTIA_LANDR_MEAN_FRAMES_ALGO_H
#define ESSENTIA_LANDR_MEAN_FRAMES_ALGO_H

#include <essentia/essentiamath.h>
#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class MeanFramesAlgo : public Algorithm {
        protected:
            Sink<std::vector<std::vector<Real> > > _input;
            Source<std::vector<Real> > _output;

        public:
            MeanFramesAlgo() : Algorithm() {
                declareInput(_input, 1, "input", "");
                declareOutput(_output, 1, "output", "");
                declareParameters();
            }

            ~MeanFramesAlgo();

            AlgorithmStatus process();
        
            void declareParameters() {}
            void configure() {}
            
            static const char* name;
            static const char* category;
            static const char* description;
            
        private:

    };

} // namespace streaming
} // namespace essentia

#endif ESSENTIA_LANDR_MEAN_FRAMES_ALGO_H
