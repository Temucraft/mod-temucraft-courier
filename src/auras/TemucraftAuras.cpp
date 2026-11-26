/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 */

// Cosmetic auras: toy items that switch a look-only aura on and off (one at a time). The choice is saved per character
// and put back at login and after you come back from death. Hidden in battlegrounds and arenas (and in combat, if the
// config says so); it comes back by itself when you leave. The auras are stock visual spells with no effect on play.

#include "Chat.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "Item.h"
#include "Map.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "WorldSession.h"
#include "SpellAuras.h"
#include "TemucraftModuleText.h"
#include <mutex>
#include <unordered_map>

namespace
{
    constexpr uint32 PERM_USE = 103650;   // RBAC permission (db-auth SQL), linked to the player roles
    char const* const MODULE = "mod-temucraft-auras";

    struct Settings
    {
        bool enable = true;
        bool hideInPvP = true;
        bool hideInCombat = false;
    } cfg;

    // item entry -> aura spell (the same list as temucraft_auras.sql)
    std::unordered_map<uint32, uint32> const AURAS =
    {
        { 610110, 57901 }, { 610111, 70788 }, { 610112, 34401 }, { 610113, 34404 }, { 610114, 34399 },
        { 610115, 34403 }, { 610116, 55928 }, { 610117, 47840 }, { 610118, 43576 }, { 610119, 22518 },
        { 610120, 22576 }, { 610121, 22577 }, { 610122, 22578 }, { 610123, 22579 }, { 610124, 22580 },
        { 610125, 22581 }, { 610126, 34224 }, { 610127, 34225 }, { 610128, 34228 }, { 610129, 62192 },
        { 610130, 50200 }, { 610131, 62011 }, { 610132, 36113 }, { 610133, 36114 }, { 610134, 36169 },
        { 610135, 36178 }, { 610136, 12898 }, { 610137, 51444 },
    };

    // player -> aura spell (0 = none). Written from item use and read from player hooks on map threads (several at
    // once): only through Chosen() / SetChosen(), under chosenLock.
    std::mutex chosenLock;
    std::unordered_map<ObjectGuid::LowType, uint32> chosen;

    uint32 Chosen(ObjectGuid::LowType guid)
    {
        std::lock_guard<std::mutex> guard(chosenLock);
        auto it = chosen.find(guid);
        return it == chosen.end() ? 0 : it->second;
    }

    void SetChosen(ObjectGuid::LowType guid, uint32 spell)
    {
        std::lock_guard<std::mutex> guard(chosenLock);
        if (spell)
            chosen[guid] = spell;
        else
            chosen.erase(guid);
    }

    char const* T(Player* p, uint32 id, char const* fb) { return TemucraftModuleText(MODULE, p, id, fb); }

    bool IsOurAura(uint32 spell)
    {
        for (auto const& [item, s] : AURAS)
            if (s == spell)
                return true;
        return false;
    }

    // removes every aura of this list, except `keep`
    void ClearAll(Player* player, uint32 keep = 0)
    {
        for (auto const& [item, spell] : AURAS)
            if (spell != keep && player->HasAura(spell))
                player->RemoveAurasDueToSpell(spell);
    }

    bool Hidden(Player* player)
    {
        if (!player->IsAlive())
            return true;
        if (cfg.hideInPvP && (player->GetMap()->IsBattlegroundOrArena() || player->InBattleground() || player->InArena()))
            return true;
        if (cfg.hideInCombat && player->IsInCombat())
            return true;
        return false;
    }

    // shows or hides the chosen aura to match where the player is
    void Refresh(Player* player)
    {
        if (!player || !player->IsInWorld())
            return;
        uint32 spell = cfg.enable ? Chosen(player->GetGUID().GetCounter()) : 0;
        if (!spell || Hidden(player))
        {
            ClearAll(player);
            return;
        }
        ClearAll(player, spell);
        if (!player->HasAura(spell))
            if (Aura* aura = player->AddAura(spell, player))
            {
                aura->SetMaxDuration(-1);
                aura->SetDuration(-1);
            }
    }

