-- mod-temucraft-courier: its commands (all under .temucraft) as RBAC permissions 102250-102299. Every command is admin only by
-- default: linked to the administrator roles 192 and 1014. To give one to players, link it to 195 and 1010 here (or
-- grant it per account with .rbac account grant <account> <id>). "Temucraft: use ..." permissions are not
-- commands: they stay linked to the player roles so the items keep working.
DELETE FROM `rbac_linked_permissions` WHERE `linkedId` BETWEEN 102250 AND 102299;
DELETE FROM `rbac_permissions` WHERE `id` BETWEEN 102250 AND 102299;
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(102250, 'Command: temucraft courier'),
(102251, 'Temucraft: use the helper items (Bell, Mirror, Almanac, Lantern)');
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(192, 102250),
(1014, 102250),
(195, 102251),
(1010, 102251);
