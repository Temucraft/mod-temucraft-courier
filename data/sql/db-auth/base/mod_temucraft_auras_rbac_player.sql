-- mod-temucraft-auras: RBAC permissions 103650-103699, linked to the stock player role 195 and the Temucraft player role 1010 so every
-- account has them. Remove a link (or .rbac account deny <account> <id>) to take the feature away.
-- Re-runnable without touching other roles' links (role 1000 on the PTR, links an admin added by hand): the permission is
-- updated in place (deleting it would cascade to every link to it), only unused ids in the range are removed, and the
-- player links are added if missing.
DELETE FROM `rbac_permissions` WHERE `id` BETWEEN 103651 AND 103699;
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(103650, 'Temucraft: use cosmetic aura toys')
ON DUPLICATE KEY UPDATE `name` = VALUES(`name`);
INSERT IGNORE INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(195, 103650),
(1010, 103650);
