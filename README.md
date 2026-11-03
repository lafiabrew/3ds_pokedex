# Pokedex 3DS (`3ds_pokedex`)

<p align="center">
  <img src="docs/icon.png" width="72" height="72" alt="3ds_pokedex icon" />
  &nbsp;&nbsp;&nbsp;
  <img src="docs/banner.png" width="256" height="128" alt="3ds_pokedex 3DS banner" />
</p>

<p align="center">
  <img src="docs/screenshot.png" width="360" alt="3ds_pokedex dual-screen 3DS preview" />
</p>

> **Gen 1-7 Stat Encyclopedia (`CTR-P-PKDX`, UniqueId `0x0D3EC`)** — Part of the [**lafiabrew**](https://github.com/orgs/lafiabrew/repositories) Nintendo 3DS Homebrew Suite.

On-console competitive & ROM-hack Pokédex encyclopedia displaying base stats (`HP/Atk/Def/SpA/SpD/Spe`), BST, abilities, and Drayano/ROM-hack notes.

---

## Architecture & Data Flow

```mermaid
flowchart LR
    LIST[Bottom Scrollable National Dex] --> CARD[Top Screen Base Stat Bars + BST]
    CARD --> HACK[ROM-Hack Ability & Type Notes]
```

---

## Controls & Touch Ergonomics

| Button / Input | Action |
|---|---|
| `D-Pad Up / Down` (or Touch Row) | Scroll through Pokémon entries |
| `D-Pad Left / Right` | Page jump 4 entries up/down |
| `Top Screen` | Displays color-coded base stat bars, BST sum, abilities, and ROM-hack notes |

---

## Build, Verify & Install

```bash
make test
make 3ds && make verify-3ds
make qr
```
