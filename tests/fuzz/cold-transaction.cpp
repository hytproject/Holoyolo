// Copyright (c) 2017-2019, The Monero Project
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "include_base_utils.h"
#include "file_io_utils.h"
#include "cryptonote_basic/blobdatatype.h"
#include "cryptonote_basic/cryptonote_basic.h"
#include "cryptonote_basic/cryptonote_format_utils.h"
#include "wallet/wallet2.h"
#include "fuzzer.h"

static tools::wallet2 wallet;

BEGIN_INIT_SIMPLE_FUZZER()
  static const char * const spendkey_hex = "0b4f47697ec99c3de6579304e5f25c68b07afbe55b71d99620bf6cbf4e45a80f";
  crypto::secret_key spendkey;
  epee::string_tools::hex_to_pod(spendkey_hex, spendkey);

  wallet.init("", boost::none, boost::asio::ip::tcp::endpoint{}, 0, true, epee::net_utils::ssl_support_t::e_ssl_support_disabled);
  wallet.set_subaddress_lookahead(1, 1);
  wallet.generate("", "", spendkey, true, false);
END_INIT_SIMPLE_FUZZER()

BEGIN_SIMPLE_FUZZER()
  std::string s = std::string("\x01\x16serialization::archive") + std::string((const char*)buf, len);
  tools::wallet2::unsigned_tx_set exported_txs;
  std::stringstream iss;
  iss << s;
  boost::archive::portable_binary_iarchive ar(iss);
  ar >> exported_txs;
  std::vector<tools::wallet2::pending_tx> ptx;
  bool success = wallet.sign_tx(exported_txs, "/tmp/cold-transaction-test-signed", ptx);
  std::cout << (success ? "signed" : "error") << std::endl;
END_SIMPLE_FUZZER()
