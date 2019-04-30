#ifndef ESSENTIA_LANDR_MEAN_FRAMES_H
#define ESSENTIA_LANDR_MEAN_FRAMES_H

#include <essentia/essentiamath.h>
#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class MeanFrames : public Algorithm {
        protected:
            Sink<std::vector<std::vector<Real> > > _input;
            Source<std::vector<Real> > _output;

        public:
            MeanFrames() : Algorithm() {
                declareInput(_input, 1, "input", "");
                declareOutput(_output, 1, "output", "");
                declareParameters();
            }

            ~MeanFrames();

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

#endif ESSENTIA_LANDR_MEAN_FRAMES_H
