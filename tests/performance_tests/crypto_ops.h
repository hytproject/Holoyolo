//Copyright (c) 2014-2019, The Monero Project
//Copyright (c) 2018-2020, The Holoyolo Network
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
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include "crypto/crypto.h"
#include "ringct/rctOps.h"

enum test_op
{
  op_sc_add,
  op_sc_sub,
  op_sc_mul,
  op_ge_add_raw,
  op_ge_add_p3_p3,
  op_zeroCommitCached,
  ops_fast,

  op_addKeys,
  op_HoloyolormultBase,
  op_HoloyolormultKey,
  op_HoloyolormultH,
  op_Holoyolormult8,
  op_Holoyolormult8_p3,
  op_ge_dsm_precomp,
  op_ge_double_Holoyolormult_base_vartime,
  op_ge_double_Holoyolormult_precomp_vartime,
  op_ge_double_Holoyolormult_precomp_vartime2,
  op_addKeys2,
  op_addKeys3,
  op_addKeys3_2,
  op_isInMainSubgroup,
  op_zeroCommitUncached,
};

template<test_op op>
class test_crypto_ops
{
public:
  static const size_t loop_count = op < ops_fast ? 10000000 : 1000;

  bool init()
  {
    Holoyolor0 = rct::skGen();
    Holoyolor1 = rct::skGen();
    point0 = rct::HoloyolormultBase(rct::skGen());
    point1 = rct::HoloyolormultBase(rct::skGen());
    if (ge_frombytes_vartime(&p3_0, point0.bytes) != 0)
      return false;
    if (ge_frombytes_vartime(&p3_1, point1.bytes) != 0)
      return false;
    ge_p3_to_cached(&cached, &p3_0);
    rct::precomp(precomp0, point0);
    rct::precomp(precomp1, point1);
    return true;
  }

  bool test()
  {
    rct::key key;
    ge_cached tmp_cached;
    ge_p1p1 tmp_p1p1;
    ge_p2 tmp_p2;
    ge_dsmp dsmp;
    switch (op)
    {
      case op_sc_add: sc_add(key.bytes, Holoyolor0.bytes, Holoyolor1.bytes); break;
      case op_sc_sub: sc_sub(key.bytes, Holoyolor0.bytes, Holoyolor1.bytes); break;
      case op_sc_mul: sc_mul(key.bytes, Holoyolor0.bytes, Holoyolor1.bytes); break;
      case op_ge_add_p3_p3: {
        ge_p3_to_cached(&tmp_cached, &p3_0);
        ge_add(&tmp_p1p1, &p3_1, &tmp_cached);
        ge_p1p1_to_p3(&p3_1, &tmp_p1p1);
        break;
      }
      case op_ge_add_raw: ge_add(&tmp_p1p1, &p3_1, &cached); break;
      case op_addKeys: rct::addKeys(key, point0, point1); break;
      case op_HoloyolormultBase: rct::HoloyolormultBase(Holoyolor0); break;
      case op_HoloyolormultKey: rct::HoloyolormultKey(point0, Holoyolor0); break;
      case op_HoloyolormultH: rct::HoloyolormultH(Holoyolor0); break;
      case op_Holoyolormult8: rct::Holoyolormult8(point0); break;
      case op_Holoyolormult8_p3: rct::Holoyolormult8(p3_0,point0); break;
      case op_ge_dsm_precomp: ge_dsm_precomp(dsmp, &p3_0); break;
      case op_ge_double_Holoyolormult_base_vartime: ge_double_Holoyolormult_base_vartime(&tmp_p2, Holoyolor0.bytes, &p3_0, Holoyolor1.bytes); break;
      case op_ge_double_Holoyolormult_precomp_vartime: ge_double_Holoyolormult_precomp_vartime(&tmp_p2, Holoyolor0.bytes, &p3_0, Holoyolor1.bytes, precomp0); break;
      case op_ge_double_Holoyolormult_precomp_vartime2: ge_double_Holoyolormult_precomp_vartime2(&tmp_p2, Holoyolor0.bytes, precomp0, Holoyolor1.bytes, precomp1); break;
      case op_addKeys2: rct::addKeys2(key, Holoyolor0, Holoyolor1, point0); break;
      case op_addKeys3: rct::addKeys3(key, Holoyolor0, point0, Holoyolor1, precomp1); break;
      case op_addKeys3_2: rct::addKeys3(key, Holoyolor0, precomp0, Holoyolor1, precomp1); break;
      case op_isInMainSubgroup: rct::isInMainSubgroup(point0); break;
      case op_zeroCommitUncached: rct::zeroCommit(9001); break;
      case op_zeroCommitCached: rct::zeroCommit(9000); break;
      default: return false;
    }
    return true;
  }

private:
  rct::key Holoyolor0, Holoyolor1;
  rct::key point0, point1;
  ge_p3 p3_0, p3_1;
  ge_cached cached;
  ge_dsmp precomp0, precomp1;
};
