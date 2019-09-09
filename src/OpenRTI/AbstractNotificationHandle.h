
#pragma once

struct AbstractNotificationHandle
{
  virtual ~AbstractNotificationHandle() {}
  virtual void Signal() = 0;
};
