// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <accounts/db.h>

#include <iostream>
#include <fstream>

bool CManagedAccountDB::AddAccount(CTxDestination address, CManagedAccountData account) {
    std::cout << __func__ << ":" << __LINE__ << "> Adding node: " << EncodeDestination(address) << " -> ";  // FIXME
    std::cout << account.ToString() << std::endl;  // FIXME
    std::cout << __func__ << ":" << __LINE__ << "> ~~~~~~ " << std::endl;  // FIXME

    accountDB.insert(std::make_pair(address,account));

    SaveToDisk();
    return true;
}

bool CManagedAccountDB::UpdateAccount(CTxDestination address, CManagedAccountData account) {
    // FIXME improve logging
    std::cout << __func__ << ":" << __LINE__ << "> Updating node: " << EncodeDestination(address) << std::endl;
    std::cout << __func__ << ":" << __LINE__ << ">   Parent: " ;
    std::cout << (IsValidDestination(account.GetParent())? EncodeDestination(account.GetParent()) : "-none-");
    std::cout << std::endl;
    std::cout << __func__ << ":" << __LINE__ << ">    Roles: ";
    std::cout << ValueFromRoles(account.GetRoles()).get_str() << std::endl;
    std::cout << __func__ << ":" << __LINE__ << "> Children: ";

    for(const auto& child:account.GetChildren()) {
        std::cout << EncodeDestination(child) << " ";
    }
    std::cout << std::endl;
    std::cout << __func__ << ":" << __LINE__ << "> ~~~~~~ " << std::endl;

    if(!ExistsAccountForAddress(address)) {
        return AddAccount(address, account);
    } else {
        accountDB.at(address) = account;
        SaveToDisk();
        return true;
    }
}

bool CManagedAccountDB::DeleteAccount(CTxDestination address) {
    // FIXME Not yet implemented
    SaveToDisk();
    return false;
}

bool CManagedAccountDB::GetAccountByAddress(CTxDestination address, CManagedAccountData& account) {
    if(ExistsAccountForAddress(address)) {
        account = accountDB.at(address);
        return true;
    }

    return false;
}

bool CManagedAccountDB::ExistsAccountForAddress(CTxDestination address) {
    return (accountDB.find(address) != accountDB.end());
}

// Serialization methods
void CManagedAccountDB::SaveToDisk() {
    std::ofstream file;
    file.open(dbFilePath.c_str());

    std::map<CTxDestination, CManagedAccountData>::iterator iter;

    for(iter=accountDB.begin(); iter!=accountDB.end(); ++iter)
    {
        std::cout << __func__ << ":" << __LINE__ << "> Saving " << EncodeDestination(iter->first) << " == ";  // FIXME
        std::cout << (iter->second).ToString() << std::endl;  // FIXME
        file << EncodeDestination(iter->first) << std::endl;
        file << iter->second << std::endl;
    }

    file.close();
}

void CManagedAccountDB::LoadFromDisk() {
    std::ifstream file;
    std::string address;
    CManagedAccountData accountData;

    file.open(dbFilePath.c_str());

    while(file >> address >> accountData)
    {
        accountDB.insert(std::make_pair(DecodeDestination(address), accountData));
    }

    file.close();
    std::cout << __func__ << ":" << __LINE__ << "> " << accountDB.size() << " account(s) loaded" << std::endl;  // FIXME
}
