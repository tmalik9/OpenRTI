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
#include "Thread.h"

class Tester
{

  public:
    bool CreateServer()
    {
      serverThread = OpenRTI::MakeShared<OpenRTI::ServerThread>();
      serverThread->setupServer("127.0.0.1", OpenRTI::SocketAddress(), false);
      listenAddress = serverThread->getAddress();

      return true;
    }
    void DestroyServer()
    {
      serverThread->stopServer();
    }
    bool ConnectFail()
    {
      std::wstring federationExecutionName(L"rti://");
      federationExecutionName += OpenRTI::localeToUcs(listenAddress.getNumericName());
      ambassador.setConnectWaitTimeout(10000);
      OpenRTI::Clock maxtime = OpenRTI::Clock::now() + OpenRTI::Clock::fromSeconds(20);
      try
      {
        ambassador.connect(federationExecutionName);
      }
      catch (const rti1516e::ConnectionFailed&)
      {
        // should report this
      }
      catch (const rti1516e::Exception& e)
      {
        std::wcout << L"connect: rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"connect: Unknown Exception!" << std::endl;
        return false;
      }
      if (OpenRTI::Clock::now() > maxtime)
      {
        std::wcout << L"Timeout!" << std::endl;
        return false;
      }
    }
    bool ConnectSuccess()
    {
      std::wstring url(L"rti://127.0.0.1:14321");
      //url += L"127.0.0.1"; // OpenRTI::localeToUcs(listenAddress.getNumericName());
      ambassador.setConnectWaitTimeout(10000);
      ambassador.setOperationWaitTimeout(10000);
      OpenRTI::Clock maxtime = OpenRTI::Clock::now() + OpenRTI::Clock::fromSeconds(20);
      try
      {
        ambassador.connect(url);
        return true;
      }
      catch (const rti1516e::Exception& e)
      {
        std::wcout << L"connect to " << url << " failed: " << e.what() << L"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"connect: Unknown Exception!" << std::endl;
        return false;
      }
      //if (OpenRTI::Clock::now() > maxtime)
      //{
      //  std::wcout << L"Timeout!" << std::endl;
      //  return false;
      //}
    }
    bool CreateAndJoinSuccess(const std::wstring& federationExecutionName, const std::wstring& fullPathNameToTheFDDfile)
    {
      OpenRTI::Clock maxtime = OpenRTI::Clock::now() + OpenRTI::Clock::fromSeconds(20);
      try
      {
        ambassador.createFederationExecution(federationExecutionName, fullPathNameToTheFDDfile);
      }
      catch (const rti1516e::Exception& e)
      {
        std::wcout << L"createFederationExecution: createFederationExecution: rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"createFederationExecution: Unknown Exception!" << std::endl;
        return false;
      }
      if (maxtime < OpenRTI::Clock::now())
      {
        std::wcout << L"createFederationExecution: Timeout!" << std::endl;
        return false;
      }
      maxtime = OpenRTI::Clock::now() + OpenRTI::Clock::fromSeconds(20);
      try
      {
        ambassador.joinFederationExecution(L"federate", federationExecutionName);
      }
      catch (const rti1516e::Exception& e)
      {
        std::wcout << L"joinFederationExecution: rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"joinFederationExecution: Unknown Exception!" << std::endl;
        return false;
      }
      if (maxtime < OpenRTI::Clock::now())
      {
        std::wcout << L"joinFederationExecution: Timeout!" << std::endl;
        return false;
      }
      return true;
    }
    bool ResignAndDestroySuccess(const std::wstring& federationExecutionName)
    {
      OpenRTI::Clock maxtime = OpenRTI::Clock::now() + OpenRTI::Clock::fromSeconds(20);
      try
      {
        ambassador.resignFederationExecution(rti1516e::NO_ACTION);
      }
      catch (const rti1516e::Exception& e)
      {
        std::wcout << L"resignFederationExecution: rti1516e::Exception: \"" << e.what()
                   << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"resignFederationExecution: Unknown Exception!" << std::endl;
        return false;
      }
      if (maxtime < OpenRTI::Clock::now())
      {
        std::wcout << L"resignFederationExecution: Timeout!" << std::endl;
        return EXIT_FAILURE;
      }
      maxtime = OpenRTI::Clock::now() + OpenRTI::Clock::fromSeconds(20);
      try
      {
        ambassador.destroyFederationExecution(federationExecutionName);
      }
      catch (const rti1516e::Exception& e)
      {
        std::wcout << "destroyFederationExecution: " << e.what()
                   << std::endl;
        return EXIT_FAILURE;
      }
      catch (...)
      {
        std::wcout << L"destroyFederationExecution: Unknown Exception!" << std::endl;
        return EXIT_FAILURE;
      }
      if (maxtime < OpenRTI::Clock::now())
      {
        std::wcout << L"destroyFederationExecution: Timeout!" << std::endl;
        return EXIT_FAILURE;
      }
      return true;
    }
    bool ResignAndDestroyFail(const std::wstring& federationExecutionName)
    {
      OpenRTI::Clock maxtime = OpenRTI::Clock::now() + OpenRTI::Clock::fromSeconds(20);
      try
      {
        ambassador.resignFederationExecution(rti1516e::NO_ACTION);
        std::wcout << L"resignFederationExecution: unexpected success" << std::endl;
        return false;
      }
      catch (const rti1516e::NotConnected&)
      {
        // should report this
      }
      catch (const rti1516e::Exception& e)
      {
        std::wcout << L"resignFederationExecution: rti1516e::Exception: \"" << e.what()
                   << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"resignFederationExecution: Unknown Exception!" << std::endl;
        return EXIT_FAILURE;
      }
      if (maxtime < OpenRTI::Clock::now())
      {
        std::wcout << L"resignFederationExecution: Timeout!" << std::endl;
        return false;
      }
      maxtime = OpenRTI::Clock::now() + OpenRTI::Clock::fromSeconds(20);
      try
      {
        ambassador.destroyFederationExecution(federationExecutionName);
        std::wcout << L"destroyFederationExecution: unexpected success" << std::endl;
        return false;
      }
      catch (const rti1516e::NotConnected&)
      {
        // should report this
      }
      catch (const rti1516e::Exception& e)
      {
        std::wcout << "destroyFederationExecution: " << e.what()
                   << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"destroyFederationExecution: Unknown Exception!" << std::endl;
        return false;
      }
      if (maxtime < OpenRTI::Clock::now())
      {
        std::wcout << L"destroyFederationExecution: Timeout!" << std::endl;
        return EXIT_FAILURE;
      }
      return true;
    }
  public:
    OpenRTI::RTI1516ESimpleAmbassador ambassador;
  private:
    OpenRTI::SocketAddress listenAddress;
    OpenRTI::SharedPtr<OpenRTI::ServerThread> serverThread;
};

