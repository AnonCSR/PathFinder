#pragma once

#include "query/optimizer/plan/plan.h"

namespace SPARQL {
class TriplePlan : public Plan {
public:
    TriplePlan(Id subject, Id predicate, Id object);

    TriplePlan(const TriplePlan& other) :
        subject            (other.subject),
        predicate          (other.predicate),
        object             (other.object),
        subject_assigned   (other.subject_assigned),
        predicate_assigned (other.predicate_assigned),
        object_assigned    (other.object_assigned) { }

    std::unique_ptr<Plan> clone() const override {
        return std::make_unique<TriplePlan>(*this);
    }

    double estimate_cost() const override;
    double estimate_output_size() const override;

    std::set<VarId> get_vars() const override;
    void            set_input_vars(const std::set<VarId>& input_vars) override;

    std::unique_ptr<BindingIter> get_binding_iter() const override;

    bool get_leapfrog_iter(std::vector<std::unique_ptr<LeapfrogIter>>& leapfrog_iters,
                           std::vector<VarId>&                         var_order,
                           uint_fast32_t&                              enumeration_level) const override;

    void print(std::ostream& os, int indent) const override;

private:
    Id subject;
    Id predicate;
    Id object;

    bool subject_assigned;
    bool predicate_assigned;
    bool object_assigned;
};
} // namespace SPARQL
