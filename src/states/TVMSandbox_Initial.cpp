#include "TVMSandbox_Initial.h"

#include "../TVMSandbox.h"

void TVMSandbox_Initial::configure(const mc_rtc::Configuration & config)
{
}

void TVMSandbox_Initial::start(mc_control::fsm::Controller & ctl_)
{
  auto & ctl = static_cast<TVMSandbox &>(ctl_);
}

bool TVMSandbox_Initial::run(mc_control::fsm::Controller & ctl_)
{
  auto & ctl = static_cast<TVMSandbox &>(ctl_);
  output("OK");
  return true;
}

void TVMSandbox_Initial::teardown(mc_control::fsm::Controller & ctl_)
{
  auto & ctl = static_cast<TVMSandbox &>(ctl_);
}

EXPORT_SINGLE_STATE("TVMSandbox_Initial", TVMSandbox_Initial)
