-- mod-temucraft-courier: the pocket courier (gossip, banker and mailbox flags; not attackable)
SET @Courier := 608400;
SET @CourierText := 608400;
DELETE FROM `creature_template` WHERE `entry` = @Courier;
INSERT INTO `creature_template` (`entry`, `name`, `subname`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `unit_class`, `unit_flags`, `type`, `type_flags`, `RegenHealth`, `flags_extra`, `VerifiedBuild`)
VALUES (@Courier, 'Pocket Courier', 'Bank and Mail', 1, 1, 35, 67239937, 1.2, 1.4, 1, 2, 7, 0, 1, 2, 0);
DELETE FROM `creature_template_model` WHERE `CreatureID` = @Courier;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES (@Courier, 0, 7039, 0.8, 1, 0);
DELETE FROM `npc_text` WHERE `ID` = @CourierText;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `Probability0`) VALUES
(@CourierText, 'Bank or post, $n? I carry both, so long as you don''t ask me to carry them far.', 'Bank or post, $n? I carry both, so long as you don''t ask me to carry them far.', 1);
