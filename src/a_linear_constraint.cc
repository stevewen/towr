/**
 @file    a_linear_constraint.cc
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    May 26, 2016
 @brief   Brief description
 */

#include <xpp/zmp/a_linear_constraint.h>

namespace xpp {
namespace zmp {

ALinearConstraint::ALinearConstraint (OptimizationVariables& subject)
    :IObserver(subject)
{
}

void
ALinearConstraint::Init (const MatVec& linear_equation)
{
  linear_equation_ = linear_equation;
  Update();
}

void
ALinearConstraint::Update ()
{
  x_coeff_ = subject_->GetSplineCoefficients();
}

ALinearConstraint::VectorXd
ALinearConstraint::EvaluateConstraint () const
{
  return linear_equation_.M*x_coeff_; // linear part respected in bounds
}

LinearEqualityConstraint::LinearEqualityConstraint (OptimizationVariables& subject)
    :ALinearConstraint(subject)
{
}

LinearEqualityConstraint::VecBound
LinearEqualityConstraint::GetBounds () const
{
  VecBound bounds;
  for (int i=0; i<linear_equation_.v.rows(); ++i) {
    Bound bound(-linear_equation_.v[i],-linear_equation_.v[i]);
    bounds.push_back(bound);
  }
  return bounds;
}

LinearInequalityConstraint::LinearInequalityConstraint (OptimizationVariables& subject)
    :ALinearConstraint(subject)
{
}

LinearInequalityConstraint::VecBound
LinearInequalityConstraint::GetBounds () const
{
  VecBound bounds;
  for (int i=0; i<linear_equation_.v.rows(); ++i) {
    Bound bound(-linear_equation_.v[i], kNoBound_.upper_);
    bounds.push_back(bound);
  }
  return bounds;
}

} /* namespace zmp */
} /* namespace xpp */