    void Save(Player* player, uint32 spell)
    {
        uint32 guid = player->GetGUID().GetCounter();
        SetChosen(guid, spell);
        if (spell)
        {
            CharacterDatabase.Execute("REPLACE INTO `mod_temucraft_character_setting` (`guid`, `setting`, `value`) VALUES ({}, 'auras', {})", guid, spell);
        }
        else
        {
            CharacterDatabase.Execute("DELETE FROM `mod_temucraft_character_setting` WHERE `guid` = {} AND `setting` = 'auras'", guid);
        }
    }
}

class TemucraftAuras_Player : public PlayerScript
{
public:
    TemucraftAuras_Player() : PlayerScript("TemucraftAuras_Player") { }

    void OnPlayerLogin(Player* player) override
    {
        uint32 guid = player->GetGUID().GetCounter();
        SetChosen(guid, 0);
        if (QueryResult result = CharacterDatabase.Query("SELECT `value` FROM `mod_temucraft_character_setting` WHERE `guid` = {} AND `setting` = 'auras'", guid))
        {
            uint32 spell = uint32((*result)[0].Get<int32>());
            if (IsOurAura(spell))
                SetChosen(guid, spell);
        }
        Refresh(player);
    }

    void OnPlayerLogout(Player* player) override { SetChosen(player->GetGUID().GetCounter(), 0); }
    void OnPlayerMapChanged(Player* player) override { Refresh(player); }
    void OnPlayerResurrect(Player* player, float /*restore*/, bool& /*sickness*/) override { Refresh(player); }
    void OnPlayerRemoveFromBattleground(Player* player, Battleground* /*bg*/) override { Refresh(player); }

    void OnPlayerEnterCombat(Player* player, Unit* /*enemy*/) override
    {
        if (cfg.hideInCombat)
            Refresh(player);
    }

    void OnPlayerLeaveCombat(Player* player) override
    {
        if (cfg.hideInCombat)
            Refresh(player);
    }
};

// every aura item: switches its aura on, or off when it is already the chosen one
class item_temucraft_aura : public ItemScript
{
public:
    item_temucraft_aura() : ItemScript("item_temucraft_aura") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        ChatHandler handler(player->GetSession());
        auto entry = AURAS.find(item->GetEntry());
        if (!cfg.enable || entry == AURAS.end())
            return true;
        if (!player->GetSession()->HasPermission(PERM_USE))
        {
            handler.SendSysMessage(T(player, 4, "You can't use this here."));
            return true;
        }
        if (Chosen(player->GetGUID().GetCounter()) == entry->second)

        {
            Save(player, 0);
            Refresh(player);
            handler.SendSysMessage(T(player, 1, "Aura off."));
            return true;
        }
        Save(player, entry->second);
        Refresh(player);
        handler.SendSysMessage(Hidden(player)
            ? T(player, 2, "Aura chosen. It shows once you leave battlegrounds, arenas or combat.")
            : T(player, 3, "Aura on. Use the same toy again to switch it off."));
        return true;
    }
};

class TemucraftAuras_World : public WorldScript
{
public:
    TemucraftAuras_World() : WorldScript("TemucraftAuras_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        cfg.enable = sConfigMgr->GetOption<bool>("Temucraft.Auras.Enable", true);
        cfg.hideInPvP = sConfigMgr->GetOption<bool>("Temucraft.Auras.HideInPvP", true);
        cfg.hideInCombat = sConfigMgr->GetOption<bool>("Temucraft.Auras.HideInCombat", false);
    }
};

// a deleted character's chosen aura (mod_temucraft_character_setting 'auras') goes with it
// OnPlayerDeleteFromDB: only when the character is really removed (not a CharDelete.Method = 1 unlink, which can be
// restored), and in the same transaction as the core's own deletes.
class TemucraftAuras_Delete : public PlayerScript
{
public:
    TemucraftAuras_Delete() : PlayerScript("TemucraftAuras_Delete", { PLAYERHOOK_ON_DELETE_FROM_DB }) { }

    void OnPlayerDeleteFromDB(CharacterDatabaseTransaction trans, uint32 guid) override
    {
        trans->Append("DELETE FROM `mod_temucraft_character_setting` WHERE `guid` = {} AND `setting` = 'auras'", guid);
    }
};

void AddTemucraftAurasScripts()
{
    new TemucraftAuras_Player();
    new item_temucraft_aura();
    new TemucraftAuras_World();
    new TemucraftAuras_Delete();
}
