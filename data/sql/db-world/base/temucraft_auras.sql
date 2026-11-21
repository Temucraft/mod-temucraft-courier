-- mod-temucraft-auras: the cosmetic aura toys, sold on the Temucraft Curator's Auras list (vendor 608010,
-- added by mod-temucraft-shop) for 100 or 125 tokens (ItemExtendedCost 3205 / 3206). Keep the item -> spell list
-- the same as AURAS in src/TemucraftAuras.cpp.
SET @FIRST := 610110; SET @LAST := 610137; SET @AURAS := 608010;
DELETE FROM `item_template` WHERE `entry` BETWEEN @FIRST AND @LAST;
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`, `name`, `displayid`, `Quality`, `Flags`, `BuyCount`, `BuyPrice`, `SellPrice`, `InventoryType`, `ItemLevel`, `maxcount`, `stackable`, `spellid_1`, `spelltrigger_1`, `spellcharges_1`, `spellcooldown_1`, `bonding`, `description`, `Material`, `ScriptName`) VALUES
(610110, 15, 0, -1, 'Naaru''s Grace', 23727, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Soft light from the naaru circles you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610111, 15, 0, -1, 'Holy Halo', 24730, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'A bright holy glow around you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610112, 15, 0, -1, 'Arcane Spark', 3231, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Arcane energy crackles around you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610113, 15, 0, -1, 'Frost Spark', 2935, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Frost energy crackles around you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610114, 15, 0, -1, 'Shadow Spark', 38621, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Shadow energy crackles around you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610115, 15, 0, -1, 'Holy Spark', 29175, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Holy energy crackles around you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610116, 15, 0, -1, 'Flame Orb', 38618, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'A ball of fire floats with you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610117, 15, 0, -1, 'Purple Ghost', 32819, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'A ghostly purple mist hangs around you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610118, 15, 0, -1, 'Frost Power', 38622, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Cold power shines from you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610119, 15, 0, -1, 'Red Glow', 35850, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'You glow red. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610120, 15, 0, -1, 'Blue Glow', 35851, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'You glow blue. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610121, 15, 0, -1, 'Green Glow', 35849, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'You glow green. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610122, 15, 0, -1, 'Dark Glow', 35846, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'You glow dark. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610123, 15, 0, -1, 'Orange Glow', 35844, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'You glow orange. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610124, 15, 0, -1, 'Yellow Glow', 35842, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'You glow yellow. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610125, 15, 0, -1, 'Purple Glow', 40051, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'You glow purple. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610126, 15, 0, -1, 'Shadowy Hands', 3109, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Shadow wraps your hands. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610127, 15, 0, -1, 'Frosty Hands', 961, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Frost wraps your hands. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610128, 15, 0, -1, 'Holy Hands', 58601, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Holy light wraps your hands. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610129, 15, 0, -1, 'Ribbon Trail', 32746, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Ribbons trail behind you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610130, 15, 0, -1, 'Sparkle Hand', 2480, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Your right hand sparkles. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610131, 15, 0, -1, 'Lovestruck', 34508, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Little hearts float around you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610132, 15, 0, -1, 'Earthen Soul', 38617, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'An earthen spirit glows within you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610133, 15, 0, -1, 'Fiery Soul', 38618, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'A fiery spirit glows within you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610134, 15, 0, -1, 'Watery Soul', 38622, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'A watery spirit glows within you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610135, 15, 0, -1, 'Blue Cyclone', 38620, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'A small blue whirlwind spins around you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610136, 15, 0, -1, 'Smoke Cloud', 32819, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Smoke drifts off you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura'),
(610137, 15, 0, -1, 'Glinting Bugs', 2480, 3, 0, 1, 0, 0, 0, 1, 1, 1, 18282, 0, 0, 1500, 1, 'Tiny glinting bugs buzz around you. Use again to switch it off. Hidden in battlegrounds and arenas.', 4, 'item_temucraft_aura');
DELETE FROM `npc_vendor` WHERE `entry` = @AURAS;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `VerifiedBuild`) VALUES
(@AURAS, 0, 610110, 0, 0, 3206, 0),
(@AURAS, 1, 610111, 0, 0, 3206, 0),
(@AURAS, 2, 610112, 0, 0, 3205, 0),
(@AURAS, 3, 610113, 0, 0, 3205, 0),
(@AURAS, 4, 610114, 0, 0, 3205, 0),
(@AURAS, 5, 610115, 0, 0, 3205, 0),
(@AURAS, 6, 610116, 0, 0, 3206, 0),
(@AURAS, 7, 610117, 0, 0, 3206, 0),
(@AURAS, 8, 610118, 0, 0, 3205, 0),
(@AURAS, 9, 610119, 0, 0, 3205, 0),
(@AURAS, 10, 610120, 0, 0, 3205, 0),
(@AURAS, 11, 610121, 0, 0, 3205, 0),
(@AURAS, 12, 610122, 0, 0, 3205, 0),
(@AURAS, 13, 610123, 0, 0, 3205, 0),
(@AURAS, 14, 610124, 0, 0, 3205, 0),
(@AURAS, 15, 610125, 0, 0, 3205, 0),
(@AURAS, 16, 610126, 0, 0, 3205, 0),
(@AURAS, 17, 610127, 0, 0, 3205, 0),
(@AURAS, 18, 610128, 0, 0, 3205, 0),
(@AURAS, 19, 610129, 0, 0, 3206, 0),
(@AURAS, 20, 610130, 0, 0, 3205, 0),
(@AURAS, 21, 610131, 0, 0, 3206, 0),
(@AURAS, 22, 610132, 0, 0, 3206, 0),
(@AURAS, 23, 610133, 0, 0, 3206, 0),
(@AURAS, 24, 610134, 0, 0, 3206, 0),
(@AURAS, 25, 610135, 0, 0, 3206, 0),
(@AURAS, 26, 610136, 0, 0, 3205, 0),
(@AURAS, 27, 610137, 0, 0, 3205, 0);

-- the Vanity window (mod-temucraft-account-wide): bought once, any character of the account can take it again
CREATE TABLE IF NOT EXISTS `mod_temucraft_vanity_item` (
  `item` int unsigned NOT NULL COMMENT 'item_template.entry',
  `category` varchar(32) NOT NULL DEFAULT '' COMMENT 'heading in the Vanity window',
  `sort` smallint unsigned NOT NULL DEFAULT 0 COMMENT 'order inside its heading',
  PRIMARY KEY (`item`)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='Temucraft: vanity items (account collections)';
DELETE FROM `mod_temucraft_vanity_item` WHERE `item` BETWEEN @FIRST AND @LAST;
INSERT INTO `mod_temucraft_vanity_item` (`item`, `category`, `sort`) SELECT `entry`, 'Auras', `entry` - @FIRST FROM `item_template` WHERE `entry` BETWEEN @FIRST AND @LAST;

DELETE FROM `module_string` WHERE `module` = 'mod-temucraft-auras';
INSERT INTO `module_string` (`module`, `id`, `string`) VALUES
('mod-temucraft-auras', 1, 'Aura off.'),
('mod-temucraft-auras', 2, 'Aura chosen. It shows once you leave battlegrounds, arenas or combat.'),
('mod-temucraft-auras', 3, 'Aura on. Use the same toy again to switch it off.'),
('mod-temucraft-auras', 4, 'You can''t use this here.');
