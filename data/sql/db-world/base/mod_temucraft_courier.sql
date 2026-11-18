-- mod-temucraft-courier: the pocket courier (gossip, banker and mailbox flags; not attackable)
DELETE FROM `creature_template` WHERE `entry` = 608400;
INSERT INTO `creature_template` (`entry`, `name`, `subname`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `unit_class`, `unit_flags`, `type`, `type_flags`, `RegenHealth`, `flags_extra`, `VerifiedBuild`)
VALUES (608400, 'Pocket Courier', 'Bank and Mail', 1, 1, 35, 67239937, 1.2, 1.4, 1, 2, 7, 0, 1, 2, 0);
DELETE FROM `creature_template_model` WHERE `CreatureID` = 608400;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES (608400, 0, 7039, 0.8, 1, 0);
DELETE FROM `npc_text` WHERE `ID` = 608400;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `Probability0`) VALUES
(608400, 'Bank or post, $n? I carry both, so long as you don''t ask me to carry them far.', 'Bank or post, $n? I carry both, so long as you don''t ask me to carry them far.', 1);
