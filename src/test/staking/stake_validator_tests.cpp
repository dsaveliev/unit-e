// Copyright (c) 2018 The Unit-e developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <blockchain/blockchain_genesis.h>
#include <blockchain/blockchain_parameters.h>
#include <hash.h>
#include <staking/stake_validator.h>
#include <test/test_unite.h>
#include <uint256.h>

#include <test/test_unite.h>
#include <test/test_unite_mocks.h>
#include <boost/test/unit_test.hpp>

namespace {

struct Fixture {

  blockchain::Parameters parameters = blockchain::Parameters::MainNet();
  std::unique_ptr<blockchain::Behavior> b =
      blockchain::Behavior::NewFromParameters(parameters);

  mocks::ActiveChainMock active_chain_mock;
};

}  // namespace

BOOST_AUTO_TEST_SUITE(stake_validator_tests)

BOOST_AUTO_TEST_CASE(check_kernel) {
  Fixture fixture;
  const auto stake_validator = staking::StakeValidator::New(fixture.b.get(), &fixture.active_chain_mock);
  const uint256 kernel;
  const auto difficulty = blockchain::GenesisBlockBuilder().Build(fixture.parameters).nBits;
  BOOST_CHECK(stake_validator->CheckKernel(1, kernel, difficulty));
}

BOOST_AUTO_TEST_CASE(check_kernel_fail) {
  Fixture fixture;
  const auto stake_validator = staking::StakeValidator::New(fixture.b.get(), &fixture.active_chain_mock);
  const uint256 kernel = uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
  const auto difficulty = blockchain::GenesisBlockBuilder().Build(fixture.parameters).nBits;
  BOOST_CHECK(!stake_validator->CheckKernel(1, kernel, difficulty));
}

BOOST_AUTO_TEST_CASE(remember_and_forget) {
  Fixture fixture;
  const auto stake_validator = staking::StakeValidator::New(fixture.b.get(), &fixture.active_chain_mock);
  const uint256 txid = uint256S("000000000000000000000000e6b8347d447e02ed383a3e96986815d576fb2a5a");
  const COutPoint stake(txid, 2);
  LOCK(stake_validator->GetLock());
  BOOST_CHECK(!stake_validator->IsPieceOfStakeKnown(stake));
  stake_validator->RememberPieceOfStake(stake);
  BOOST_CHECK(stake_validator->IsPieceOfStakeKnown(stake));
  stake_validator->ForgetPieceOfStake(stake);
  BOOST_CHECK(!stake_validator->IsPieceOfStakeKnown(stake));
}

BOOST_AUTO_TEST_CASE(check_stake) {
  Fixture fixture;
  const auto stake_validator = staking::StakeValidator::New(fixture.b.get(), &fixture.active_chain_mock);

  CBlock block;

  uint256 stake_txid = uint256S("7f6b062da8f3c99f302341f06879ff94db0b7ae291b38438846c9878b58412d4");
  std::uint32_t stake_ix = 7;

  CScript stake_script_sig;
  COutPoint stake_ref(stake_txid, stake_ix);
  CTxIn stake(stake_ref, stake_script_sig);

  CMutableTransaction tx;
  tx.vin = {stake};

  LOCK(fixture.active_chain_mock.GetLock());
  stake_validator->CheckStake(block);


}

BOOST_AUTO_TEST_SUITE_END()