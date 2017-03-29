/**************************************************************************/
/*!
    @file     BLECentralService.cpp
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2017, Adafruit Industries (adafruit.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#include "bluefruit.h"

// Last service that is discovered
BLECentralService* BLECentralService::lastService = NULL;

void BLECentralService::_init(void)
{
  _discovered = false;
}

BLECentralService::BLECentralService(void)
  : uuid()
{
  _init();
}

BLECentralService::BLECentralService(BLEUuid bleuuid)
  : uuid(bleuuid)
{
  _init();
}


bool BLECentralService::begin(void)
{
  // Add UUID128 if needed
  uuid.begin();

  lastService = this;
  (void) Bluefruit.Central._registerService(this);

  return true;
}

bool BLECentralService::discover(uint16_t conn_handle)
{
  VERIFY( Bluefruit.Discovery.discoverService(conn_handle, *this) );
  _discovered = true;
  return true;
}

bool BLECentralService::discovered(void)
{
  return _discovered;
}

void BLECentralService::disconnect(void)
{
  _discovered = false;
  // inherited service may want to clean up its own characteristic
}