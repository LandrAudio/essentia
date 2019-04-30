#include "count.h"

namespace essentia {
namespace streaming {

const char* LANDR::Count::name = "Count";
const char* LANDR::Count::category = "None";
const char* LANDR::Count::description = "";

LANDR::Count::Count()
    : AlgorithmComposite()
{
    declareInput(_input, "input", "");
    declareOutput(_output, 0, "output", "");

    _vectorOutput = std::make_shared<VectorOutput<std::vector<essentia::Real>>>(&_accu);
    _input >> _vectorOutput->input("data");
}

LANDR::Count::~Count()
{}

void
LANDR::Count::Register()
{
    AlgorithmFactory::Registrar<Count> count;
}

void
LANDR::Count::configure()
{}

void
LANDR::Count::declareParameters()
{}

AlgorithmStatus
LANDR::Count::process()
{
    if (!shouldStop()) return PASS;

    float count = 0;

    for (auto data : _accu)
    {
        count += data.size();
    }

    _output.push(count);

    return FINISHED;

}

} // namespace streaming
} // namespace essentia
