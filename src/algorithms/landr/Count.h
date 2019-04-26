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

#pragma once

//=============================================================================
// I N C L U D E   F I L E S   A N D   F O R W A R D   D E C L A R A T I O N S

#include <memory>

//=============================================================================
//P R E P R O C E S S O R   D E F I N I T I O N

//=============================================================================
// C L A S S

namespace LANDR
{
    class Count : public essentia::streaming::AlgorithmComposite
    {
    public:
        
        Count();
        virtual ~Count();       
       
        void declareParameters() override;
        void configure() override;
        essentia::streaming::AlgorithmStatus process() override;
        
        static const char* name;
        static const char* category;
        static const char* description;
    
        static void Register();
        
        void declareProcessOrder() override
        {
            declareProcessStep(essentia::streaming::ChainFrom(_vectorOutput.get()));
            declareProcessStep(essentia::streaming::SingleShot(this));
        }

    protected:
        
        essentia::streaming::SinkProxy<std::vector<essentia::Real> > _input;
        essentia::streaming::Source<essentia::Real> _output;
        
    private:

        std::shared_ptr<essentia::streaming::Algorithm> _vectorOutput;
        std::vector<std::vector<essentia::Real>> _accu;

    };
}