int
main(int argc, char* argv[])
{
  Tester tester;
  std::wstring fullPathNameToTheFDDfile(L"fdd.xml");
  OpenRTI::Options options(argc, argv);
  std::vector<std::wstring> args;
  while (options.next("O:"))
  {
    switch (options.getOptChar())
    {
      case 'O':
        fullPathNameToTheFDDfile = OpenRTI::localeToUcs(options.getArgument());
        break;
      case '\0':
        args.push_back(OpenRTI::localeToUcs(options.getArgument()));
        break;
    }
  }

  // TEST SEQUENCE
  if (!tester.CreateServer())
  {
    std::wcout << L" could not create NetworkServer" << std::endl;
    return EXIT_FAILURE;
  }
  if (!tester.ConnectSuccess())
  {
    std::wcout << L" could not connect to server" << std::endl;
    return EXIT_FAILURE;
  }
  if (!tester.CreateAndJoinSuccess(L"TestFederation", fullPathNameToTheFDDfile))
  {
    std::wcout << L" could not create and join federation" << std::endl;
    return EXIT_FAILURE;
  }
  tester.DestroyServer();
  while (!tester.ambassador.isConnectionLost())
  {
    tester.ambassador.evokeCallback(1.0);
  }
  if (!tester.ResignAndDestroyFail(L"TestFederation"))
  {
    std::wcout << L"resign/destroy unexpectedly succeeded" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
