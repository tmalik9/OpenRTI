/**
 * @file
 *
 * Copyright (C) 2013 Max Oberberger <max@oberbergers.de>
 *
 * Last modified: 2013 February 23, 20:05:02 by max
 *
 * This file is part of ba-hla.
 *
 * ba-hla is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ba-hla is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ba-hla.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TimeConstrainedFederate.h"
#include "RTI/Exception.h"

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
  std::string federateName, fomPath, rtiAddress;
  bool name = false, fom = false, address = false, iterationsGiven =false;
  unsigned int iterations = 20;
  for (int i = 0; i < argc; i++)
  {
    if (strcmp(argv[i], "--name") == 0)
    {
      federateName = argv[i + 1];
      name = true;
    }
    else if (strcmp(argv[i], "--fom") == 0)
    {
      fom = true;
      fomPath = argv[i + 1];
    }
    else if (strcmp(argv[i], "--address") == 0)
    {
      address = true;
      rtiAddress = argv[i + 1];
    }
    else if (strcmp(argv[i], "--iterations") == 0)
    {
      iterationsGiven = true;
      iterations = strtoul( argv[i + 1], nullptr, 10);
    }
    else if (strcmp(argv[i], "--help") == 0)
    {
      helpmenu(argv[0]);
      exit(0);
    }
  }
  // create and run the federate
  TimeConstrainedFederate* federate = nullptr;
  if (name && fom)
  {
    try {
      if (!address) { rtiAddress = "127.0.0.1"; }
      federate = new TimeConstrainedFederate();
      federate->initialize(rtiAddress, federateName, fomPath, false, true);
      federate->runFederate(iterations);
    }
    catch( Exception& e)
    {
      printf("%S", e.what().c_str());
    }
    catch( std::exception& e)
    {
      printf("std::exception: %s", e.what());
    }
  }
  else
  {
    if (!name) { printf("No Federatename given\n"); }
    if (!fom) { printf("No FOM location given\n"); }
    helpmenu(argv[0]);
    exit(1);
  }
  // clean up
  delete federate;
  return 0;
}
