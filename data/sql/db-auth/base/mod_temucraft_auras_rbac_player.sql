-- mod-temucraft-auras: RBAC permissions 103650-103699, linked to the stock player role 195 and the Temucraft player role 1010 so every
-- account has them. Remove a link (or .rbac account deny <account> <id>) to take the feature away.
DELETE FROM `rbac_linked_permissions` WHERE `linkedId` BETWEEN 103650 AND 103699;
DELETE FROM `rbac_permissions` WHERE `id` BETWEEN 103650 AND 103699;
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(103650, 'Temucraft: use cosmetic aura toys');
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(195, 103650),
(1010, 103650);
