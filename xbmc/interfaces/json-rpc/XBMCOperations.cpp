/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "XBMCOperations.h"

#include "ServiceBroker.h"
#include "messaging/ApplicationMessenger.h"
#include "powermanagement/PowerManager.h"
#include "utils/Variant.h"

using namespace JSONRPC;

JSONRPC_STATUS CXBMCOperations::GetInfoLabels(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  std::vector<std::string> info;

  for (unsigned int i = 0; i < parameterObject["labels"].size(); i++)
  {
    std::string field = parameterObject["labels"][i].asString();
    StringUtils::ToLower(field);

    info.push_back(parameterObject["labels"][i].asString());
  }

  if (!info.empty())
  {
    std::vector<std::string> infoLabels;
    CServiceBroker::GetAppMessenger()->SendMsg(TMSG_GUI_INFOLABEL, -1, -1,
                                               static_cast<void*>(&infoLabels), "", info);

    for (unsigned int i = 0; i < info.size(); i++)
    {
      if (i >= infoLabels.size())
        break;
      result[info[i]] = infoLabels[i];
    }
  }

  return OK;
}

JSONRPC_STATUS CXBMCOperations::GetInfoBooleans(const std::string &method, ITransportLayer *transport, IClient *client, const CVariant &parameterObject, CVariant &result)
{
  std::vector<std::string> info;

  bool CanControlPower = (client->GetPermissionFlags() & ControlPower) > 0;

  for (unsigned int i = 0; i < parameterObject["booleans"].size(); i++)
  {
    std::string field = parameterObject["booleans"][i].asString();
    StringUtils::ToLower(field);

    // Need to override power management of whats in infomanager since jsonrpc
    // have a security layer aswell.
    if (field == "system.canshutdown")
      result[parameterObject["booleans"][i].asString()] = (CServiceBroker::GetPowerManager().CanPowerdown() && CanControlPower);
    else if (field == "system.canpowerdown")
      result[parameterObject["booleans"][i].asString()] = (CServiceBroker::GetPowerManager().CanPowerdown() && CanControlPower);
    else if (field == "system.cansuspend")
      result[parameterObject["booleans"][i].asString()] = (CServiceBroker::GetPowerManager().CanSuspend() && CanControlPower);
    else if (field == "system.canhibernate")
      result[parameterObject["booleans"][i].asString()] = (CServiceBroker::GetPowerManager().CanHibernate() && CanControlPower);
    else if (field == "system.canreboot")
      result[parameterObject["booleans"][i].asString()] = (CServiceBroker::GetPowerManager().CanReboot() && CanControlPower);
    else
      info.push_back(parameterObject["booleans"][i].asString());
  }

  if (!info.empty())
  {
    std::vector<bool> infoLabels;
    CServiceBroker::GetAppMessenger()->SendMsg(TMSG_GUI_INFOBOOL, -1, -1,
                                               static_cast<void*>(&infoLabels), "", info);
    for (unsigned int i = 0; i < info.size(); i++)
    {
      if (i >= infoLabels.size())
        break;
      result[info[i].c_str()] = CVariant(infoLabels[i]);
    }
  }

  return OK;
}
