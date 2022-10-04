#include "pelycanApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
pelycanApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  // Do not use legacy material output
  params.set<bool>("use_legacy_material_output") = false;
  return params;
}

pelycanApp::pelycanApp(InputParameters parameters) : MooseApp(parameters)
{
  pelycanApp::registerAll(_factory, _action_factory, _syntax);
}

pelycanApp::~pelycanApp() {}

void
pelycanApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAll(f, af, syntax);
  Registry::registerObjectsTo(f, {"pelycanApp"});
  Registry::registerActionsTo(af, {"pelycanApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
pelycanApp::registerApps()
{
  registerApp(pelycanApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
pelycanApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  pelycanApp::registerAll(f, af, s);
}
extern "C" void
pelycanApp__registerApps()
{
  pelycanApp::registerApps();
}
