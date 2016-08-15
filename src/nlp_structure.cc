/**
 @file    nlp_structure.cc
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    Jun 8, 2016
 @brief   Defines the class functions in NlpStructure
 */

#include <xpp/zmp/nlp_structure.h>

namespace xpp {
namespace zmp {

class VariableSet {
public:
  typedef Eigen::VectorXd VectorXd;
  typedef AConstraint::VecBound VecBound;

  VariableSet(int n_variables, int idx);
  virtual ~VariableSet();

  VectorXd GetVariables() const;
  VecBound GetBounds() const;
  int GetIndex() const;

  void SetVariables(const VectorXd& x);
private:
  VectorXd x_;
  VecBound bounds_;
  int idx_;
};

VariableSet::VariableSet (int n_variables, int idx)
{
  x_ = Eigen::VectorXd::Zero(n_variables);
  bounds_.assign(n_variables, AConstraint::kNoBound_);
  idx_ = idx;
}

VariableSet::~VariableSet ()
{
}

int
VariableSet::GetIndex () const
{
  return idx_;
}

VariableSet::VectorXd
VariableSet::GetVariables () const
{
  return x_;
}

VariableSet::VecBound
VariableSet::GetBounds () const
{
  return bounds_;
}

void
VariableSet::SetVariables (const VectorXd& x)
{
  x_ = x;
}
///////////////////////////////////////////////////////////////////////////////

NlpStructure::NlpStructure()
{
  n_variables_ = 0;
}

NlpStructure::~NlpStructure ()
{
}

void
NlpStructure::AddVariableSet (int idx, int n_variables)
{
  variable_sets_.push_back(VariableSetPtr(new VariableSet(n_variables, idx)));
  n_variables_ += n_variables;
}

void
NlpStructure::Reset ()
{
  variable_sets_.clear();
  n_variables_ = 0;
}

int
NlpStructure::GetOptimizationVariableCount() const
{
  return n_variables_;
}

NlpStructure::VectorXd
NlpStructure::GetAllOptimizationVariables () const
{
  Eigen::VectorXd x(GetOptimizationVariableCount());
  int c = 0;
  for (const auto& set : variable_sets_) {
    const VectorXd& var = set->GetVariables();
    x.middleRows(c, var.rows()) = var;
    c += var.rows();
  }

  return x;
}

NlpStructure::VecBound
NlpStructure::GetAllBounds () const
{
  VecBound bounds_;
  for (const auto& set : variable_sets_) {
    const VecBound& b = set->GetBounds();
    bounds_.insert(std::end(bounds_), std::begin(b), std::end(b));
  }

  return bounds_;
}

void
NlpStructure::SetAllVariables(const VectorXd& x_all)
{
  int c = 0;
  for (const auto& set : variable_sets_) {
    int n_var = set->GetVariables().rows();
    set->SetVariables(x_all.middleRows(c,n_var));
    c += n_var;
  }
}

void
NlpStructure::SetVariables (int idx, const VectorXd& values)
{
  for (const auto& s : variable_sets_)
    if (s->GetIndex() == idx) {
     s->SetVariables(values);
     return;
    }

  assert(false); // name not present in set
}

NlpStructure::VectorXd
NlpStructure::GetVariables (int idx) const
{
  for (const auto& s : variable_sets_)
    if (s->GetIndex() == idx)
     return s->GetVariables();

  assert(false); // name not present in set
}

} // namespace zmp
} // namespace xpp
