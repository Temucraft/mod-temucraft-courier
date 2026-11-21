/*
 * mod-temucraft-courier: a pocket courier, your bank and mailbox anywhere. .courier calls a small helper that
 * follows you for Temucraft.Courier.Duration seconds; talk to it to open your bank or your mailbox. Only the player
 * who called it can use it. Not in battlegrounds or arenas, not in combat; once every Temucraft.Courier.Cooldown
 * seconds.
 *
 * Also the portable helpers (items, each with its own cooldown): Courier's Bell (calls the courier), Tailor's Folding
 * Mirror (a transmog mirror from mod-transmog), Artisan's Almanac (the trainer of one of your professions) and
 * Beastkeeper's Lantern (a stable master, hunters). Like the courier, each works only for the one who called it.
 */
#include "Chat.h"
#include "TemucraftModuleText.h"
#include "ChatCommand.h"
#include "Config.h"
#include "Creature.h"
#include "GameTime.h"
#include "Item.h"
#include "SharedDefines.h"
#include "Map.h"
#include "MotionMaster.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "TemporarySummon.h"
#include "WorldSession.h"
#include <unordered_map>

using namespace Acore::ChatCommands;

namespace
{
    enum : uint32 { ACTION_BANK = 1, ACTION_MAIL = 2 };

    struct CourierSettings { bool enable = true; uint32 duration = 180; uint32 cooldown = 300; bool bank = true; bool mail = true; uint32 creature = 608400; uint32 text = 608400; } ccfg;
    std::unordered_map<ObjectGuid, time_t> lastCall;
    std::unordered_map<ObjectGuid, ObjectGuid> courierOwner;   // courier and helpers -> player

    char const* T(WorldSession* s, uint32 id, char const* fb) { return TemucraftModuleText("mod-temucraft-courier", s, id, fb); }

    // the portable helpers
    enum HelperKind : uint8 { HELPER_BELL = 0, HELPER_MIRROR = 1, HELPER_ALMANAC = 2, HELPER_LANTERN = 3, HELPER_COUNT = 4 };
    struct HelperSettings
    {
        bool enable = true;
        uint32 item[HELPER_COUNT] = { 610140, 610141, 610142, 610143 };
        uint32 cooldown[HELPER_COUNT] = { 0, 600, 600, 300 };       // the Bell uses the courier's own cooldown
        uint32 duration = 180;
        uint32 mirror = 608401, lantern = 608402, almanacFirst = 608410;
    } hcfg;
    std::unordered_map<ObjectGuid, time_t> helperLast[HELPER_COUNT];
    constexpr uint32 ALMANAC_MENU = 608420;

    // Artisan's Almanac: profession skill -> offset of its trainer creature (almanacFirst + offset)
    struct Profession { uint32 skill; char const* name; };
    Profession const PROFESSIONS[] =
    {
        { SKILL_ALCHEMY, "Alchemy" }, { SKILL_BLACKSMITHING, "Blacksmithing" }, { SKILL_ENCHANTING, "Enchanting" },
        { SKILL_ENGINEERING, "Engineering" }, { SKILL_HERBALISM, "Herbalism" }, { SKILL_INSCRIPTION, "Inscription" },
        { SKILL_JEWELCRAFTING, "Jewelcrafting" }, { SKILL_LEATHERWORKING, "Leatherworking" }, { SKILL_MINING, "Mining" },
        { SKILL_SKINNING, "Skinning" }, { SKILL_TAILORING, "Tailoring" }, { SKILL_COOKING, "Cooking" },
        { SKILL_FIRST_AID, "First Aid" }, { SKILL_FISHING, "Fishing" },
    };

    bool CanCallHelper(Player* player)
    {
        return !player->IsInCombat() && player->IsAlive() && !player->GetMap()->IsBattlegroundOrArena() && !player->IsInFlight();
    }

