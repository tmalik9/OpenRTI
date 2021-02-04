
#include "ManagerFederate.h"
#include "RTI/Exception.h"
#include "StringUtils.h"
#include <signal.h>
#include <cstring>

void helpmenu(const char* progname)
{
  printf("Usage: %s --address <IP-ADDRESS>\n", progname);
  printf("\nIP-ADDRESS:  Address where rtinode can be reached. Example:" \
         " 127.0.0.1. 127.0.0.1 is the default setting for IP-ADDRESS.");
}

ManagerFederate* federate = nullptr;

#ifdef _WIN32
static void DumpHeap()
{
  _CrtDumpMemoryLeaks();
}

static int initHeapDump = atexit( DumpHeap);
#endif

extern "C" {

static void sighandler(int sig)
{
  if (federate != nullptr)
  {
    federate->setDone();
  }
}

}

int main(int argc, char* argv[])
{
  ::signal(SIGTERM, sighandler);
  ::signal(SIGINT, sighandler);

  std::string rtiAddress = "127.0.0.1";
  std::string federateName = "";
  std::string federationName = "";
  std::string timeImplName = "HLAinteger64Time";
  for (int i = 0; i < argc; i++)
  {
    if (strcmp(argv[i], "--address") == 0)
    {
      rtiAddress = argv[i + 1];
    }
    else if (strcmp(argv[i], "--name") == 0)
    {
      federateName = argv[i + 1];
    }
    else if (strcmp(argv[i], "--federation") == 0)
    {
      federationName = argv[i + 1];
    }
    else if (strcmp(argv[i], "--float") == 0)
    {
      timeImplName = "HLAfloat64Time";;
    }
    else if (strcmp(argv[i], "--help") == 0)
    {
      helpmenu(argv[0]);
      exit(0);
    }
  }
  // create and run the federate
  try {
    federate = new ManagerFederate();
    federate->initialize(rtiAddress, federationName, timeImplName);
    federate->runFederate();
  }
  catch( rti1516e::Exception& e)
  {
    printf("%S", e.what().c_str());
  }
  catch( std::exception& e)
  {
    printf("std::exception: %s", e.what());
  }
  // clean up
  delete federate;
  return 0;
}
