#include "meanframes.h"

namespace essentia {
namespace streaming {

const char* LANDR::MeanFrames::name = "MeanFrames";
const char* LANDR::MeanFrames::category = "None";
const char* LANDR::MeanFrames::description = "";

LANDR::MeanFrames::MeanFrames()
    : Algorithm()
{
    declareInput(_input, 1, "input", "");
    declareOutput(_output, 1, "output", "");
    declareParameters();
}

LANDR::MeanFrames::~MeanFrames()
{}

void
LANDR::MeanFrames::Register()
{
    AlgorithmFactory::Registrar<MeanFrames> meanFrames;
}

void
LANDR::MeanFrames::configure()
{}

void
LANDR::MeanFrames::declareParameters()
{}

AlgorithmStatus
LANDR::MeanFrames::process()
{
    auto status = acquireData();
    if (status != OK)
    {
        if (!shouldStop()) return status;

        return NO_INPUT;
    }

    const std::vector<std::vector<std::vector<essentia::Real > > >& frames = _input.tokens();
    std::vector<std::vector<essentia::Real > >& means = _output.tokens();

    for (int i = 0; i < frames.size() && i < means.size(); ++i)
    {
        auto frame = frames[i];
        means[i] = essentia::meanFrames(frame);        
    }

    releaseData();
    return OK;
}

} // namespace streaming
} // namespace essentia
