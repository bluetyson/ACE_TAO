// $Id$
#include "Factories_Define.h"

ACE_RCSID(lib, TAO_Notify_Tests_Factories_Define, "$Id$")

#include "Name.h"
#include "Command_Factory_T.h"

#include "EventChannel_Command.h"
#include "Application_Command.h"
#include "SupplierAdmin_Command.h"
#include "ConsumerAdmin_Command.h"
#include "Periodic_Supplier_Command.h"
#include "Periodic_Consumer_Command.h"
#include "Filter_Command.h"

TAO_Notify_Tests_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_Notify_Tests_EventChannel_Command,TAO_Notify_Tests_Name::event_channel_command_factory)
TAO_Notify_Tests_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_Notify_Tests_Application_Command,TAO_Notify_Tests_Name::application_command_factory)
TAO_Notify_Tests_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_Notify_Tests_SupplierAdmin_Command,TAO_Notify_Tests_Name::supplier_admin_command_factory)
TAO_Notify_Tests_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_Notify_Tests_ConsumerAdmin_Command,TAO_Notify_Tests_Name::consumer_admin_command_factory)
TAO_Notify_Tests_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_Notify_Tests_Periodic_Supplier_Command,TAO_Notify_Tests_Name::periodic_supplier_command_factory)
TAO_Notify_Tests_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_Notify_Tests_Periodic_Consumer_Command,TAO_Notify_Tests_Name::periodic_consumer_command_factory)
TAO_Notify_Tests_COMMAND_FACTORY_DEFINE(TAO_NOTIFY_TEST,TAO_Notify_Tests_Filter_Command,TAO_Notify_Tests_Name::filter_command_factory)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_Tests_Command_Factory_T<TAO_Notify_Tests_EventChannel_Command>;
template class TAO_Notify_Tests_Command_Factory_T<TAO_Notify_Tests_Application_Command>;
template class TAO_Notify_Tests_Command_Factory_T<TAO_Notify_Tests_SupplierAdmin_Command>;
template class TAO_Notify_Tests_Command_Factory_T<TAO_Notify_Tests_ConsumerAdmin_Command>;
template class TAO_Notify_Tests_Command_Factory_T<TAO_Notify_Tests_Periodic_Supplier_Command>;
template class TAO_Notify_Tests_Command_Factory_T<TAO_Notify_Tests_Periodic_Consumer_Command>;
template class TAO_Notify_Tests_Command_Factory_T<TAO_Notify_Tests_Filter_Command>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_Tests_Command_Factory_T<TAO_Notify_Tests_EventChannel_Command>
#pragma instantiate TAO_Notify_Tests_Command_Factory_T<TAO_Notify_Tests_Application_Command>
#pragma instantiate TAO_Notify_Tests_Command_Factory_T<TAO_Notify_Tests_SupplierAdmin_Command>
#pragma instantiate TAO_Notify_Tests_Command_Factory_T<TAO_Notify_Tests_ConsumerAdmin_Command>
#pragma instantiate TAO_Notify_Tests_Command_Factory_T<TAO_Notify_Tests_Periodic_Supplier_Command>
#pragma instantiate TAO_Notify_Tests_Command_Factory_T<TAO_Notify_Tests_Periodic_Consumer_Command>
#pragma instantiate TAO_Notify_Tests_Command_Factory_T<TAO_Notify_Tests_Filter_Command>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
