// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

#ifndef GENESIS_MINE
#define GENESIS_MINE false
#endif
//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    void Mine(CBlock *pBlock) {
        
      CBlock block=*pBlock;
      cout<<block.ToString();

      // start with this nonce
      block.nNonce   =  0;

      //cout<<block.ToString();
      printf("Figure out valid hash and nonce for block\n");
      printf("block.GetHash() == %s\n", block.GetHash().ToString().c_str());
      printf("block.hashMerkleRoot == %s\n", block.hashMerkleRoot.ToString().c_str());
      printf("block.nTime = %u \n", block.nTime);
      printf("block.nNonce = %u \n", block.nNonce);

      // This will figure out a valid hash and Nonce if you're
      // creating a different genesis block:
      uint256 hashTarget = CBigNum().SetCompact(block.nBits).getuint256();
      printf("* Mining block...\n");
      printf("    Target %s\n", hashTarget.ToString().c_str());

      uint64_t nStart = GetTimeMillis();

      while (block.GetHash() > hashTarget)
         {
             if ((block.nNonce & 0xFFF) == 0)
             {
                 printf("nonce= %08X hash= %s\n",
                      block.nNonce, block.GetHash().ToString().c_str());
             }

             ++block.nNonce;
             if (block.nNonce == 0)
             {
                 printf("NONCE WRAPPED, incrementing time");
                 ++block.nTime;
             }
         }

     cout<<"Mining target met!!!\n";
     cout<<block.ToString();

      if (CheckProofOfWork(block.GetHash(), block.nBits)) {
         printf("* Solved block nonce %u hash 0x%s time %u\n",
           block.nNonce, block.GetHash().ToString().c_str(), block.nTime);
         printf("* Mining took %llu minutes\n", (unsigned long long) (GetTimeMillis() - nStart)/60000);
      }
      //cout<<block.ToString();
    }

    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.

        pchMessageStart[0] = 0xcf;
        pchMessageStart[1] = 0xd1;
        pchMessageStart[2] = 0xe8;
        pchMessageStart[3] = 0xea;
        vAlertPubKey = ParseHex("04219bef90531b2bfa82a03cf3e93f4281bf0bd9be79150ed93a3203f4d552d6a4732582fce6c94b7ca30459f4f9a84996f1a96833451cb0b13625893fa5a1c154");


        nDefaultPort = 8338;
        nRPCPort = 8339;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);
        nLastPOWBlock = BLOCKS_PER_DAY * DAYS_PER_YEAR * 128;

        static const char* hash160SeedAddress = "0553721f7507c9d43b618656b52918aa503b7f67"; // 1VALsigUQFqRAh8jBd1Q4K4n2kWq6PtSM

        static const uint256 _hashGenesisBlockMerkleRoot("0x0538153e4c67a03c7ba7e588354bbdc0f8031a60d87556ae1979f8e6b5a0938f");
        static const uint256 _hashGenesisBlock("0x00000b5e18c0bcce94a9f88751b9fafd18b4dc11f351539cc1da6fbba04cb0df");


        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        //
        const char* pszTimestamp = "Valorbit http://www.wired.com/2015/11/connecting-worlds-poorest-the-best-hope-for-ending-poverty/";
        std::vector<CTxIn> vin;
        vin.resize(1); 
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        
        CTransaction txNew(1, 0, vin, vout, 0);
        txNew.nTime = 1446501600;

        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = txNew.nTime;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 2469539;

        if (genesis.hashMerkleRoot != _hashGenesisBlockMerkleRoot)
        {
           cout<<"GENESIS block:\n"<<genesis.ToString();
        }

        assert(genesis.hashMerkleRoot == _hashGenesisBlockMerkleRoot);

        hashGenesisBlock = genesis.GetHash();
        if (GENESIS_MINE  && (hashGenesisBlock != _hashGenesisBlock )) {
                cout<<"Mining GENESIS block ...............\n";
                Mine(&genesis);
            }

        assert(hashGenesisBlock == _hashGenesisBlock);

        CTransaction txSeed;
        const char* pszTimestampSeed = "2015-11-02@valorbit.com";
        txSeed.nTime = genesis.nTime + 1;
        static const uint256 _hashSeedBlockMerkleRoot("0x1016a3b0898340769914d906d4edde0bf5c5a7da177bc83254bb5478ec78938a");
        static const uint256 _hashSeedBlock("0x000007e37b0e556cf82169d6485df7fe3c1a07214a4108a9e3da575a83a2668e");
        seed.nNonce   = 247609;
        txSeed.vin.resize(1);
        txSeed.vout.resize(1);
        txSeed.vin[0].scriptSig = CScript() << 486604799 << CBigNum(9999) << vector<unsigned char>((const unsigned char*)pszTimestampSeed, (const unsigned char*)pszTimestampSeed + strlen(pszTimestampSeed));
        txSeed.vout[0].SetEmpty();
        txSeed.vout[0].nValue= SEED_MONEY;
        txSeed.vout[0].scriptPubKey = CScript() << OP_DUP << OP_HASH160 << ParseHex(hash160SeedAddress) << OP_EQUALVERIFY << OP_CHECKSIG;

        seed.vtx.push_back(txSeed);
        seed.hashPrevBlock = genesis.GetHash();
        seed.hashMerkleRoot = seed.BuildMerkleTree();
        seed.nVersion = 1;
        seed.nTime    = txSeed.nTime;
        seed.nBits    = bnProofOfWorkLimit.GetCompact();

        if (seed.hashMerkleRoot != _hashSeedBlockMerkleRoot)
        {
           cout<<"SEED block:\n"<<seed.ToString();
        }

        assert(seed.hashMerkleRoot == _hashSeedBlockMerkleRoot);

        hashSeedBlock = seed.GetHash();
        if (GENESIS_MINE  && (hashSeedBlock != _hashSeedBlock )) {
           cout<<"Mining SEED block ...............\n";
           Mine(&seed);
        }

        assert(hashSeedBlock == _hashSeedBlock);

        vSeeds.push_back(CDNSSeedData("seed.valorbit.com", "seed.valorbit.com"));
        vSeeds.push_back(CDNSSeedData("seed2.valorbit.com", "seed2.valorbit.com"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(0);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(5);
        base58Prefixes[SECRET_KEY] =     list_of(128);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

     
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual const CBlock& SeedBlock() const { return seed; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    CBlock seed;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xe2;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xd8;
        pchMessageStart[3] = 0xc7;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        vAlertPubKey = ParseHex("04f84f9a4d41d3009d0ad1f9b30e234e8da2663631daa3ead20b6d761bd2e202725358400421d028e664990cafb53df80e890d3a40a23aed828ee1675bc2c1a644");
        nDefaultPort = 18338;
        nRPCPort = 18339;
        strDataDir = "testnet";
        nLastPOWBlock = 0x7fffffff;

        static const uint256 _hashGenesisBlock("0x00008d18e582741232b7dd943061dec5c8a53b7a5a62497a56d3bf65702c0a18");

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 54984;
        hashGenesisBlock = genesis.GetHash();
        //cout<<genesis.ToString();
        if (GENESIS_MINE  && (hashGenesisBlock != _hashGenesisBlock )) {
                printf("Mining GENESIS block on testnet ...............\n");
                Mine(&genesis);
        }

        assert(hashGenesisBlock == _hashGenesisBlock);

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = list_of(111);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(196);
        base58Prefixes[SECRET_KEY]     = list_of(239);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94);

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        nLastPOWBlock = 0x7fffffff;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0xe2;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xd9;
        pchMessageStart[3] = 0xc8;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        //genesis.nTime = 1411111111;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 3;
        nDefaultPort = 28338;
        strDataDir = "regtest";
        static const uint256 _hashGenesisBlock("0x0b687e82b23d3f22665adfc7895ac125218f504177f5735bebfd735a3c1a5df5");

        hashGenesisBlock = genesis.GetHash();
        //cout<<genesis.ToString();
        if (GENESIS_MINE && (hashGenesisBlock != _hashGenesisBlock )) {
                printf("Mining GENESIS block on regtest ...............\n");
                Mine(&genesis);
        }

        assert(hashGenesisBlock == _hashGenesisBlock);

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