    // summons an owned helper next to the player for hcfg.duration seconds
    Creature* SummonHelper(Player* player, uint32 entry)
    {
        float x, y, z;
        player->GetClosePoint(x, y, z, player->GetObjectSize(), 2.0f, float(M_PI) / 2);
        TempSummon* helper = player->SummonCreature(entry, x, y, z, player->GetAngle(x, y) + float(M_PI), TEMPSUMMON_TIMED_DESPAWN, hcfg.duration * IN_MILLISECONDS);
        if (!helper)
            return nullptr;
        courierOwner[helper->GetGUID()] = player->GetGUID();
        helper->SetFacingToObject(player);
        return helper;
    }

    // drops cooldowns that ran out, so the maps stay small
    void PruneCooldowns(time_t now)
    {
        for (uint8 k = 0; k < HELPER_COUNT; ++k)
            for (auto it = helperLast[k].begin(); it != helperLast[k].end();)
                it = now - it->second >= time_t(hcfg.cooldown[k]) ? helperLast[k].erase(it) : std::next(it);
        for (auto it = lastCall.begin(); it != lastCall.end();)
            it = now - it->second >= time_t(ccfg.cooldown) ? lastCall.erase(it) : std::next(it);
    }

    bool IsHelper(uint32 entry)
    {
        return entry == hcfg.mirror || entry == hcfg.lantern || (entry >= hcfg.almanacFirst && entry < hcfg.almanacFirst + std::size(PROFESSIONS));
    }
}

class TemucraftCourier_Creature : public AllCreatureScript
{
public:
    TemucraftCourier_Creature() : AllCreatureScript("TemucraftCourier_Creature") { }

    bool CanCreatureGossipHello(Player* player, Creature* creature) override
    {
        if (IsHelper(creature->GetEntry()))
        {
            auto owner = courierOwner.find(creature->GetGUID());
            if (owner != courierOwner.end() && owner->second == player->GetGUID())
            {
                if (creature->GetEntry() == hcfg.lantern)
                {
                    player->GetSession()->SendStablePet(creature->GetGUID());
                    return true;
                }
                if (creature->GetEntry() >= hcfg.almanacFirst && creature->GetEntry() != hcfg.mirror)
                {
                    player->GetSession()->SendTrainerList(creature);
                    return true;
                }
                return false;   // the mirror: mod-transmog's own gossip
            }
            CloseGossipMenuFor(player);
            ChatHandler(player->GetSession()).SendSysMessage(T(player->GetSession(), 7, "This helper works for someone else."));
            return true;
        }
        if (creature->GetEntry() != ccfg.creature)
            return false;
        auto owner = courierOwner.find(creature->GetGUID());
        ClearGossipMenuFor(player);
        if (owner == courierOwner.end() || owner->second != player->GetGUID())
        {
            CloseGossipMenuFor(player);
            ChatHandler(player->GetSession()).SendSysMessage(TemucraftModuleText("mod-temucraft-courier", player->GetSession(), 1, "This courier works for someone else."));
            return true;
        }
        if (ccfg.bank)
            AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, TemucraftModuleText("mod-temucraft-courier", player, 2, "Open my bank."), GOSSIP_SENDER_MAIN, ACTION_BANK);
        if (ccfg.mail)
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, TemucraftModuleText("mod-temucraft-courier", player, 3, "Open my mailbox."), GOSSIP_SENDER_MAIN, ACTION_MAIL);
        SendGossipMenuFor(player, ccfg.text, creature->GetGUID());
        return true;
    }

    bool CanCreatureGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        if (creature->GetEntry() != ccfg.creature)
            return false;
        CloseGossipMenuFor(player);
        auto owner = courierOwner.find(creature->GetGUID());
        if (owner == courierOwner.end() || owner->second != player->GetGUID())
            return true;
        if (action == ACTION_BANK && ccfg.bank)
            player->GetSession()->SendShowBank(creature->GetGUID());
        else if (action == ACTION_MAIL && ccfg.mail)
            player->GetSession()->SendShowMailBox(creature->GetGUID());
        return true;
    }

    void OnCreatureRemoveWorld(Creature* creature) override
    {
        if (creature->GetEntry() == ccfg.creature || IsHelper(creature->GetEntry()))
            courierOwner.erase(creature->GetGUID());
    }
};

