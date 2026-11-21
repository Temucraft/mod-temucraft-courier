# mod-temucraft-courier

Your bank and mailbox anywhere. `.temucraft courier` calls a Pocket Courier that follows you for 3 minutes; talk to it to open
your bank or your mailbox. Only the player who called it can use it; once every 5 minutes; not in combat,
battlegrounds, arenas or on flights.

C++ and a little SQL (the courier). Settings: `conf/mod-temucraft-courier.conf.dist`.

## Texts and translations

The 6 player-facing texts (chat messages, gossip options) are in the `module_string` table under the module name
`mod-temucraft-courier`, so they can be changed without rebuilding. Translations go in `module_string_locale` with the same
module and id plus the locale (`deDE`, `esES`, `frFR`, ...). If a row is missing the English text in the code is used.

Part of the Temucraft modules for AzerothCore 3.3.5a.

## Portable helpers

Items that call a helper for a few minutes. Each helper works only for the one who called it, not in combat,
battlegrounds or arenas, with its own cooldown. All are in the Vanity window under "Helpers" and sold on the Temucraft
Curator's Helpers list (mod-temucraft-shop).

| Item | Entry | What it does |
|---|---|---|
| Courier's Bell | 610140 | Calls your Pocket Courier, like `.temucraft courier` (same cooldown). |
| Tailor's Folding Mirror | 610141 | A wardrobe mirror: mod-transmog's transmogrifier (needs mod-transmog). |
| Artisan's Almanac | 610142 | Pick one of your professions: its trainer list, normal prices. |
| Beastkeeper's Lantern | 610143 | A stable master (hunters). |

The Almanac's tutors (608410-608423) teach what one stock trainer of each profession teaches
(`creature_default_trainer` in `mod_temucraft_courier_helpers.sql`; change the trainer ids there).
