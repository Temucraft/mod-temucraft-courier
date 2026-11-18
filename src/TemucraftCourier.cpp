/*
 * mod-temucraft-courier: a pocket courier, your bank and mailbox anywhere. .courier calls a small helper that
 * follows you for Temucraft.Courier.Duration seconds; talk to it to open your bank or your mailbox. Only the player
 * who called it can use it. Not in battlegrounds or arenas, not in combat; once every Temucraft.Courier.Cooldown
 * seconds.
 */
#include "Chat.h"
#include "TemucraftModuleText.h"
#include "ChatCommand.h"
#include "Config.h"
#include "Creature.h"
#include "GameTime.h"
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
    constexpr uint32 NPC_COURIER = 608400;
    constexpr uint32 TEXT_COURIER = 608400;
    enum : uint32 { ACTION_BANK = 1, ACTION_MAIL = 2 };

    struct CourierSettings { bool enable = true; uint32 duration = 180; uint32 cooldown = 300; bool bank = true; bool mail = true; } ccfg;
    std::unordered_map<ObjectGuid, time_t> lastCall;
    std::unordered_map<ObjectGuid, ObjectGuid> courierOwner;   // courier -> player
}

class TemucraftCourier_Creature : public AllCreatureScript
{
public:
    TemucraftCourier_Creature() : AllCreatureScript("TemucraftCourier_Creature") { }

    bool CanCreatureGossipHello(Player* player, Creature* creature) override
    {
        if (creature->GetEntry() != NPC_COURIER)
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
        SendGossipMenuFor(player, TEXT_COURIER, creature->GetGUID());
        return true;
    }

    bool CanCreatureGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        if (creature->GetEntry() != NPC_COURIER)
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
        if (creature->GetEntry() == NPC_COURIER)
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
        ccfg.bank = sConfigMgr->GetOption<bool>("Temucraft.Courier.Bank", true);
        ccfg.mail = sConfigMgr->GetOption<bool>("Temucraft.Courier.Mail", true);
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
        if (!ccfg.enable)
            return true;
        Player* player = handler->GetPlayer();
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
        TempSummon* courier = player->SummonCreature(NPC_COURIER, x, y, z, player->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN,
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

void AddTemucraftCourierScripts()
{
    new TemucraftCourier_Creature();
    new TemucraftCourier_World();
    new TemucraftCourier_Commands();
}
