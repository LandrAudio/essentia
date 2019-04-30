// #include "count.h"

// namespace essentia {
// namespace streaming {

//     const char* Count::name = "Count";
//     const char* Count::category = "None";
//     const char* Count::description = "";

//     AlgorithmStatus Count::process() {
//         if (!shouldStop()) return PASS;

//         float count = 0;

//         for (auto data : _accu)
//         {
//             count += data.size();
//         }

//         _output.push(count);

//         return FINISHED;

//     }

// } // namespace streaming
// } // namespace essentia
