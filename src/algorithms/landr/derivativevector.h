#ifndef ESSENTIA_LANDR_DERIVATIVE_VECTOR_H
#define ESSENTIA_LANDR_DERIVATIVE_VECTOR_H

#include <essentia/essentiamath.h>
#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class DerivativeVector : public Algorithm {
        protected:
            Sink<std::vector<Real> > _input;
            Source<std::vector<Real> > _output;

        public:
            DerivativeVector() : Algorithm() {
                declareInput(_input, 1, "input", "");
                declareOutput(_output, 1, "output", "");
                declareParameters();
            }

            ~DerivativeVector() {}   
        
            void declareParameters() {};
            void configure() {};

            AlgorithmStatus process();
            
            static const char* name;
            static const char* category;
            static const char* description;
            
        private:
            std::vector<Real> _prevFrame;

    };

} // namespace streaming
} // namespace essentia

#endif ESSENTIA_LANDR_DERIVATIVE_VECTOR_H
