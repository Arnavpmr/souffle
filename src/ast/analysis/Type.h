/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2013, 2015, Oracle and/or its affiliates. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file Type.h
 *
 * A collection of type analyses operating on AST constructs.
 *
 ***********************************************************************/

#pragma once

#include "FunctorOps.h"
#include "ast/Argument.h"
#include "ast/Clause.h"
#include "ast/analysis/Analysis.h"
#include "ast/analysis/TypeSystem.h"
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace souffle::ast {
class Functor;
class FunctorDeclaration;
class IntrinsicFunctor;
class UserDefinedFunctor;
}  // namespace souffle::ast

namespace souffle::ast::analysis {

class TypeAnalysis : public Analysis {
public:
    static constexpr const char* name = "type-analysis";

    TypeAnalysis() : Analysis(name) {}

    void run(const TranslationUnit& translationUnit) override;

    void print(std::ostream& os) const override;

    /**
     * Get the computed types for the given argument.
     */
    TypeSet const& getTypes(const Argument* argument) const {
        return argumentTypes.at(argument);
    }

    /**
     * Analyse the given clause and computes for each contained argument
     * a set of potential types. If the set associated to an argument is empty,
     * no consistent typing can be found and the rule can not be properly typed.
     *
     * @param env a typing environment describing the set of available types
     * @param clause the clause to be typed
     * @param program the program
     * @return a map mapping each contained argument to a a set of types
     */
    static std::map<const Argument*, TypeSet> analyseTypes(
            const TranslationUnit&, const Clause&, std::ostream* /*logs*/ = nullptr);

    /** Functor-related */
    TypeAttribute getFunctorReturnType(const Functor* functor) const;

    TypeAttribute getFunctorArgType(const Functor* functor, const size_t idx) const;

    const std::vector<TypeAttribute>& getFunctorArgTypes(const UserDefinedFunctor& udf) const;

    bool isStatefulFunctor(const UserDefinedFunctor* udf) const;

    static bool isMultiResultFunctor(const Functor& functor);

    IntrinsicFunctors validOverloads(const ast::IntrinsicFunctor& func, IntrinsicFunctors functorInfos) const;

    bool hasProcessedFunctor(const Functor* functor) const;
    bool isInvalidFunctor(const IntrinsicFunctor* func) const;

private:
    std::map<const Argument*, TypeSet> argumentTypes;
    VecOwn<Clause> annotatedClauses;
    std::stringstream analysisLogs;

    std::map<std::string, const FunctorDeclaration*> udfDeclaration;
    std::map<const IntrinsicFunctor*, const IntrinsicFunctorInfo*> functorInfo;
    std::set<const IntrinsicFunctor*> invalidFunctors;
};

}  // namespace souffle::ast::analysis
