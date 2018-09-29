/***
  This file is part of pavucontrol.

  Copyright 2006-2008 Lennart Poettering
  Copyright 2009 Colin Guthrie

  pavucontrol is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  pavucontrol is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with pavucontrol. If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef sourceoutputwidget_h
#define sourceoutputwidget_h

#include "pavucontrol.h"

class MainWindow;

class SourceOutputWidget {
public:
    SourceOutputWidget();
    ~SourceOutputWidget(void);

    SourceOutputType type;

    std::string name;
    uint32_t index, clientIndex;
    uint32_t sourceIndex();
    virtual void moveSourceOutput(uint32_t defIndex);

private:
    uint32_t mSourceIndex;
};

#endif