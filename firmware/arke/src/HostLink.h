#pragma once

#include "inttypes.h"

#include "yaacl.h"




// Init the UART link.
void InitHostLink();

// Process incoming packet from host
void ProcessHostLink();

// Prepares an upstreamn
int HostSendCANPacket(const yaacl_txn_t * txn);
