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
                printf("Figure out valid hash and Nonce for block 3.3 \n");
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
                               printf("nonce %08X: hash = %s (target = %s)\n",
                                    block.nNonce, block.GetHash().ToString().c_str(), hashTarget.ToString().c_str());
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
                       printf("* Solved genesis block! nonce %u hash 0x%s time %u\n",
                         block.nNonce, block.GetHash().ToString().c_str(), block.nTime);
                       printf("* Mining took %lu minutes\n", (GetTimeMillis() - nStart)/60000);
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
        vAlertPubKey = ParseHex("");
        nDefaultPort = 7654;
        nRPCPort = 17654;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);

        static const char* hash160SeedAddress = "055372147866f59ecaf625f8577d39b4015c8780"; // 1VALs3VmH24rHYiPgwKD897w2FjdvVfUk
        //static const char* hash160GenesisAddress = "0553716e9c88172a42c19529d14a51e650047a34"; //1VALgqxbb66Vwr98RqnYkncksCQAKM9dy
        static const uint256 _hashGenesisBlockMerkleRoot("0xbd5ba50e6d20f38fdda75cd525574e9847f81f6592fb24a417ba9f07956d452f");
        static const uint256 _hashGenesisBlock("0x00000b34c4eba95817c9884657e325925a19bba5d39f70a81e911e1fb9f1a535");
        static const uint256 _hashSeedBlockMerkleRoot("0xd6497c87e3b9645815a83a96de31a8845689aae8b8eb96dab06389369e3c91ef");
        static const uint256 _hashSeedBlock("0x000009e5a674bfc1df69c34a152ddca5758c2ee9dde1204f0ff1af947abcec00");


        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        //
        const char* pszTimestamp = "ValorCoin:www.wired.com/2015/05/internet-anything-brain-monitors-going-mainstream-despite-skepticism/";
        std::vector<CTxIn> vin;
        
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        CTransaction txNew(1, 1430830000, vin, vout, 0);
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = txNew.nTime;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 2530046;

        if (genesis.hashMerkleRoot != _hashGenesisBlockMerkleRoot)
        {
           cout<<"GENESIS block:\n"<<genesis.ToString();
        }

        assert(genesis.hashMerkleRoot == _hashGenesisBlockMerkleRoot);

        hashGenesisBlock = genesis.GetHash();
        if (true  && (hashGenesisBlock != _hashGenesisBlock )) {
                cout<<"Mining GENESIS block ...............\n";
                Mine(&genesis);
            }

        assert(hashGenesisBlock == _hashGenesisBlock);

        const char* pszTimestampSeed = "05-May-2015@seed.valorcoin.com";
        CTransaction txSeed;
        txSeed.nTime = genesis.nTime + 1;
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
        seed.nNonce   = 320794;

        if (seed.hashMerkleRoot != _hashSeedBlockMerkleRoot)
        {
           cout<<"SEED block:\n"<<seed.ToString();
        }

        assert(seed.hashMerkleRoot == _hashSeedBlockMerkleRoot);

        hashSeedBlock = seed.GetHash();
        if (true  && (hashSeedBlock != _hashSeedBlock )) {
           cout<<"Mining SEED block ...............\n";
           Mine(&seed);
        }

        assert(hashSeedBlock == _hashSeedBlock);

        vSeeds.push_back(CDNSSeedData("seed.valorcoin.com", "seed.valorcoin.com"));
        vSeeds.push_back(CDNSSeedData("seed2.valorcoin.com", "seed2.valorcoin.com"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(0);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(5);
        base58Prefixes[SECRET_KEY] =     list_of(128);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        nLastPOWBlock = 10000;
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
        vAlertPubKey = ParseHex("");
        nDefaultPort = 8765;
        nRPCPort = 18765;
        strDataDir = "testnet";
        static const uint256 _hashGenesisBlock("0x00003b270ba0f623a8815eb0c7f0d33b1976159df2f1a14d8f207a515b406f80");

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 182094;
        hashGenesisBlock = genesis.GetHash();
        //cout<<genesis.ToString();
        if (true  && (hashGenesisBlock != _hashGenesisBlock )) {
                printf("Mining GENESIS block on TestNet ...............\n");
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
        genesis.nNonce = 4;
        nDefaultPort = 18766;
        strDataDir = "regtest";
        static const uint256 _hashGenesisBlock("0x30ecba39552219d81ee85776a2b27e61189d554e99e38020f462930d12f496fc");

        hashGenesisBlock = genesis.GetHash();
        //cout<<genesis.ToString();
        if (true  && (hashGenesisBlock != _hashGenesisBlock )) {
                printf("Mining GENESIS block on RegNet ...............\n");
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
