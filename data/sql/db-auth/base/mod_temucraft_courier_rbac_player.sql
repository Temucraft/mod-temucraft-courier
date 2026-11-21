-- mod-temucraft-courier: its player commands (all under .temucraft) as RBAC permissions 102250-102299. Linked to the stock player
-- role 195 and the Temucraft player role 1010, so every account has them by default. To take one away, remove its
-- link here (or deny it per account with .rbac account deny <account> <id>); to give it to staff only, link it to
-- 1011-1014 instead.
DELETE FROM `rbac_linked_permissions` WHERE `linkedId` BETWEEN 102250 AND 102299;
DELETE FROM `rbac_permissions` WHERE `id` BETWEEN 102250 AND 102299;
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(102250, 'Command: temucraft courier'),
(102251, 'Temucraft: use the helper items (Bell, Mirror, Almanac, Lantern)');
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(195, 102250),
(1010, 102250),
(195, 102251),
(1010, 102251);
