#ifndef ESSENTIA_LANDR_COUNT_H
#define ESSENTIA_LANDR_COUNT_H

#include <memory>
#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class Count : public AlgorithmComposite
    {
    public:
        
        Count();
        virtual ~Count();       
       
        void declareParameters() override;
        void configure() override;
        AlgorithmStatus process() override;
        
        static const char* name;
        static const char* category;
        static const char* description;
    
        static void Register();
        
        void declareProcessOrder() override
        {
            declareProcessStep(ChainFrom(_vectorOutput.get()));
            declareProcessStep(SingleShot(this));
        }

    protected:
        
        SinkProxy<std::vector<essentia::Real> > _input;
        Source<essentia::Real> _output;
        
    private:

        std::shared_ptr<Algorithm> _vectorOutput;
        std::vector<std::vector<essentia::Real>> _accu;

    };

} // namespace streaming
} // namespace essentia

#endif // ESSENTIA_COUNT_H
