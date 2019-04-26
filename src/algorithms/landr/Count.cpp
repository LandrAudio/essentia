//=============================================================================
//
// Copyright(c) 2016 LANDR Audio Inc. All rights reserved.
//
// These coded instructions, statements, and computer programs contain
// proprietary information written by LANDR and
// are protected by copyright law. They may not be disclosed
// to third parties or copied or duplicated in any form, in whole or
// in part, without the prior written consent of LANDR.
//
//=============================================================================

//=============================================================================
// I N C L U D E   F I L E S

//=============================================================================
// C O N S T A N T S   &   L O C A L   V A R I A B L E S

const char* LANDR::Count::name = "Count";
const char* LANDR::Count::category = "None";
const char* LANDR::Count::description = "";

//=============================================================================
// C O N S T R U C T O R ( S ) / D E S T R U C T O R   C O D E   S E C T I O N

//-----------------------------------------------------------------------------
//
LANDR::Count::Count()
    : AlgorithmComposite()
{
    declareInput(_input, "input", "");
    declareOutput(_output, 0, "output", "");

    _vectorOutput = std::make_shared<essentia::streaming::VectorOutput<std::vector<essentia::Real>>>(&_accu);
    _input >> _vectorOutput->input("data");
}

//-----------------------------------------------------------------------------
//
LANDR::Count::~Count()
{}

//=============================================================================
// M E T H O D S   C O D E   S E C T I O N

//-----------------------------------------------------------------------------
//
void
LANDR::Count::Register()
{
    essentia::streaming::AlgorithmFactory::Registrar<Count> count;
}

//-----------------------------------------------------------------------------
//
void
LANDR::Count::configure()
{}

//-----------------------------------------------------------------------------
//
void
LANDR::Count::declareParameters()
{}

//-----------------------------------------------------------------------------
//
essentia::streaming::AlgorithmStatus
LANDR::Count::process()
{
    if (!shouldStop()) return essentia::streaming::PASS;

    float count = 0;

    for (auto data : _accu)
    {
        count += data.size();
    }

    _output.push(count);

    return essentia::streaming::FINISHED;

}

//-----------------------------------------------------------------------------
//


