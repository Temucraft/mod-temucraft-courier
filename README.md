# mod-temucraft-courier

Your bank and mailbox anywhere. `.courier` calls a Pocket Courier that follows you for 3 minutes; talk to it to open
your bank or your mailbox. Only the player who called it can use it; once every 5 minutes; not in combat,
battlegrounds, arenas or on flights.

C++ and a little SQL (the courier). Settings: `conf/mod-temucraft-courier.conf.dist`.

## Texts and translations

The 6 player-facing texts (chat messages, gossip options) are in the `module_string` table under the module name
`mod-temucraft-courier`, so they can be changed without rebuilding. Translations go in `module_string_locale` with the same
module and id plus the locale (`deDE`, `esES`, `frFR`, ...). If a row is missing the English text in the code is used.

Part of the Temucraft modules for AzerothCore 3.3.5a.
