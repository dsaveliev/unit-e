// Copyright (c) 2018 The Unit-e developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <esperanza/vote.h>
#include <key.h>
#include <primitives/transaction.h>

#ifndef UNIT_E_TESTS_ESPERANZA_FINALIZATION_UTILS_H
#define UNIT_E_TESTS_ESPERANZA_FINALIZATION_UTILS_H

CTransaction CreateVoteTx(esperanza::Vote &vote, const CKey &spendableKey);

CTransaction CreateDepositTx(const CTransaction &spendableTx,
                             const CKey &spendableKey, CAmount amount);

CTransaction CreateLogoutTx(const CTransaction &spendableTx,
                            const CKey &spendableKey, CAmount amount);

CTransaction CreateWithdrawTx(const CTransaction &spendableTx,
                              const CKey &spendableKey, CAmount amount);

CTransaction CreateP2PKHTx(const CTransaction &spendableTx,
                           const CKey &spendableKey, CAmount amount);

#endif  // UNIT_E_TESTS_ESPERANZA_FINALIZATION_UTILS_H