#include "TVMTutorial_ProblemWritingExample.h"

#include <mc_control/fsm/Controller.h>
#include <tvm/LinearizedControlProblem.h>
#include <tvm/Variable.h>
#include <tvm/function/IdentityFunction.h>
#include <tvm/solver/defaultLeastSquareSolver.h>
#include <tvm/task_dynamics/Proportional.h>
#include <tvm/task_dynamics/ProportionalDerivative.h>
#include <tvm/task_dynamics/VelocityDamper.h>
#include <mc_rtc/gui.h>


using namespace Eigen;
using namespace tvm;
using namespace tvm::requirements;
using namespace tvm::task_dynamics;
using std::make_shared;

// This is an interactive implementation of the example given in the TVM documentation.
// Please refer to https://jrl.cnrs.fr/tvm/doxygen/HEAD/problemWritingExample.html for further information.

TVMTutorial_ProblemWritingExample::TVMTutorial_ProblemWritingExample()
  :
  // Creating the resolution scheme
  solver(solver::DefaultLSSolverOptions{})
{
  // Creating variable x in R^2 and initialize it to [0.5, 0.5]
  Space R2(2);
  x = R2.createVariable("x");
  x << 0.5, 0.5;

  // Creating variable q in R^3 and initialize it to [0.4, -0.6, -0.1]
  Space R3(3);
  q = R3.createVariable("q");
  q->set(Vector3d(0.4, -0.6, -0.1));


  // Creating the functions
  g = make_shared<Simple2dRobotEE>(q, Vector2d(-3, 0), Vector3d(1, 1, 1)); // g(q)
  auto idx = make_shared<function::IdentityFunction>(x);                        // I x
  auto e1 = make_shared<Difference>(g, idx);                                    // e_1(q,x) = g(q) - x
  auto e2 = make_shared<SphereFunction>(x, center_, radius_);                  // e_2(x)

  Vector3d b = Vector3d::Constant(tvm::constant::pi / 2);

  // Creating the linear control problem
  auto t1 = lpb.add(e1 == 0., Proportional(2), PriorityLevel(0));
  auto t2 = lpb.add(e2 == 0., Proportional(2), PriorityLevel(0));
  auto t3 = lpb.add(-b <= q <= b, VelocityDamper({1, 0.01, 0, 0.1}), PriorityLevel(0));
  auto t4 = lpb.add(dot(q) == 0., {PriorityLevel(1), AnisotropicWeight(Vector3d(10, 2, 1))});
}

void TVMTutorial_ProblemWritingExample::start(mc_control::fsm::Controller & ctl)
{
  mc_rtc::gui::PointConfig pc;
  pc.scale = 0.05;
  pc.color = mc_rtc::gui::Color::Green;

  ctl.gui()->addElement(
      this,
      {},
      mc_rtc::gui::Cylinder("Circle",
        { radius_, length_ },
        [this]() { return Eigen::Vector3d{center_.x(), center_.y(), 0}; }
        ),
      mc_rtc::gui::Point3D("g(q)",
        pc,
        [this]() -> Eigen::Vector3d
        {
          auto v = g->value();
          return Eigen::Vector3d{v[0], v[1], 0};
        }),
      mc_rtc::gui::Label("q", [this]() { return q->value(); }),
      mc_rtc::gui::Label("dot(q)", [this]() { return dot(q)->value(); }),
      mc_rtc::gui::Label("norm of dot(q)", [this]() { return dot(q)->value().norm(); })
      );

  output("OK");
}

bool TVMTutorial_ProblemWritingExample::run(mc_control::fsm::Controller & ctl)
{
  auto dt = ctl.timeStep / 10;

  solver.solve(lpb);
  x->set(x->value() + dot(x)->value() * dt);
  q->set(q->value() + dot(q)->value() * dt);

  // std::cout << "At q = " << q->value().transpose() << ",\n    e1 = " << e1->value().transpose() << "\n"
  //           << "   convergence in " << i << " iterations" << std::endl;
  return false;
}

void TVMTutorial_ProblemWritingExample::teardown(mc_control::fsm::Controller & ctl)
{
  ctl.gui()->removeElements(this);
}

EXPORT_SINGLE_STATE("TVMTutorial_ProblemWritingExample", TVMTutorial_ProblemWritingExample)

