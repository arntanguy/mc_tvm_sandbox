#pragma once

#include <mc_control/fsm/State.h>
#include <tvm/LinearizedControlProblem.h>
#include <tvm/scheme/WeightedLeastSquares.h>
#include <tvm/Variable.h>

#include "SolverTestFunctions.h"


struct TVMTutorial_ProblemWritingExample : mc_control::fsm::State
{
  TVMTutorial_ProblemWritingExample();
  void start(mc_control::fsm::Controller & ctl) override;
  bool run(mc_control::fsm::Controller & ctl) override;
  void teardown(mc_control::fsm::Controller & ctl) override;

  protected:
    tvm::VariablePtr x;
    tvm::VariablePtr q;
    std::shared_ptr<Simple2dRobotEE> g;
    tvm::LinearizedControlProblem lpb;
    tvm::scheme::WeightedLeastSquares solver;

    const double radius_ = 1;
    const double length_ = 0.01; // for gui only
    const Eigen::Vector2d center_ = Eigen::Vector2d(0, 0);
};

