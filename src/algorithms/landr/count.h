#ifndef ESSENTIA_LANDR_COUNT_H
#define ESSENTIA_LANDR_COUNT_H

#include <memory>
#include "streamingalgorithmcomposite.h"
#include <essentia/streaming/algorithms/vectoroutput.h>

namespace essentia {
namespace streaming {

    class Count : public AlgorithmComposite {
        protected:
            SinkProxy<std::vector<Real> > _input;
            Source<Real> _output;
            
        public:
            Count() : AlgorithmComposite() {
                declareInput(_input, "input", "");
                declareOutput(_output, 0, "output", "");

                _vectorOutput = std::make_shared<VectorOutput<std::vector<Real> > >(&_accu);
                _input >> _vectorOutput->input("data");
            }

            ~Count() {}
        
            AlgorithmStatus process();

            void declareParameters() {};
            void configure() {};
            
            static const char* name;
            static const char* category;
            static const char* description;
            
            void declareProcessOrder() {
                declareProcessStep(ChainFrom(_vectorOutput.get()));
                declareProcessStep(SingleShot(this));
            }

        private:
            std::shared_ptr<Algorithm> _vectorOutput;
            std::vector<std::vector<Real> > _accu;

    };

} // namespace streaming
} // namespace essentia

#endif // ESSENTIA_COUNT_H