class TemucraftCourier_World : public WorldScript
{
public:
    TemucraftCourier_World() : WorldScript("TemucraftCourier_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        ccfg.enable = sConfigMgr->GetOption<bool>("Temucraft.Courier.Enable", true);
        ccfg.duration = sConfigMgr->GetOption<uint32>("Temucraft.Courier.Duration", 180);
        ccfg.cooldown = sConfigMgr->GetOption<uint32>("Temucraft.Courier.Cooldown", 300);
        ccfg.creature = sConfigMgr->GetOption<uint32>("Temucraft.Courier.Creature", 608400);
        ccfg.text = sConfigMgr->GetOption<uint32>("Temucraft.Courier.Text", 608400);
        ccfg.bank = sConfigMgr->GetOption<bool>("Temucraft.Courier.Bank", true);
        ccfg.mail = sConfigMgr->GetOption<bool>("Temucraft.Courier.Mail", true);
        hcfg.enable = sConfigMgr->GetOption<bool>("Temucraft.Courier.Helpers.Enable", true);
        hcfg.duration = sConfigMgr->GetOption<uint32>("Temucraft.Courier.Helpers.Duration", 180);
        hcfg.cooldown[HELPER_MIRROR] = sConfigMgr->GetOption<uint32>("Temucraft.Courier.Helpers.MirrorCooldown", 600);
        hcfg.cooldown[HELPER_ALMANAC] = sConfigMgr->GetOption<uint32>("Temucraft.Courier.Helpers.AlmanacCooldown", 600);
        hcfg.cooldown[HELPER_LANTERN] = sConfigMgr->GetOption<uint32>("Temucraft.Courier.Helpers.LanternCooldown", 300);
        hcfg.mirror = sConfigMgr->GetOption<uint32>("Temucraft.Courier.Helpers.MirrorCreature", 608401);
        hcfg.lantern = sConfigMgr->GetOption<uint32>("Temucraft.Courier.Helpers.LanternCreature", 608402);
        hcfg.almanacFirst = sConfigMgr->GetOption<uint32>("Temucraft.Courier.Helpers.AlmanacFirstCreature", 608410);
        for (uint8 i = 0; i < HELPER_COUNT; ++i)
            hcfg.item[i] = sConfigMgr->GetOption<uint32>("Temucraft.Courier.Helpers.Item" + std::to_string(i), 610140 + i);
    }
};

class TemucraftCourier_Commands : public CommandScript
{
public:
    TemucraftCourier_Commands() : CommandScript("TemucraftCourier_Commands") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "courier", HandleCourier, SEC_PLAYER, Console::No },
        };
        return commandTable;
    }

    static bool HandleCourier(ChatHandler* handler)
    {
        return CallCourier(handler->GetPlayer(), handler);
    }

    static bool CallCourier(Player* player, ChatHandler* handler)
    {
        if (!ccfg.enable)
            return true;
        if (player->IsInCombat() || !player->IsAlive() || player->GetMap()->IsBattlegroundOrArena() || player->IsInFlight())
        {
            handler->SendSysMessage(TemucraftModuleText("mod-temucraft-courier", handler, 4, "You can't call your courier right now."));
            return true;
        }
        time_t now = GameTime::GetGameTime().count();
        auto& last = lastCall[player->GetGUID()];
        if (!player->IsGameMaster() && last && now - last < time_t(ccfg.cooldown))
        {
            handler->PSendSysMessage(TemucraftModuleText("mod-temucraft-courier", handler, 5, "Your courier is on another errand: {} seconds."), ccfg.cooldown - uint32(now - last));
            return true;
        }
        float x, y, z;
        player->GetClosePoint(x, y, z, player->GetObjectSize(), 2.0f, float(M_PI) / 2);
        TempSummon* courier = player->SummonCreature(ccfg.creature, x, y, z, player->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN,
                                                     ccfg.duration * IN_MILLISECONDS);
        if (!courier)
            return true;
        last = now;
        courierOwner[courier->GetGUID()] = player->GetGUID();
        courier->GetMotionMaster()->MoveFollow(player, 2.0f, float(M_PI) / 2);
        handler->PSendSysMessage(TemucraftModuleText("mod-temucraft-courier", handler, 6, "Your courier is here for {} seconds: talk to it for your bank or your mail."), ccfg.duration);
        return true;
    }
};

