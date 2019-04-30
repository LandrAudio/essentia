#include "normalizer.h"

namespace essentia {
namespace streaming {

const char* Normalizer::name = "Normalizer";
const char* Normalizer::category = "None";
const char* Normalizer::description = "";

Normalizer::Normalizer()
    : Algorithm()
{
    declareInput(_input, 1, "input", "");
    declareOutput(_output, 1, "output", "");
    declareParameters();
}

Normalizer::~Normalizer()
{}

void
Normalizer::Register()
{
    AlgorithmFactory::Registrar<Normalizer> Normalizer;
}

void
Normalizer::configure()
{}

void
Normalizer::declareParameters()
{}

AlgorithmStatus
Normalizer::process()
{
    auto status = acquireData();
    if (status != OK)
    {
        if (!shouldStop()) return status;

        return NO_INPUT;
    }

    const std::vector<std::vector<Real > >& inputFrames = _input.tokens();
    std::vector<std::vector<Real > >& outputFrames = _output.tokens();

    for (int i = 0; i < inputFrames.size() && i < outputFrames.size(); ++i)
    {
        outputFrames[i] = inputFrames[i];
        normalize(outputFrames[i]);
    }

    releaseData();
    return OK;
}

} // namespace streaming
} // namespace essentia
