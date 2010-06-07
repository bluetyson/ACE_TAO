// $Id$
#include "Hello_i.h"
#include "HelloC.h"
#include "Client_ORBInitializer.h"

#include "tao/ORBInitializer_Registry.h"
#include "tao/PI_Server/PI_Server.h"
#include "ace/Task.h"
#include "ace/OS.h"
#include "ace/Get_Opt.h"
#include <iostream>

const ACE_TCHAR *ior_output_file = ACE_TEXT("client.ior");
const ACE_TCHAR *server1_ior = ACE_TEXT("file://server1.ior");
const ACE_TCHAR *server2_ior = ACE_TEXT("file://server2.ior");
const ACE_TCHAR *server1_shutdown_ior = ACE_TEXT("file://server1_shutdown.ior");
const ACE_TCHAR *server2_shutdown_ior = ACE_TEXT("file://server2_shutdown.ior");

CORBA::ORB_var orb;
Demo::HelloWorld_var server1_shuutdownObj;
Demo::HelloWorld_var server2_shuutdownObj;
int test_duration = 30;

class ClientTask : public ACE_Task_Base
{
public:
  ClientTask () {};
  ~ ClientTask () {};
   
  virtual int svc (void)
  {
    CORBA::Object_var helloObj = orb->string_to_object(server1_ior);

    Demo::HelloWorld_var hello = Demo::HelloWorld::_narrow(helloObj);

    if (CORBA::is_nil(hello))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("hello reference is nil\n")));
      return 0;
    }
    else
    {
    
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ClientTask start.\n")));
      int i = 0;
      ACE_Time_Value due = ACE_OS::gettimeofday () + ACE_Time_Value (test_duration);
      
      while (ACE_OS::gettimeofday () < due)
      {
        try
        {
          ++ i;
          ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)======client calling server1 sayHello %d\n"), i));
          const char* pMsg = " server1 say Hello"; 
          hello->sayHello(pMsg) ;
          ACE_OS::sleep(2);
        }
        catch (const CORBA::OBJECT_NOT_EXIST &)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("sayHello() got OBJECT_NOT_EXIST exception !!\n")));
        }
        catch (const CORBA::INV_OBJREF &)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("sayHello() got INV_OBJREF exception !!\n")));
        }
        catch (const CORBA::COMM_FAILURE &)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("sayHello() got COMM_FAILURE exception !!\n")));
        }
        catch (const CORBA::Exception &ex) 
        {
          ex._tao_print_exception ("sayHello()");
        }
      }
    }
    return 0;
  }
};


int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "t:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 't':
        test_duration = ACE_OS::atoi(get_opts.opt_arg());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                              "usage:  %s"
                              " -t <test duration>"
                              "\n",
                              argv [0]),
            -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try 
  {
    PortableInterceptor::ORBInitializer_ptr temp_initializer;

    ACE_NEW_RETURN (temp_initializer,
                    Client_ORBInitializer(),
                    -1);  
    PortableInterceptor::ORBInitializer_var initializer =
      temp_initializer;

    PortableInterceptor::register_orb_initializer (initializer.in ());

    orb = CORBA::ORB_init (argc, argv);
    
    // Initialize options based on command-line arguments.
    if (parse_args (argc, argv))
      {
        return -1;
      }
        
    CORBA::Object_var shutdownObj = orb->string_to_object(server1_shutdown_ior);

    server1_shuutdownObj = Demo::HelloWorld::_narrow(shutdownObj);

    if (CORBA::is_nil(server1_shuutdownObj))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("server1 shutdown object reference is nil\n")));
      return 1;
    }

    shutdownObj = orb->string_to_object(server2_shutdown_ior);

    server2_shuutdownObj = Demo::HelloWorld::_narrow(shutdownObj);

    if (CORBA::is_nil(server2_shuutdownObj))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("server2 shutdown object reference is nil\n")));
      return 1;
    }
        
    ClientTask task;
    if (task.activate(THR_NEW_LWP | THR_JOINABLE, 1) != 0) 
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "Cannot activate client threads\n"),
                        1);
    }

    task.wait ();
    
    server1_shuutdownObj->shutdown ();
    server2_shuutdownObj->shutdown ();
    
    server1_shuutdownObj = Demo::HelloWorld::_nil ();
    server2_shuutdownObj = Demo::HelloWorld::_nil ();
      
    orb->destroy ();
    
    orb = CORBA::ORB::_nil ();
  }
  catch (const CORBA::Exception &e) 
  {
    std::cerr << "Unexpected exception: " << e << std::endl;
    return 1;
  }

  return 0;
}
