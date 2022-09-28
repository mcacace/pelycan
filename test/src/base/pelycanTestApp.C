//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "pelycanTestApp.h"
#include "pelycanApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

InputParameters
pelycanTestApp::validParams()
{
  InputParameters params = pelycanApp::validParams();
  return params;
}

pelycanTestApp::pelycanTestApp(InputParameters parameters) : MooseApp(parameters)
{
  pelycanTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

pelycanTestApp::~pelycanTestApp() {}

void
pelycanTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  pelycanApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"pelycanTestApp"});
    Registry::registerActionsTo(af, {"pelycanTestApp"});
  }
}

void
pelycanTestApp::registerApps()
{
  registerApp(pelycanApp);
  registerApp(pelycanTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
pelycanTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  pelycanTestApp::registerAll(f, af, s);
}
extern "C" void
pelycanTestApp__registerApps()
{
  pelycanTestApp::registerApps();
}
