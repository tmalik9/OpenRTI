/* -*-c++-*- OpenRTI - Copyright (C) 2009-2016 Mathias Froehlich
 *
 * This file is part of OpenRTI.
 *
 * OpenRTI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * OpenRTI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenRTI.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <cstdlib>
#include <string>
#include <memory>
#include <vector>
#include <iostream>

#include <RTI/RTIambassadorFactory.h>
#include <RTI/RTIambassador.h>

#include <Options.h>
#include <StringUtils.h>

#include "RTI1516ETestLib.h"
#include "NetworkServer.h"

int
main(int argc, char* argv[])
{
  std::wstring fullPathNameToTheFDDfile(L"fdd.xml");

  OpenRTI::Options options(argc, argv);
  std::vector<std::wstring> args;
  while (options.next("O:")) {
    switch (options.getOptChar()) {
    case 'O':
      fullPathNameToTheFDDfile = OpenRTI::localeToUcs(options.getArgument());
      break;
    case '\0':
      args.push_back(OpenRTI::localeToUcs(options.getArgument()));
      break;
    }
  }

  std::list<OpenRTI::SocketAddress> addressList = OpenRTI::SocketAddress::resolve("localhost", "0", true);
  if (addressList.empty())
    return EXIT_FAILURE;

  // Set up a stream socket for the server connect
  bool success = false;
  OpenRTI::SocketAddress listenAddress;
  while (!addressList.empty()) {
    OpenRTI::SocketAddress address = addressList.front();
    addressList.pop_front();

    // At first set up a listening server and determine an unused socket address
    OpenRTI::SharedPtr<OpenRTI::NetworkServer> listeningNetworkServer = OpenRTI::MakeShared<OpenRTI::NetworkServer>();
    try {
      listenAddress = listeningNetworkServer->listenInet(address, 1);
      success = true;
    } catch (const OpenRTI::Exception&) {
      continue;
    }

    // Now retract the listen, which must result in an immediate connection refused
    // Note that this address should not even be reused within this time so this test is (hopefully?) safe.
    listeningNetworkServer.reset();

    std::wstring federationExecutionName(L"rti://");
    federationExecutionName += OpenRTI::localeToUcs(listenAddress.getNumericName());
    //federationExecutionName += L"/nothingthere";

    OpenRTI::RTI1516ESimpleAmbassador ambassador;
    ambassador.setConnectWaitTimeout(10000);
    OpenRTI::Clock maxtime = OpenRTI::Clock::now() + OpenRTI::Clock::fromSeconds(20);
    try {
      ambassador.connect(federationExecutionName);
    } catch (const rti1516e::ConnectionFailed& e) {
      std::wcout << L"connect: rti1516e::ConnectionFailed(" << federationExecutionName << L"): \"" << e.what() << L"\" (expected)" << std::endl;
    } catch (const rti1516e::Exception& e) {
      std::wcout << L"connect: rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
      return EXIT_FAILURE;
    } catch (...) {
      std::wcout << L"connect: Unknown Exception!" << std::endl;
      return EXIT_FAILURE;
    }
    if (OpenRTI::Clock::now() > maxtime) {
      std::wcout << L"Timeout!" << std::endl;
      return EXIT_FAILURE;
    }

    // all of them must not run into any timeout!! FIXME check this!!

    // All those must not work
    // Note that rti1516e does not have a specific way to report connection failures.
    // Se we check here aginst RTIinternalError
    try {
      ambassador.createFederationExecution(federationExecutionName, fullPathNameToTheFDDfile);
      std::wcout << L"createFederationExecution: Must not be able to connect to the given address" << std::endl;
      return EXIT_FAILURE;
    } catch (const rti1516e::NotConnected& e) {
      std::wcout << L"createFederationExecution: rti1516e::NotConnected: \"" << e.what() << L"\" (expected)" << std::endl;
    } catch (const rti1516e::Exception& e) {
      std::wcout << L"createFederationExecution: createFederationExecution: rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
      return EXIT_FAILURE;
    } catch (...) {
      std::wcout << L"createFederationExecution: Unknown Exception!" << std::endl;
      return EXIT_FAILURE;
    }
    if (maxtime < OpenRTI::Clock::now()) {
      std::wcout << L"createFederationExecution: Timeout!" << std::endl;
      return EXIT_FAILURE;
    }

    maxtime = OpenRTI::Clock::now() + OpenRTI::Clock::fromSeconds(20);

    try {
      ambassador.joinFederationExecution(L"federate", federationExecutionName);
      std::wcout << L"joinFederationExecution: Must not be able to connect to the given address" << std::endl;
      return EXIT_FAILURE;
    } catch (const rti1516e::NotConnected& e) {
      std::wcout << L"joinFederationExecution: rti1516e::NotConnected: \"" << e.what() << L"\" (expected)" << std::endl;
    } catch (const rti1516e::Exception& e) {
      std::wcout << L"joinFederationExecution: rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
      return EXIT_FAILURE;
    } catch (...) {
      std::wcout << L"joinFederationExecution: Unknown Exception!" << std::endl;
      return EXIT_FAILURE;
    }
    if (maxtime < OpenRTI::Clock::now()) {
      std::wcout << L"joinFederationExecution: Timeout!" << std::endl;
      return EXIT_FAILURE;
    }

    maxtime = OpenRTI::Clock::now() + OpenRTI::Clock::fromSeconds(20);

    try {
      ambassador.resignFederationExecution(rti1516e::NO_ACTION);
      std::wcout << L"resignFederationExecution: Must not be able to connect to the given address" << std::endl;
      return EXIT_FAILURE;
    } catch (const rti1516e::NotConnected& e) {
      std::wcout << L"resignFederationExecution: rti1516e::NotConnected: \"" << e.what() << L"\" (expected)" << std::endl;
    } catch (const rti1516e::Exception& e) {
      std::wcout << L"resignFederationExecution: rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
      return EXIT_FAILURE;
    } catch (...) {
      std::wcout << L"resignFederationExecution: Unknown Exception!" << std::endl;
      return EXIT_FAILURE;
    }
    if (maxtime < OpenRTI::Clock::now()) {
      std::wcout << L"resignFederationExecution: Timeout!" << std::endl;
      return EXIT_FAILURE;
    }

    maxtime = OpenRTI::Clock::now() + OpenRTI::Clock::fromSeconds(20);

    try {
      ambassador.destroyFederationExecution(federationExecutionName);
      std::wcout << L"Must not be able to connect to the given address" << std::endl;
      return EXIT_FAILURE;
    } catch (const rti1516e::NotConnected& e) {
      std::wcout << L"destroyFederationExecution: rti1516e::NotConnected: \"" << e.what() << L"\" (expected)" << std::endl;
    } catch (const rti1516e::Exception& e) {
      std::wcout << e.what() << std::endl;
      return EXIT_FAILURE;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return EXIT_FAILURE;
    }
    if (maxtime < OpenRTI::Clock::now()) {
      std::wcout << L"Timeout expecrienced!" << std::endl;
      return EXIT_FAILURE;
    }
  }
  if (!success) {
    std::wcout << L"Could not get any listening server to check against refused connects!" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
