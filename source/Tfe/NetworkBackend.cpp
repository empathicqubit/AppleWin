/*
AppleWin : An Apple //e emulator for Windows

Copyright (C) 2022, Andrea Odetti

AppleWin is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

AppleWin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with AppleWin; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "StdAfx.h"
#include "NetworkBackend.h"

#include "Tfe/tfearch.h"

#ifdef _MSC_VER
#define PCAP_FILENAME "AppleWin.pcap"
#else
#define PCAP_FILENAME "/tmp/AppleWin.pcap"
#endif

NetworkBackend::~NetworkBackend()
{
}

namespace
{

  class PCAPDumper
  {
  private:
    pcap_t *myPcap;
    pcap_dumper_t *myDumper;

  public:
    PCAPDumper()
    {
      tfe_pcap_dump_open(PCAP_FILENAME, 16384, myPcap, myDumper);
    }

    ~PCAPDumper()
    {
      tfe_pcap_dump_close(myPcap, myDumper);
    }

    void dumpPacket(
        const int txlength, /* Frame length */
        uint8_t *txframe /* Data */)
    {
      tfe_pcap_dump(myDumper, txlength, txframe);
    }
  };

  PCAPDumper dumper;
}

void NetworkBackend::dumpPacket(
    const int txlength, /* Frame length */
    uint8_t *txframe /* Data */)
{
  dumper.dumpPacket(txlength, txframe);
}
