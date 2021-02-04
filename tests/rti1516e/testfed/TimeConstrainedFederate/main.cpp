
#include "TimeConstrainedFederate.h"
#include "RTI/Exception.h"
#include <signal.h>

TimeConstrainedFederate* federate = nullptr;

extern "C" {

static void sighandler(int sig)
{
  if (federate != nullptr)
  {
    federate->setDone();
  }
}

}

void helpmenu(const char* progname)
{
  printf("Usage: %s --name <NAME> --address <IP-ADDRESS> --fom </PATH/TO/FOM>\n", progname);
  printf("\nNAME:        use a normal Name to identify federate. Example: exampleFederate");
  printf("\nIP-ADDRESS:  Address where rtinode can be reached. Example:" \
         " 127.0.0.1. 127.0.0.1 is the default setting for IP-ADDRESS.");
  printf("\nPATH/TO/FOM: Path where fom file can be reached. Example: /tmp/fom.xml");
}

int main(int argc, char* argv[])
{
  // check to see if we have a federate name
  //char *federateName = "exampleFederate";
  std::string federateName;
  std::string fomPath = "testfom.xml";
  std::string rtiAddress = "127.0.0.1";
  std::string federationName = "ExampleFederation";

  for (int i = 0; i < argc; i++)
  {
    if (strcmp(argv[i], "--name") == 0)
    {
      federateName = argv[i + 1]; i++;
    }
    else if (strcmp(argv[i], "--federation") == 0)
    {
      federationName = argv[i + 1]; i++;
    }
    else if (strcmp(argv[i], "--fom") == 0)
    {
      fomPath = argv[i + 1]; i++;
    }
    else if (strcmp(argv[i], "--address") == 0)
    {
      rtiAddress = argv[i + 1]; i++;
    }
    else if (strcmp(argv[i], "--help") == 0)
    {
      helpmenu(argv[0]);
      exit(0);
    }
  }

  ::signal(SIGTERM, sighandler);
  ::signal(SIGINT, sighandler);

  // create and run the federate
  try {
    federate = new TimeConstrainedFederate();
    federate->join(rtiAddress, federateName, fomPath, federationName, false, true);
    federate->run(1000);
    federate->disconnect();
  }
  catch( Exception& e)
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
