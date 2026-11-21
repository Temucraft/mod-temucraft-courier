-- mod-temucraft-courier: player-facing texts. Translations go in module_string_locale (same module and id, plus the locale).
DELETE FROM `module_string` WHERE `module` = 'mod-temucraft-courier';
INSERT INTO `module_string` (`module`, `id`, `string`) VALUES
('mod-temucraft-courier', 1, 'This courier works for someone else.'),
('mod-temucraft-courier', 2, 'Open my bank.'),
('mod-temucraft-courier', 3, 'Open my mailbox.'),
('mod-temucraft-courier', 4, 'You can''t call your courier right now.'),
('mod-temucraft-courier', 5, 'Your courier is on another errand: {} seconds.'),
('mod-temucraft-courier', 6, 'Your courier is here for {} seconds: talk to it for your bank or your mail.'),
('mod-temucraft-courier', 7, 'This helper works for someone else.'),
('mod-temucraft-courier', 8, 'You can''t call a helper right now.'),
('mod-temucraft-courier', 9, 'Only hunters keep a stable.'),
('mod-temucraft-courier', 10, 'Not ready yet: {} seconds.'),
('mod-temucraft-courier', 11, 'You know no profession yet.'),
('mod-temucraft-courier', 12, 'Your helper is here for {} seconds.'),
('mod-temucraft-courier', 13, 'You can''t use this here.');
