#include "normalizer.h"

namespace essentia {
namespace streaming {

const char* LANDR::Normalizer::name = "Normalizer";
const char* LANDR::Normalizer::category = "None";
const char* LANDR::Normalizer::description = "";

LANDR::Normalizer::Normalizer()
    : Algorithm()
{
    declareInput(_input, 1, "input", "");
    declareOutput(_output, 1, "output", "");
    declareParameters();
}

LANDR::Normalizer::~Normalizer()
{}

void
LANDR::Normalizer::Register()
{
    AlgorithmFactory::Registrar<Normalizer> Normalizer;
}

void
LANDR::Normalizer::configure()
{}

void
LANDR::Normalizer::declareParameters()
{}

AlgorithmStatus
LANDR::Normalizer::process()
{
    auto status = acquireData();
    if (status != OK)
    {
        if (!shouldStop()) return status;

        return NO_INPUT;
    }

    const std::vector<std::vector<essentia::Real > >& inputFrames = _input.tokens();
    std::vector<std::vector<essentia::Real > >& outputFrames = _output.tokens();

    for (int i = 0; i < inputFrames.size() && i < outputFrames.size(); ++i)
    {
        outputFrames[i] = inputFrames[i];
        essentia::normalize(outputFrames[i]);
    }

    releaseData();
    return OK;
}

} // namespace streaming
} // namespace essentia
