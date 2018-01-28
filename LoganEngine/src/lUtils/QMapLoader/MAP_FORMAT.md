# Map format

## Introduction

This document describes the grammar of the .map format this module can load. The goal is to load anything GtkRadiant saves into.

The features are listed following a bottom up principle: it explains the smallest primitive first and the more complex features later.

## Supported features

### Line comment

#### Description

Line comment is a C-Style line comment. It starts with "`//`" and extends to the end of the current line.

#### Code

    //Example comment

### Plane

#### Description

A plane describes an infinite plane ([see Wikipedia](https://en.wikipedia.org/wiki/Plane_(geometry))) and information about texturing the plane.

#### Format

    ( p0x p0y p0z ) ( p1x p1y p1z ) ( p2x p2y p2z ) texture_name offset_x offset_y rot_angle scale_x scale_y unused unused unused

#### Code

    ( 10 0 0 ) ( 10 10 0 ) ( 10 0 10 ) common/caulk 0 0 0 1.0 1.0 0 4 0

### Brush

#### Description

A brush describes a convex shape using its boundary planes.

#### Format

    {
     <List of planes>
    }

#### Code

	{
	 ( -48  24 8 ) ( -48 -16 8 ) ( -48 -16 0 ) common/caulk 1 2 5 3.000000 4.000000 0 4 0
	 (  56  24 8 ) ( -48  24 8 ) ( -48  24 0 ) common/caulk 1 2 5 3.000000 4.000000 0 4 0
	 (  56 -16 8 ) (  56  24 8 ) (  56  24 0 ) common/caulk 1 2 5 3.000000 4.000000 0 4 0
	 ( -48 -16 8 ) (  56 -16 8 ) (  56 -16 0 ) common/caulk 1 2 5 3.000000 4.000000 0 4 0
	 ( -48 -16 8 ) ( -48  24 8 ) (  56  24 8 ) common/caulk 1 2 5 3.000000 4.000000 0 4 0
	 (  56  24 0 ) ( -48  24 0 ) ( -48 -16 0 ) common/caulk 1 2 5 3.000000 4.000000 0 4 0
	}

### Key-value pair

#### Description

A key-value pair is a pair of quoted string. The first string is the key and the second is the value.

#### Format

    "key" "value"

#### Code

    "classname" "worldspawn"

### Entity

#### Description

An entity consists a list of key-value pairs and a list of brushes.

#### Format

    {
     <List of key-vaue pairs>
     <List of brushes>
    }

#### Code

    {
     "classname" "worldspawn"
     {
      ( 10  0  0 ) ( 10 10  0 ) ( 10  0 10 ) common/caulk 0 0 0 1.0 1.0
      ( 20  0  0 ) ( 20  0 10 ) ( 20 10  0 ) common/caulk 0 0 0 1.0 1.0
      (  0 10  0 ) (  0 10 10 ) ( 10 10  0 ) common/caulk 0 0 0 1.0 1.0
      (  0 30  0 ) ( 10 30  0 ) (  0 30 10 ) common/caulk 0 0 0 1.0 1.0
      (  0  0  5 ) ( 10  0  5 ) (  0 10  5 ) common/caulk 0 0 0 1.0 1.0
      (  0  0 10 ) (  0 10 10 ) ( 10  0 10 ) common/caulk 0 0 0 1.0 1.0
     }
    }

### Map

#### Description
A map is a list of entities. A map must contain exactly one worldspawn entity that defines the structure of the map.

#### Format

    <List of entities>

#### Code

    // entity 0
    {
     "classname" "worldspawn"
     // brush 0
     {
      ( -48  24 8 ) ( -48 -16 8 ) ( -48 -16 0 ) common/caulk 1 2 5 3.000000 4.000000 0 4 0
      (  56  24 8 ) ( -48  24 8 ) ( -48  24 0 ) common/caulk 1 2 5 3.000000 4.000000 0 4 0
      (  56 -16 8 ) (  56  24 8 ) (  56  24 0 ) common/caulk 1 2 5 3.000000 4.000000 0 4 0
      ( -48 -16 8 ) (  56 -16 8 ) (  56 -16 0 ) common/caulk 1 2 5 3.000000 4.000000 0 4 0
      ( -48 -16 8 ) ( -48  24 8 ) (  56  24 8 ) common/caulk 1 2 5 3.000000 4.000000 0 4 0
      (  56  24 0 ) ( -48  24 0 ) ( -48 -16 0 ) common/caulk 1 2 5 3.000000 4.000000 0 4 0
     }
    }
    // entity 1
    {
     "origin" "32 32 24"
     "classname" "info_player_start"
     "spawnflags" "0"
    }

## Sources

[Quake MAP Specs](http://www.gamers.org/dEngine/quake/QDP/qmapspec.html)

[The Quake 2 Map Format: Quake MAP files](http://www.gamers.org/dEngine/quake2/Q2DP/Q2DP_Map/Q2DP_Map-2.html)

[Quake Map Format](https://quakewiki.org/wiki/Quake_Map_Format)

[QuakeForge: Map (.map) File Format](http://www.quakeforge.net/doxygen/mapformat.html)

[Quake3World.com - View topic - Q4 map format](https://www.quake3world.com/forum/viewtopic.php?f=10&t=17936)
