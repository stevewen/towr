/**
 @file    dynamic_constraint.h
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    Dec 5, 2016
 @brief   Brief description
 */

#ifndef XPP_XPP_OPT_SRC_DYNAMIC_CONSTRAINT_H_
#define XPP_XPP_OPT_SRC_DYNAMIC_CONSTRAINT_H_

#include <xpp/opt/linear_inverted_pendulum.h>
#include <xpp/opt/center_of_pressure.h>
#include <xpp/constraint.h>
#include <memory>

namespace xpp {
namespace opt {

class BaseMotion;

class DynamicConstraint : public Constraint {
public:
  using BaseMotionPtr = std::shared_ptr<BaseMotion>;
  using CopPtr        = std::shared_ptr<CenterOfPressure>;

  DynamicConstraint ();
  virtual ~DynamicConstraint ();

  void Init(const BaseMotionPtr&, const CopPtr&, double T, double dt);
  void UpdateConstraintValues () override;
  void UpdateBounds () override;

private:
  BaseMotionPtr com_motion_;
  CopPtr cop_;
  mutable LinearInvertedPendulum model_;

  std::vector<double> dts_;

  void UpdateJacobians() override;
  void UpdateJacobianWrtCop();
  void UpdateJacobianWrtCom();
  double kHeight_;
};

} /* namespace opt */
} /* namespace xpp */

#endif /* XPP_XPP_OPT_SRC_DYNAMIC_CONSTRAINT_H_ */