-- mod-temucraft-courier: the portable helpers. Items 610140-610143 (sold on the Temucraft Curator's Helpers list,
-- vendor 608011 from mod-temucraft-shop), the mirror (608401, a copy of mod-transmog's npc 190010), the stable master
-- (608402) and one trainer per profession for the Almanac (608410-608423, trainers from creature_default_trainer).
SET @ITEM := 610140; SET @MIRROR := 608401; SET @LANTERN := 608402; SET @ALMANAC := 608410;

DELETE FROM `item_template` WHERE `entry` BETWEEN @ITEM AND @ITEM+3;
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`, `name`, `displayid`, `Quality`, `Flags`, `BuyCount`, `BuyPrice`, `SellPrice`, `InventoryType`, `ItemLevel`, `maxcount`, `stackable`, `spellid_1`, `spelltrigger_1`, `spellcharges_1`, `spellcooldown_1`, `bonding`, `description`, `Material`, `ScriptName`) VALUES
(@ITEM+0, 15, 0, -1, 'Courier''s Bell', 17911, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Rings for your Pocket Courier: your bank and mailbox, wherever you are.', 1, 'item_temucraft_helper'),
(@ITEM+1, 15, 0, -1, 'Tailor''s Folding Mirror', 46680, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Unfolds a wardrobe mirror for a few minutes: change your looks anywhere out of combat.', 1, 'item_temucraft_helper'),
(@ITEM+2, 15, 0, -1, 'Artisan''s Almanac', 1103, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Calls the trainer of one of your professions for a few minutes. Normal trainer prices.', 7, 'item_temucraft_helper'),
(@ITEM+3, 15, 0, -1, 'Beastkeeper''s Lantern', 2746, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Calls a stable master for a few minutes. Hunters only.', 1, 'item_temucraft_helper');

-- the mirror: mod-transmog's npc, so its own gossip runs (nothing happens without mod-transmog)
DELETE FROM `creature_template_model` WHERE `CreatureID` IN (@MIRROR, @LANTERN) OR `CreatureID` BETWEEN @ALMANAC AND @ALMANAC+13;
DELETE FROM `creature_template` WHERE `entry` IN (@MIRROR, @LANTERN) OR `entry` BETWEEN @ALMANAC AND @ALMANAC+13;
DELETE FROM `creature_default_trainer` WHERE `CreatureId` BETWEEN @ALMANAC AND @ALMANAC+13;
INSERT INTO `creature_template` (`entry`, `name`, `subname`, `gossip_menu_id`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `unit_class`, `unit_flags`, `type`, `flags_extra`, `ScriptName`)
SELECT @MIRROR, 'Wardrobe Mirror', 'Transmogrifier', `gossip_menu_id`, 1, 1, 35, `npcflag`, 1, 1, 1, 2 | 256 | 512, 7, 2, `ScriptName` FROM `creature_template` WHERE `entry` = 190010;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT @MIRROR, 0, `CreatureDisplayID`, `DisplayScale`, 1 FROM `creature_template_model` WHERE `CreatureID` = 190010 AND `Idx` = 0;

-- the stable master: gossip + stable master flags; the look of the Stormwind stable master
INSERT INTO `creature_template` (`entry`, `name`, `subname`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `unit_class`, `unit_flags`, `type`, `flags_extra`) VALUES
(@LANTERN, 'Lantern Keeper', 'Stable Master', 1, 1, 35, 1 | 4194304, 1, 1, 1, 2 | 256 | 512, 7, 2);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT @LANTERN, 0, `CreatureDisplayID`, `DisplayScale`, 1 FROM `creature_template_model` WHERE `CreatureID` = 6749 AND `Idx` = 0;

-- the Almanac's trainers, same order as PROFESSIONS in the code; each teaches what that stock trainer teaches
DROP TEMPORARY TABLE IF EXISTS `tmp_temucraft_almanac`;
CREATE TEMPORARY TABLE `tmp_temucraft_almanac` (`n` TINYINT UNSIGNED, `name` VARCHAR(40), `trainer` INT UNSIGNED);
INSERT INTO `tmp_temucraft_almanac` VALUES
(0, 'Alchemy', 65), (1, 'Blacksmithing', 59), (2, 'Enchanting', 94), (3, 'Engineering', 89), (4, 'Herbalism', 71),
(5, 'Inscription', 119), (6, 'Jewelcrafting', 111), (7, 'Leatherworking', 63), (8, 'Mining', 78), (9, 'Skinning', 102),
(10, 'Tailoring', 72), (11, 'Cooking', 75), (12, 'First Aid', 81), (13, 'Fishing', 97);
INSERT INTO `creature_template` (`entry`, `name`, `subname`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `unit_class`, `unit_flags`, `type`, `flags_extra`)
SELECT @ALMANAC + `n`, 'Almanac Tutor', CONCAT(`name`, ' Trainer'), 1, 1, 35, 1 | 16 | 32, 1, 1, 1, 2 | 256 | 512, 7, 2 FROM `tmp_temucraft_almanac`;
INSERT INTO `creature_default_trainer` (`CreatureId`, `TrainerId`) SELECT @ALMANAC + `n`, `trainer` FROM `tmp_temucraft_almanac`;
-- the look of a stock trainer of that profession
-- (a profession without such a trainer gets no row: the column can't be NULL)
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`)
SELECT * FROM (
    SELECT @ALMANAC + a.`n` AS `id`, 0 AS `idx`, (SELECT m.`CreatureDisplayID` FROM `creature_default_trainer` d JOIN `creature_template_model` m ON m.`CreatureID` = d.`CreatureId`
                                 WHERE d.`TrainerId` = a.`trainer` AND d.`CreatureId` < @ALMANAC ORDER BY d.`CreatureId` LIMIT 1) AS `display`, 1 AS `scale`, 1 AS `prob`
    FROM `tmp_temucraft_almanac` a) t
WHERE t.`display` IS NOT NULL;
DROP TEMPORARY TABLE `tmp_temucraft_almanac`;

-- the Vanity window (mod-temucraft-account-wide)
CREATE TABLE IF NOT EXISTS `mod_temucraft_vanity_item` (
  `item` int unsigned NOT NULL COMMENT 'item_template.entry',
  `category` varchar(32) NOT NULL DEFAULT '' COMMENT 'heading in the Vanity window',
  `sort` smallint unsigned NOT NULL DEFAULT 0 COMMENT 'order inside its heading',
  PRIMARY KEY (`item`)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='Temucraft: vanity items (account collections)';
DELETE FROM `mod_temucraft_vanity_item` WHERE `item` BETWEEN @ITEM AND @ITEM+3;
INSERT INTO `mod_temucraft_vanity_item` (`item`, `category`, `sort`) VALUES
(@ITEM+0, 'Helpers', 0), (@ITEM+1, 'Helpers', 1), (@ITEM+2, 'Helpers', 2), (@ITEM+3, 'Helpers', 3);


-- on the Temucraft Curator's Helpers list (vendor 608011 from mod-temucraft-shop): 150, 150, 200 and 100 tokens
DELETE FROM `npc_vendor` WHERE `entry` = 608011 AND `item` BETWEEN @ITEM AND @ITEM+3;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `VerifiedBuild`) VALUES
(608011, 0, @ITEM+0, 0, 0, 3208, 0),
(608011, 1, @ITEM+1, 0, 0, 3208, 0),
(608011, 2, @ITEM+2, 0, 0, 3210, 0),
(608011, 3, @ITEM+3, 0, 0, 3205, 0);