// every helper item: what it does depends on which item it is (hcfg.item)
class item_temucraft_helper : public ItemScript
{
public:
    item_temucraft_helper() : ItemScript("item_temucraft_helper") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        ChatHandler handler(player->GetSession());
        uint8 kind = HELPER_COUNT;
        for (uint8 i = 0; i < HELPER_COUNT; ++i)
            if (hcfg.item[i] == item->GetEntry())
                kind = i;
        if (!hcfg.enable || kind == HELPER_COUNT)
            return true;
        if (kind == HELPER_BELL)
            return TemucraftCourier_Commands::CallCourier(player, &handler);
        if (!CanCallHelper(player))
        {
            handler.SendSysMessage(T(player->GetSession(), 8, "You can't call a helper right now."));
            return true;
        }
        if (kind == HELPER_LANTERN && player->getClass() != CLASS_HUNTER)
        {
            handler.SendSysMessage(T(player->GetSession(), 9, "Only hunters keep a stable."));
            return true;
        }
        time_t now = GameTime::GetGameTime().count();
        PruneCooldowns(now);
        time_t& last = helperLast[kind][player->GetGUID()];
        if (!player->IsGameMaster() && last && now - last < time_t(hcfg.cooldown[kind]))
        {
            handler.PSendSysMessage(T(player->GetSession(), 10, "Not ready yet: {} seconds."), hcfg.cooldown[kind] - uint32(now - last));
            return true;
        }
        if (kind == HELPER_ALMANAC)
        {
            ClearGossipMenuFor(player);
            uint32 known = 0;
            for (uint32 i = 0; i < std::size(PROFESSIONS); ++i)
                if (player->HasSkill(PROFESSIONS[i].skill))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, PROFESSIONS[i].name, GOSSIP_SENDER_MAIN, i + 1);
                    ++known;
                }
            if (!known)
            {
                handler.SendSysMessage(T(player->GetSession(), 11, "You know no profession yet."));
                return true;
            }
            player->PlayerTalkClass->GetGossipMenu().SetMenuId(ALMANAC_MENU);
            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
            return true;
        }
        if (SummonHelper(player, kind == HELPER_MIRROR ? hcfg.mirror : hcfg.lantern))
        {
            last = now;
            handler.PSendSysMessage(T(player->GetSession(), 12, "Your helper is here for {} seconds."), hcfg.duration);
        }
        return true;
    }
};

// the Almanac's list of professions
class TemucraftHelpers_Player : public PlayerScript
{
public:
    TemucraftHelpers_Player() : PlayerScript("TemucraftHelpers_Player") { }

    void OnPlayerGossipSelect(Player* player, uint32 menu, uint32 /*sender*/, uint32 action) override
    {
        if (menu != ALMANAC_MENU)
            return;
        CloseGossipMenuFor(player);
        if (!action || action > std::size(PROFESSIONS) || !player->HasSkill(PROFESSIONS[action - 1].skill) || !CanCallHelper(player))
            return;
        time_t now = GameTime::GetGameTime().count();
        time_t& last = helperLast[HELPER_ALMANAC][player->GetGUID()];
        if (!player->IsGameMaster() && last && now - last < time_t(hcfg.cooldown[HELPER_ALMANAC]))
            return;
        if (Creature* trainer = SummonHelper(player, hcfg.almanacFirst + action - 1))
        {
            last = now;
            player->GetSession()->SendTrainerList(trainer);
        }
    }
};

void AddTemucraftCourierScripts()
{
    new item_temucraft_helper();
    new TemucraftHelpers_Player();
    new TemucraftCourier_Creature();
    new TemucraftCourier_World();
    new TemucraftCourier_Commands();
}
