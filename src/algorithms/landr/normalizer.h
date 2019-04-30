#ifndef ESSENTIA_LANDR_NORMALIZER_H
#define ESSENTIA_LANDR_NORMALIZER_H

#include <essentia/essentiamath.h>
#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class Normalizer : public Algorithm {
        protected:
            Sink<std::vector<Real> > _input;
            Source<std::vector<Real> > _output;
            
        public:
            Normalizer() : Algorithm() {
                declareInput(_input, 1, "input", "");
                declareOutput(_output, 1, "output", "");
                declareParameters();
            }
            
            ~Normalizer() {}

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

#endif ESSENTIA_LANDR_NORMALIZER_H
