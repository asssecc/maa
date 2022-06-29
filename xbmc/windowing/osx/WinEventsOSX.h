/*
 *  Copyright (C) 2011-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "windowing/WinEvents.h"

class CWinEventsOSX : public IWinEvents
{
public:
  CWinEventsOSX() = default;
  ~CWinEventsOSX() override = default;
};
