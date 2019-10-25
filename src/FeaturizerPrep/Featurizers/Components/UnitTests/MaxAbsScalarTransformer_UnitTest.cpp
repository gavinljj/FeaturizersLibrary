// ----------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License
// ----------------------------------------------------------------------
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../../../3rdParty/optional.h"
#include "../../../Featurizers/MaxAbsScalarFeaturizer.h"
#include "../../TestHelpers.h"

namespace NS = Microsoft::Featurizer;

//transformer test
template <typename InputT, typename TransformedT>
void Transformer_Test(
    std::vector<std::remove_const_t<std::remove_reference_t<InputT>>> const &data,
    TransformedT scale,
    std::vector<std::remove_const_t<std::remove_reference_t<TransformedT>>> const &label
) {
    using ScaleEstimator                         = NS::Featurizers::Components::MaxAbsScalarEstimator<InputT, TransformedT>;
    using Transformer                            = typename ScaleEstimator::Transformer;

    NS::AnnotationMapsPtr const                  pAllColumnAnnotations(NS::CreateTestAnnotationMapsPtr(1));
    ScaleEstimator                               estimator(pAllColumnAnnotations);


    typename ScaleEstimator::TransformerUniquePtr         
                                                pTransformer(std::make_unique<Transformer>(scale));

    std::vector<TransformedT>    	            output;

    output.reserve(data.size());

    for(auto const &item : data) {
        output.emplace_back(pTransformer->execute(item));
    }

    CHECK(NS::TestHelpers::FuzzyCheck(output, label));
}

//TestWrapper for Transformer test
template<typename InputT, typename TransformedT>
void TestWrapper_Transformer(){
    auto inferencingInput = NS::TestHelpers::make_vector<InputT>(
        static_cast<InputT>(-1),
        static_cast<InputT>( 3),
        static_cast<InputT>(-5),
        static_cast<InputT>( 7),
        static_cast<InputT>(-9)
    );
    auto inferencingOutput = NS::TestHelpers::make_vector<TransformedT>(
        static_cast<TransformedT>(-0.1),
        static_cast<TransformedT>( 0.3),
        static_cast<TransformedT>(-0.5),
        static_cast<TransformedT>( 0.7),
        static_cast<TransformedT>(-0.9)
    );
    Transformer_Test<InputT, TransformedT>(inferencingInput, 10, inferencingOutput);
}

//Tranformer test
TEST_CASE("MaxAbsScalarTransformer - input<int8_t> - output<float_t/double_t>") {
    TestWrapper_Transformer<std::int8_t, std::float_t>();
    TestWrapper_Transformer<std::int8_t, std::double_t>();
}

TEST_CASE("MaxAbsScalarTransformer - input<int16_t> - output<float_t/double_t>") {
    TestWrapper_Transformer<std::int16_t, std::float_t>();
    TestWrapper_Transformer<std::int16_t, std::double_t>();
}

TEST_CASE("MaxAbsScalarTransformer - input<int32_t> - output<float_t/double_t>") {
    TestWrapper_Transformer<std::int32_t, std::float_t>();
    TestWrapper_Transformer<std::int32_t, std::double_t>();
}

TEST_CASE("MaxAbsScalarTransformer - input<int64_t> - output<float_t/double_t>") {
    TestWrapper_Transformer<std::int64_t, std::float_t>();
    TestWrapper_Transformer<std::int64_t, std::double_t>();
}

TEST_CASE("MaxAbsScalarTransformer - input<float_t> - output<float_t/double_t>") {
    TestWrapper_Transformer<std::float_t, std::float_t>();
    TestWrapper_Transformer<std::float_t, std::double_t>();
}

TEST_CASE("MaxAbsScalarTransformer - input<double_t> - output<float_t/double_t>") {
    TestWrapper_Transformer<std::double_t, std::double_t>();
}
