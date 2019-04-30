#ifndef ESSENTIA_LANDR_TRANSPOSE_H
#define ESSENTIA_LANDR_TRANSPOSE_H

#include <memory>
#include <essentia/essentiamath.h>
#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class Transpose : public Algorithm {
        protected:
            Sink<std::vector<Real> > _input;
            Source<std::vector<Real> > _output;
            
        public:
            Transpose() : Algorithm() {
                declareInput(_input, 1, "input", "");
                declareOutput(_output, "output", "");
                declareParameters();
            }
            
            ~Transpose() {}

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

#endif ESSENTIA_LANDR_TRANSPOSE_H
