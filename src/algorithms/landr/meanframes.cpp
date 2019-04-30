#include "meanframes.h"

namespace essentia {
namespace streaming {

const char* MeanFrames::name = "MeanFrames";
const char* MeanFrames::category = "None";
const char* MeanFrames::description = "";

MeanFrames::MeanFrames()
    : Algorithm()
{
    declareInput(_input, 1, "input", "");
    declareOutput(_output, 1, "output", "");
    declareParameters();
}

MeanFrames::~MeanFrames()
{}

void
MeanFrames::Register()
{
    AlgorithmFactory::Registrar<MeanFrames> meanFrames;
}

void
MeanFrames::configure()
{}

void
MeanFrames::declareParameters()
{}

AlgorithmStatus
MeanFrames::process()
{
    auto status = acquireData();
    if (status != OK)
    {
        if (!shouldStop()) return status;

        return NO_INPUT;
    }

    const std::vector<std::vector<std::vector<Real > > >& frames = _input.tokens();
    std::vector<std::vector<Real > >& means = _output.tokens();

    for (int i = 0; i < frames.size() && i < means.size(); ++i)
    {
        auto frame = frames[i];
        means[i] = meanFrames(frame);        
    }

    releaseData();
    return OK;
}

} // namespace streaming
} // namespace essentia
